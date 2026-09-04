use std::path::{Path, PathBuf};
pub fn root() -> PathBuf {
    compiler_core::routing::root().to_path_buf()
}
pub fn basename(path: &str) -> &str {
    let trimmed = path.trim_end_matches('/');
    if trimmed.is_empty() {
        return if path.is_empty() { "" } else { "/" };
    }
    match trimmed.rfind('/') {
        Some(at) => &trimmed[at + 1..],
        None => trimmed,
    }
}
#[derive(Debug, Clone)]
pub enum OverlaySource {
    Url(PathBuf),
    Str(String),
    /// In-memory listing that belongs to a known overlay: the overlay id
    /// cannot be read off a path, so it travels with the text.
    Named {
        overlay: String,
        text: String,
    },
}
impl OverlaySource {
    pub fn path(path: impl Into<PathBuf>) -> Self {
        OverlaySource::Str(path.into().to_string_lossy().to_string())
    }
    pub fn text(text: impl Into<String>) -> Self {
        OverlaySource::Str(text.into())
    }
    pub fn named(overlay: impl Into<String>, text: impl Into<String>) -> Self {
        OverlaySource::Named {
            overlay: overlay.into(),
            text: text.into(),
        }
    }
    /// The overlay this listing belongs to, when the source says so.
    pub fn overlay_id(&self) -> Option<String> {
        match self {
            OverlaySource::Named { overlay, .. } => Some(overlay.clone()),
            _ => None,
        }
    }
    pub fn to_display_string(&self) -> String {
        match self {
            OverlaySource::Url(path) => format!("file://{}", path.to_string_lossy()),
            OverlaySource::Str(text) => text.clone(),
            OverlaySource::Named { overlay, .. } => format!("{overlay}_overlay.s"),
        }
    }
    pub fn read_text(&self) -> std::io::Result<String> {
        match self {
            OverlaySource::Url(path) => std::fs::read_to_string(path),
            OverlaySource::Str(text) => {
                if !text.contains('\n') && Path::new(text).exists() {
                    std::fs::read_to_string(text)
                } else {
                    Ok(text.clone())
                }
            }
            OverlaySource::Named { text, .. } => Ok(text.clone()),
        }
    }
}
