//! The `node:path` (posix) operations this tool uses.
//!
//! `std::path` is close but not equal: `Path::file_stem` on `"a.tar.gz"` is
//! `"a.tar"` and node's `basename(p, ".gz")` is `"a.tar"` too, but node's
//! `extname` returns `""` for a leading-dot file where `std` returns the whole
//! name, and node's `relative` never returns a `.`-prefixed path when the base
//! is a prefix. The differences are small and every one of them lands in an
//! emitted linker script or manifest, so they are written out here.
//!
//! `basename` and `extname` themselves come from `alchemy-plan`, which already
//! owns proved ports of both. A second copy of them is exactly the hand-sync
//! problem this effort exists to end.

pub use alchemy_plan::nodepath::{basename, extname};

/// `basename(path, ext)` -- the two-argument form, which strips the suffix only
/// when it is not the whole name.
pub fn basename_with_ext<'a>(path: &'a str, ext: &str) -> &'a str {
    let name = basename(path);
    if !ext.is_empty() && name.len() > ext.len() && name.ends_with(ext) {
        &name[..name.len() - ext.len()]
    } else {
        name
    }
}

/// `isAbsolute(path)` on posix.
pub fn is_absolute(path: &str) -> bool {
    path.starts_with('/')
}

/// `join(a, b)` for the shapes this tool builds. Node normalises the result;
/// the inputs here never contain `.` or `..` segments, and the normalisation
/// that does matter is the collapsing of duplicate separators.
pub fn join(left: &str, right: &str) -> String {
    if left.is_empty() {
        return normalize(right);
    }
    if right.is_empty() {
        return normalize(left);
    }
    normalize(&format!("{}/{}", left.trim_end_matches('/'), right))
}

/// `dirname(path)` on posix.
pub fn dirname(path: &str) -> &str {
    if path == "/" {
        return "/";
    }
    let trimmed = path.trim_end_matches('/');
    match trimmed.rfind('/') {
        None => ".",
        Some(0) => "/",
        Some(at) => &trimmed[..at],
    }
}

/// `resolve(base, path)`: an absolute `path` wins outright, otherwise it is
/// appended to `base`.
pub fn resolve(base: &str, path: &str) -> String {
    if is_absolute(path) {
        normalize(path)
    } else {
        join(base, path)
    }
}

/// `relative(from, to)` for the case this tool uses: both absolute, `from` a
/// proper ancestor of `to`. The general case falls back to `to` unchanged,
/// which is also what node does when no relative route exists.
pub fn relative(from: &str, to: &str) -> String {
    let from = normalize(from);
    let to = normalize(to);
    if to == from {
        return String::new();
    }
    let prefix = format!("{}/", from.trim_end_matches('/'));
    if let Some(rest) = to.strip_prefix(&prefix) {
        return rest.to_string();
    }
    let from_parts: Vec<&str> = from.split('/').filter(|p| !p.is_empty()).collect();
    let to_parts: Vec<&str> = to.split('/').filter(|p| !p.is_empty()).collect();
    let shared = from_parts
        .iter()
        .zip(to_parts.iter())
        .take_while(|(a, b)| a == b)
        .count();
    let mut out: Vec<&str> = vec![".."; from_parts.len() - shared];
    out.extend(&to_parts[shared..]);
    out.join("/")
}

/// Collapse repeated separators and strip a trailing one. Node's `normalize`
/// also resolves `.`/`..`; no path this tool constructs contains either, and a
/// silent resolution of one would be a behaviour change rather than a fix.
fn normalize(path: &str) -> String {
    let absolute = path.starts_with('/');
    let body: Vec<&str> = path.split('/').filter(|part| !part.is_empty()).collect();
    let joined = body.join("/");
    if absolute {
        format!("/{joined}")
    } else {
        joined
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn basename_strips_only_a_real_suffix() {
        assert_eq!(basename_with_ext("/a/b/0801c0c8.c", ".c"), "0801c0c8");
        assert_eq!(basename_with_ext("/a/b/.c", ".c"), ".c", "not the whole name");
        assert_eq!(basename_with_ext("/a/b/x.h", ".c"), "x.h");
    }

    #[test]
    fn joining_collapses_separators() {
        assert_eq!(join("/root/", "out/claimed"), "/root/out/claimed");
        assert_eq!(join("/root", "/abs"), "/root/abs");
    }

    #[test]
    fn resolve_honours_an_absolute_second_argument() {
        assert_eq!(resolve("/root", "out"), "/root/out");
        assert_eq!(resolve("/root", "/elsewhere/out"), "/elsewhere/out");
    }

    #[test]
    fn relative_inside_the_root_has_no_dot_prefix() {
        assert_eq!(relative("/root", "/root/out/obj/x.o"), "out/obj/x.o");
        assert_eq!(relative("/root", "/root"), "");
        assert_eq!(relative("/root/a", "/root/b/c"), "../b/c");
    }

    #[test]
    fn dirname_walks_up() {
        assert_eq!(dirname("/a/b/c.ts"), "/a/b");
        assert_eq!(dirname("/a"), "/");
        assert_eq!(dirname("a"), ".");
    }
}
