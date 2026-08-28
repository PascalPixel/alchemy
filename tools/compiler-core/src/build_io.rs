//! Small, shared path and argv operations used by the production build stages.

use std::path::{Path, PathBuf};

pub fn argv(items: &[&str]) -> Vec<String> {
    items.iter().map(|item| (*item).into()).collect()
}

pub fn text(path: impl AsRef<Path>) -> String {
    path.as_ref().to_string_lossy().into_owned()
}

pub fn read(path: impl AsRef<Path>) -> Result<Vec<u8>, String> {
    let path = path.as_ref();
    std::fs::read(path).map_err(|error| format!("{}: {error}", path.display()))
}

pub fn write(path: impl AsRef<Path>, bytes: impl AsRef<[u8]>) -> Result<(), String> {
    let path = path.as_ref();
    std::fs::write(path, bytes).map_err(|error| format!("{}: {error}", path.display()))
}

pub fn read_json<T: serde::de::DeserializeOwned>(path: impl AsRef<Path>) -> Result<T, String> {
    let path = path.as_ref();
    serde_json::from_slice(&read(path)?).map_err(|error| format!("{}: {error}", path.display()))
}

pub fn rooted(root: impl AsRef<Path>, path: impl AsRef<Path>) -> PathBuf {
    let path = path.as_ref();
    if path.is_absolute() {
        path.into()
    } else {
        root.as_ref().join(path)
    }
}

pub fn relative(root: impl AsRef<Path>, path: impl AsRef<Path>) -> String {
    let (root, path) = (root.as_ref(), path.as_ref());
    text(path.strip_prefix(root).unwrap_or(path)).replace('\\', "/")
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn relative_only_strips_the_repository_root() {
        assert_eq!(
            relative("/repo", "/repo/games/gs1/src/a.c"),
            "games/gs1/src/a.c"
        );
        assert_eq!(
            relative("/repo", "/opt/toolchain/agscc"),
            "/opt/toolchain/agscc"
        );
        assert_eq!(relative("/repo", "games/gs1/src/a.c"), "games/gs1/src/a.c");
    }

    #[cfg(unix)]
    #[test]
    fn relative_keeps_non_utf8_components_visible() {
        use std::{ffi::OsString, os::unix::ffi::OsStringExt};

        let path = PathBuf::from(OsString::from_vec(b"/outside/\xff/file.c".to_vec()));
        let rendered = relative("/repo", path);
        assert!(rendered.starts_with("/outside/"));
        assert!(rendered.ends_with("/file.c"));
        assert!(!rendered.contains("//"));
    }
}
