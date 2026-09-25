//! Weyard UI: the one design of the README figures and the dashboard. A teal
//! face with one-pixel bevels, a darker well cut into it, white labels with
//! a black shadow, gold and blue for the two games; every box has pixel-
//! stepped corners of `CORNER` and every light bevel line is drawn at
//! `LIGHT_OPACITY` over what it sits on. The figures draw these constants;
//! the dashboard receives them as CSS custom properties.

/// Every raised or sunken box's corner, in game pixels: the pixels nearer the
/// corner than this step along the diagonal are cut away, so the bevel steps
/// diagonally past them. 2 is the smallest step, the corner pixel alone.
pub(crate) const CORNER: i32 = 5;
/// The opacity in percent of every light bevel line (`LIGHT`, and `BAND` as
/// the inner highlight); dark bevels stay opaque.
pub(crate) const LIGHT_OPACITY: u32 = 75;

/// The raised face of every panel, chart and folder.
pub(crate) const FACE: &str = "#1f7f93";
/// Bevel light (top and left of a raised face) and dark (bottom and right).
pub(crate) const LIGHT: &str = "#c9e1dc";
pub(crate) const DARK: &str = "#103840";
/// A sunken well cut into the face, with its quiet rules.
pub(crate) const WELL: &str = "#17606f";
pub(crate) const GRID: &str = "#246f7e";
/// A lighter band across a well: a marked day, a selected row.
pub(crate) const BAND: &str = "#3a8c9c";
/// Labels: white ink over a black one-pixel shadow; muted and hover tones.
pub(crate) const INK: &str = "#ffffff";
pub(crate) const SHADOW: &str = "#000000";
pub(crate) const MUTED: &str = "#b4ccd2";
pub(crate) const HOVER: &str = "#fff3ac";
/// The Broken Seal in gold, The Lost Age in a pale window blue.
pub(crate) const GOLD: &str = "#f4c84f";
pub(crate) const BLUE: &str = "#a8c4f8";

/// The palette as the dashboard's CSS custom properties.
pub(crate) const CSS: [(&str, &str); 13] = [
    ("face", FACE),
    ("light", LIGHT),
    ("dark", DARK),
    ("well", WELL),
    ("grid", GRID),
    ("band", BAND),
    ("ink", INK),
    ("shadow", SHADOW),
    ("muted", MUTED),
    ("hover", HOVER),
    ("gold", GOLD),
    ("blue", BLUE),
    ("unknown", super::boxtree::UNKNOWN),
];

/// Whether the pixel `across` and `down` from a box's corner is cut away by a
/// corner of step `corner`: those before its diagonal.
pub(crate) fn cut(across: i32, down: i32, corner: i32) -> bool {
    across + down < corner - 1
}
/// The step a box `width` by `height` rounds with: `CORNER`, or the smallest
/// step on a box too small for it, and none on one too small for any.
pub(crate) fn corner_for(width: i32, height: i32) -> i32 {
    match width.min(height) {
        side if side > 2 * CORNER => CORNER,
        side if side >= 3 => 2.min(CORNER),
        _ => 0,
    }
}
/// A CSS `clip-path` keeping a box's pixels inside its stepped corners, each
/// step `unit` CSS pixels; with `cap`, a step is at most that percentage of
/// the box, so a treemap tile of a pixel or two keeps its smallest step.
pub(crate) fn clip_path(unit: u32, cap: Option<u32>) -> String {
    let length = |n: i32| {
        let px = format!("{}px", n as u32 * unit);
        match (n, cap) {
            (0, _) => (String::from("0"), String::from("100%")),
            (_, Some(cap)) => (
                format!("min({px},{cap}%)"),
                format!("calc(100% - min({px},{cap}%))"),
            ),
            (_, None) => (px.clone(), format!("calc(100% - {px})")),
        }
    };
    // The top-left stair from the left edge to the top edge, clockwise.
    let mut stair = Vec::new();
    for k in 0..CORNER - 1 {
        stair.push((k, CORNER - 1 - k));
        stair.push((k + 1, CORNER - 1 - k));
    }
    stair.push((CORNER - 1, 0));
    let mut points = Vec::new();
    for turn in 0..4 {
        for (a, b) in &stair {
            let ((a0, a1), (b0, b1)) = (length(*a), length(*b));
            points.push(match turn {
                0 => format!("{a0} {b0}"),
                1 => format!("{b1} {a0}"),
                2 => format!("{a1} {b1}"),
                _ => format!("{b0} {a1}"),
            });
        }
    }
    format!("polygon({})", points.join(","))
}
/// The CSS colour of a light bevel line drawn at `LIGHT_OPACITY`.
pub(crate) fn bevel_light(token: &str) -> String {
    format!("color-mix(in srgb,var(--{token}) {LIGHT_OPACITY}%,transparent)")
}

#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn corners_step_on_whole_pixels() {
        assert_eq!(CORNER, 5);
        // A two-pixel step cuts the corner pixel alone.
        assert!(cut(0, 0, 2) && !cut(1, 0, 2) && !cut(0, 1, 2));
        assert!(!cut(0, 0, 0));
        assert_eq!(
            (corner_for(40, 5), corner_for(3, 9), corner_for(2, 9)),
            (2, 2, 0)
        );
        // Each corner is a stair of CORNER - 1 steps: 2 * (CORNER - 1) + 1 points.
        let one = clip_path(1, None);
        assert_eq!(
            one.matches(',').count() + 1,
            4 * (2 * (CORNER as usize - 1) + 1)
        );
        assert!(one.starts_with("polygon(0 4px,1px 4px,1px 3px"));
        assert!(
            clip_path(2, Some(30)).starts_with("polygon(0 min(8px,30%),min(2px,30%) min(8px,30%)")
        );
    }
}
