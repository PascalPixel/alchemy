
pub(crate) const CORNER: i32 = 8;
pub(crate) const LIGHT_OPACITY: u32 = 50;

pub(crate) const FACE: &str = "#1f7f93";
pub(crate) const LIGHT: &str = "#c9e1dc";
pub(crate) const DARK: &str = "#103840";
pub(crate) const WELL: &str = "#17606f";
pub(crate) const GRID: &str = "#246f7e";
pub(crate) const INK: &str = "#ffffff";
pub(crate) const SHADOW: &str = "#000000";
pub(crate) const MUTED: &str = "#b4ccd2";
pub(crate) const GOLD: &str = "#f4c84f";
pub(crate) const BLUE: &str = "#a8c4f8";
/// DONE data's line: each game's colour, darker.
pub(crate) const DARK_GOLD: &str = "#b98a1c";
pub(crate) const DARK_BLUE: &str = "#5078c8";

pub(crate) fn cut(across: i32, down: i32, corner: i32) -> bool {
    if across < 0 || down < 0 {
        return true;
    }
    let (dx, dy) = (2 * (corner - across) - 1, 2 * (corner - down) - 1);
    across < corner && down < corner && dx * dx + dy * dy > 4 * corner * corner
}
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
        assert_eq!(CORNER, 8);
        assert!(cut(0, 0, 2) && !cut(1, 0, 2) && !cut(0, 1, 2));
        assert!(!cut(0, 0, 0));
        let row = |down| (0..5).filter(|&across| cut(across, down, 5)).count();
        assert_eq!((row(0), row(1), row(2), row(3)), (3, 1, 1, 0));
        assert_eq!(
            (corner_for(40, 5), corner_for(3, 9), corner_for(2, 9)),
            (2, 2, 0)
        );
    }
}
