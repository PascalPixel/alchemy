//! The one chrome palette of the README figures and the dashboard: a teal
//! face with one-pixel bevels, a darker well cut into it, white labels with
//! a black shadow, gold and blue for the two games. The figures draw these
//! constants; the dashboard receives them as CSS custom properties.

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
