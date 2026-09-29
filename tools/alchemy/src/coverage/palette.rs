//! Weyard UI: the one design of the README figures. A teal
//! face with one-pixel bevels, a darker well cut into it, white labels with
//! a black shadow, gold and blue for the two games; every box has pixel-
//! stepped corners of `CORNER` and every light bevel line is drawn at
//! `LIGHT_OPACITY` over what it sits on. The figures draw these constants.

/// Every raised or sunken box's corner, in game pixels: the pixels nearer the
/// corner than this step along the diagonal are cut away, so the bevel steps
/// diagonally past them. 2 is the smallest step, the corner pixel alone.
pub(crate) const CORNER: i32 = 5;
/// The opacity in percent of every light bevel line (`LIGHT`, and `BAND` as
/// the inner highlight); dark bevels stay opaque.
pub(crate) const LIGHT_OPACITY: u32 = 50;

/// The raised face of every panel, chart and folder.
pub(crate) const FACE: &str = "#1f7f93";
/// Bevel light (top and left of a raised face) and dark (bottom and right).
pub(crate) const LIGHT: &str = "#c9e1dc";
pub(crate) const DARK: &str = "#103840";
/// A sunken well cut into the face, with its quiet rules.
pub(crate) const WELL: &str = "#17606f";
pub(crate) const GRID: &str = "#246f7e";
/// A lighter band across a well: a marked day, a selected row.
pub(crate) const BAND: &str = "#6e2a30";
/// Labels: white ink over a black one-pixel shadow; muted and hover tones.
pub(crate) const INK: &str = "#ffffff";
pub(crate) const SHADOW: &str = "#000000";
pub(crate) const MUTED: &str = "#b4ccd2";
/// The Broken Seal in gold, The Lost Age in a pale window blue.
pub(crate) const GOLD: &str = "#f4c84f";
pub(crate) const BLUE: &str = "#a8c4f8";

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
    }
}
