use std::path::PathBuf;
pub fn root() -> PathBuf {
    compiler_core::routing::root().to_path_buf()
}
#[derive(Debug, Clone)]
pub enum OverlaySource {
    Path(PathBuf),
    Text(String),
    /// In-memory listing that belongs to a known overlay: the overlay id
    /// cannot be read off a path, so it travels with the text.
    Named {
        overlay: String,
        text: String,
    },
}
impl OverlaySource {
    pub fn path(path: impl Into<PathBuf>) -> Self {
        OverlaySource::Path(path.into())
    }
    pub fn text(text: impl Into<String>) -> Self {
        OverlaySource::Text(text.into())
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
            OverlaySource::Path(path) => path
                .file_name()
                .and_then(|name| name.to_str())
                .and_then(|name| name.strip_suffix("_overlay.s"))
                .map(str::to_owned),
            OverlaySource::Text(_) => None,
        }
    }
    pub fn to_display_string(&self) -> String {
        match self {
            OverlaySource::Path(path) => path.display().to_string(),
            OverlaySource::Text(_) => "in-memory assembly".into(),
            OverlaySource::Named { overlay, .. } => format!("{overlay}_overlay.s"),
        }
    }
    pub fn read_text(&self) -> std::io::Result<String> {
        match self {
            OverlaySource::Path(path) => std::fs::read_to_string(path),
            OverlaySource::Text(text) | OverlaySource::Named { text, .. } => Ok(text.clone()),
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn source_kind_does_not_depend_on_filesystem_state() {
        let work = tempfile::tempdir().unwrap();
        let path = work.path().join("resource_370_overlay.s");
        let file = OverlaySource::path(&path);
        let text = OverlaySource::text(path.to_str().unwrap());
        assert_eq!(
            file.read_text().unwrap_err().kind(),
            std::io::ErrorKind::NotFound
        );
        std::fs::write(&path, ".byte 1\n").unwrap();
        assert_eq!(file.read_text().unwrap(), ".byte 1\n");
        assert_eq!(text.read_text().unwrap(), path.to_str().unwrap());
        assert_eq!(file.overlay_id().as_deref(), Some("resource_370"));
        assert_eq!(text.overlay_id(), None);
        assert_eq!(
            OverlaySource::named("resource_371", ".byte 2")
                .overlay_id()
                .as_deref(),
            Some("resource_371")
        );
    }
}
