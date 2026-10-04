//! Small, shared path and argv operations used by the production build stages.

use std::path::{Path, PathBuf};

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

pub fn rooted(root: impl AsRef<Path>, path: impl AsRef<Path>) -> PathBuf {
    let path = path.as_ref();
    if path.is_absolute() {
        path.into()
    } else {
        root.as_ref().join(path)
    }
}
