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

/// Generated build products cannot become maintained inputs, including through symlinks.
pub fn generated_directory(root: &Path, path: &Path) -> Result<PathBuf, String> {
    let root = std::fs::canonicalize(root).map_err(|error| error.to_string())?;
    let path = rooted(&root, path);
    let allowed = root.join("out");
    if !path.starts_with(&allowed)
        || path
            .components()
            .any(|part| part == std::path::Component::ParentDir)
    {
        return Err(
            "generated outputs must stay under the repository's ignored out/ directory".into(),
        );
    }
    let mut parent = path.as_path();
    while std::fs::symlink_metadata(parent).is_err() {
        parent = parent.parent().ok_or("output has no existing parent")?;
    }
    let parent = std::fs::canonicalize(parent).map_err(|error| error.to_string())?;
    if parent != root && !parent.starts_with(&allowed) {
        return Err("generated output symlink escapes ignored out/".into());
    }
    std::fs::create_dir_all(&path).map_err(|error| error.to_string())?;
    let path = std::fs::canonicalize(path).map_err(|error| error.to_string())?;
    if !path.starts_with(&allowed) {
        return Err("generated output escapes ignored out/".into());
    }
    Ok(path)
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn generated_products_cannot_overwrite_source_or_escape_out() {
        let root = tempfile::tempdir().unwrap();
        for path in ["recon/tbs", "out/../recon/tbs"] {
            assert!(generated_directory(root.path(), Path::new(path)).is_err());
        }
        assert!(!root.path().join("recon").exists());
        assert!(
            generated_directory(root.path(), Path::new("out/tbs-en/native"))
                .unwrap()
                .is_dir()
        );
    }

    #[cfg(unix)]
    #[test]
    fn output_symlink_is_rejected_before_creating_outside_directories() {
        let root = tempfile::tempdir().unwrap();
        let outside = tempfile::tempdir().unwrap();
        std::fs::create_dir(root.path().join("out")).unwrap();
        std::os::unix::fs::symlink(outside.path(), root.path().join("out/link")).unwrap();
        assert!(generated_directory(root.path(), Path::new("out/link/new")).is_err());
        assert!(!outside.path().join("new").exists());
    }
}
