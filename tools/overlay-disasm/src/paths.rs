//! Node path helpers and the `string | URL` source discrimination.

use std::path::{Path, PathBuf};

/// Repository root, found from this crate's location under `tools`.
///
/// PORT NOTE: commit `b3ab4841b` moved files between `tools/` subfolders and
/// adjusted `ROOT` inconsistently in several modules, leaving stale literal
/// path segments. The only production use is `join(ROOT, "exact")`, a
/// directory that still exists.
pub fn root() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR"))
        .parent()
        .expect("crate dir has a parent")
        .parent()
        .expect("tools has a parent")
        .to_path_buf()
}

/// `path.basename(p)`.
///
/// PORT NOTE: Node splits on `/` only (on POSIX) and strips trailing
/// separators. It is NOT `Path::file_name`, which returns `None` for `".."`
/// and for a trailing-slash path, where Node returns `".."` and the last
/// segment respectively. This module feeds `basename` raw strings that may not
/// be paths at all (`assembleOverlay` calls `basename(String(source))` on a
/// whole assembly listing), so the string behaviour is the one that matters.
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

/// `path.extname(p)`: the last `.` in the basename, empty when the basename
/// starts with it or has none.
pub fn extname(path: &str) -> &str {
    let base = basename(path);
    match base.rfind('.') {
        Some(0) | None => "",
        Some(at) => &base[at..],
    }
}

/// `path.basename(p, ext)`: the basename with `ext` removed when it is a
/// proper suffix. Node does not strip when basename == ext.
pub fn basename_without<'a>(path: &'a str, ext: &str) -> &'a str {
    let base = basename(path);
    if !ext.is_empty() && base.len() > ext.len() && base.ends_with(ext) {
        &base[..base.len() - ext.len()]
    } else {
        base
    }
}

/// The TypeScript's `source: string | URL`.
///
/// PORT NOTE: the discrimination is behavioural, not typed. `sourceText` reads
/// a `URL` always; reads a `string` only when it contains no newline AND names
/// an existing file; otherwise treats the string as the assembly text itself.
/// `overlayCSources` returns `[]` for a `URL`, for a multi-line string and for
/// a non-existent path. Both rules are reproduced exactly.
#[derive(Debug, Clone)]
pub enum OverlaySource {
    /// A `URL`, as produced by `import.meta.resolve` / `pathToFileURL`.
    Url(PathBuf),
    /// A plain `string`, which may be a path or may be the assembly text.
    Str(String),
}

impl OverlaySource {
    /// A plain filesystem path -- the shape every production caller of the
    /// TS original actually uses (`assembleOverlay(join(ROOT, ...))`), never
    /// a `URL` object. This is the TS `string` branch, not the `URL` branch:
    /// it must still go through the newline/existence check so
    /// `c_source_anchor` and `read_text` behave like `sourceText`/
    /// `overlayCSources` do for a string path. Was wrongly wired to
    /// `OverlaySource::Url` (which skips C-source lookup entirely, per the
    /// TS `source instanceof URL` short-circuit), silently dropping every
    /// exact/*.c patch for every overlay built this way.
    pub fn path(path: impl Into<PathBuf>) -> Self {
        OverlaySource::Str(path.into().to_string_lossy().to_string())
    }

    pub fn text(text: impl Into<String>) -> Self {
        OverlaySource::Str(text.into())
    }

    /// `String(source)`.
    ///
    /// PORT NOTE: `String(url)` is the href, `file:///a/b.s`, not the fspath.
    /// Every consumer here immediately takes `basename` of it, and the last
    /// `/`-segment of the href equals the last segment of the path, so the two
    /// agree for this module's purposes. Reproduced as the href anyway so a
    /// future consumer that does something else with it does not silently
    /// diverge.
    pub fn to_display_string(&self) -> String {
        match self {
            OverlaySource::Url(path) => format!("file://{}", path.to_string_lossy()),
            OverlaySource::Str(text) => text.clone(),
        }
    }

    /// `sourceText(source)`.
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
        }
    }

    /// The path `overlayCSources` will accept, or `None` when the source is a
    /// `URL`, multi-line, or does not exist.
    pub fn c_source_anchor(&self) -> Option<&str> {
        match self {
            OverlaySource::Url(_) => None,
            OverlaySource::Str(text) => {
                if text.contains('\n') || !Path::new(text).exists() {
                    None
                } else {
                    Some(text)
                }
            }
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn basename_matches_node_not_path_file_name() {
        assert_eq!(basename("/a/b/c.s"), "c.s");
        assert_eq!(basename("c.s"), "c.s");
        assert_eq!(basename("/a/b/"), "b");
        assert_eq!(basename(".."), "..");
        assert_eq!(basename("/"), "/");
        assert_eq!(basename(""), "");
        // A whole assembly listing is not a path; Node still answers.
        assert_eq!(basename(".syntax unified\n\t.thumb"), ".syntax unified\n\t.thumb");
    }

    #[test]
    fn extname_matches_node() {
        assert_eq!(extname("/a/b.c"), ".c");
        assert_eq!(extname("/a/.hidden"), "");
        assert_eq!(extname("/a/b"), "");
        assert_eq!(extname("/a/b.tar.gz"), ".gz");
    }

    #[test]
    fn basename_without_does_not_strip_a_whole_name() {
        assert_eq!(basename_without("/x/a_c_02000240.c", ".c"), "a_c_02000240");
        assert_eq!(basename_without("/x/.c", ".c"), ".c");
        assert_eq!(basename_without("/x/a", ".c"), "a");
    }
}
