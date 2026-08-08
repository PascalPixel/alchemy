//! Node's `node:path` (POSIX flavour), reproduced exactly.
//!
//! WHY THIS FILE EXISTS: `tools/lib/tile_objects.ts` resolves every file it
//! touches through `resolve`/`relative`/`dirname`/`basename`, and one of those
//! results is a *security* check — `child()` refuses a plan whose sibling
//! escapes the plan's own directory by inspecting the string `relative()`
//! returns. Rust's `std::path` deliberately does *not* normalise `..` (it
//! cannot, without touching the filesystem), so `Path::join` plus
//! `strip_prefix` would answer that question differently from Node and could
//! turn a rejected path into an accepted one. The lexical, filesystem-free
//! normalisation Node performs is therefore transcribed here rather than
//! approximated.
//!
//! Only the POSIX branch exists: the TypeScript's single `process.platform`
//! test is carried across in `lib.rs` as a `PORT NOTE`, and this repo's tracked
//! assets are darwin/linux only.

use std::path::PathBuf;

/// `path.isAbsolute` (POSIX).
pub fn is_absolute(path: &str) -> bool {
    path.starts_with('/')
}

/// The lexical half of `path.resolve`: fold `.` and `..` away without asking
/// the filesystem anything.
///
/// PORT NOTE: for an absolute path Node drops leading `..` components that
/// would climb above the root (`/a/../../b` is `/b`); for a relative one it
/// keeps them (`../../b` stays `../../b`). Both behaviours are load-bearing for
/// the escape check in `child()`.
fn normalize(path: &str, absolute: bool) -> String {
    let mut parts: Vec<&str> = Vec::new();
    for segment in path.split('/') {
        match segment {
            "" | "." => {}
            ".." => {
                if let Some(last) = parts.last() {
                    if *last != ".." {
                        parts.pop();
                        continue;
                    }
                }
                if !absolute {
                    parts.push("..");
                }
            }
            other => parts.push(other),
        }
    }
    let joined = parts.join("/");
    if absolute {
        format!("/{joined}")
    } else {
        joined
    }
}

/// `path.resolve(...segments)` (POSIX), with `cwd` standing in for
/// `process.cwd()`.
///
/// Node walks the segments right to left, prepending each until the
/// accumulated path is absolute, then prepends the cwd if it never became one.
pub fn resolve_from(cwd: &str, segments: &[&str]) -> String {
    let mut resolved = String::new();
    let mut absolute = false;
    for segment in segments.iter().rev() {
        if segment.is_empty() {
            continue;
        }
        resolved = if resolved.is_empty() {
            (*segment).to_string()
        } else {
            format!("{segment}/{resolved}")
        };
        if is_absolute(segment) {
            absolute = true;
            break;
        }
    }
    if !absolute {
        resolved = if resolved.is_empty() {
            cwd.to_string()
        } else {
            format!("{cwd}/{resolved}")
        };
    }
    let normalized = normalize(&resolved, true);
    if normalized.len() > 1 {
        normalized.trim_end_matches('/').to_string()
    } else {
        normalized
    }
}

/// `path.resolve` against the real process working directory.
pub fn resolve(segments: &[&str]) -> String {
    let cwd = std::env::current_dir().unwrap_or_else(|_| PathBuf::from("/"));
    resolve_from(&cwd.to_string_lossy(), segments)
}

/// `path.dirname` (POSIX).
pub fn dirname(path: &str) -> String {
    if path.is_empty() {
        return ".".to_string();
    }
    let bytes = path.as_bytes();
    let root = bytes[0] == b'/';
    // Node ignores trailing slashes except the root's own.
    let mut end = path.len();
    while end > 1 && bytes[end - 1] == b'/' {
        end -= 1;
    }
    let trimmed = &path[..end];
    match trimmed.rfind('/') {
        None => ".".to_string(),
        Some(0) => "/".to_string(),
        Some(index) => {
            let head = &trimmed[..index];
            if head.is_empty() {
                if root {
                    "/".to_string()
                } else {
                    ".".to_string()
                }
            } else {
                head.to_string()
            }
        }
    }
}

/// `path.basename` (POSIX), without the optional suffix argument the
/// TypeScript never passes.
pub fn basename(path: &str) -> String {
    let mut end = path.len();
    let bytes = path.as_bytes();
    while end > 0 && bytes[end - 1] == b'/' {
        end -= 1;
    }
    let trimmed = &path[..end];
    match trimmed.rfind('/') {
        None => trimmed.to_string(),
        Some(index) => trimmed[index + 1..].to_string(),
    }
}

/// `path.join(...segments)` (POSIX): concatenate with `/`, then normalise,
/// *without* consulting the cwd. A relative result stays relative — which is
/// why `join(ROOT, resourceGraphicsDir(name))` in the TypeScript produces the
/// string it does.
pub fn join(segments: &[&str]) -> String {
    let mut joined = String::new();
    for segment in segments {
        if segment.is_empty() {
            continue;
        }
        if joined.is_empty() {
            joined = (*segment).to_string();
        } else {
            joined.push('/');
            joined.push_str(segment);
        }
    }
    if joined.is_empty() {
        return ".".to_string();
    }
    let absolute = is_absolute(&joined);
    let normalized = normalize(&joined, absolute);
    if normalized.is_empty() {
        ".".to_string()
    } else {
        normalized
    }
}

/// `path.relative(from, to)` (POSIX), both arguments resolved first.
pub fn relative_from(cwd: &str, from: &str, to: &str) -> String {
    let from = resolve_from(cwd, &[from]);
    let to = resolve_from(cwd, &[to]);
    if from == to {
        return String::new();
    }
    let from_parts: Vec<&str> = from.split('/').filter(|part| !part.is_empty()).collect();
    let to_parts: Vec<&str> = to.split('/').filter(|part| !part.is_empty()).collect();
    let mut shared = 0usize;
    while shared < from_parts.len() && shared < to_parts.len() && from_parts[shared] == to_parts[shared] {
        shared += 1;
    }
    let mut out: Vec<&str> = Vec::new();
    for _ in shared..from_parts.len() {
        out.push("..");
    }
    out.extend_from_slice(&to_parts[shared..]);
    out.join("/")
}

/// `path.relative` against the real process working directory.
pub fn relative(from: &str, to: &str) -> String {
    let cwd = std::env::current_dir().unwrap_or_else(|_| PathBuf::from("/"));
    relative_from(&cwd.to_string_lossy(), from, to)
}

#[cfg(test)]
mod tests {
    use super::*;

    /// Every assertion below was checked against Bun's `node:path` before being
    /// written down; the counter makes a future edit that silently drops cases
    /// visible.
    #[test]
    fn matches_node_path_posix() {
        let mut checks = 0usize;
        let mut check = |left: String, right: &str| {
            assert_eq!(left, right);
            checks += 1;
        };
        check(resolve_from("/w", &["/a/b", "../c"]), "/a/c");
        check(resolve_from("/w", &["a/b"]), "/w/a/b");
        check(resolve_from("/w", &["/a/./b//c/"]), "/a/b/c");
        check(resolve_from("/w", &["/a/../../b"]), "/b");
        check(resolve_from("/w", &[]), "/w");
        check(resolve_from("/w", &["/"]), "/");
        check(dirname("/a/b/c.json"), "/a/b");
        check(dirname("/a"), "/");
        check(dirname("a"), ".");
        check(dirname("/"), "/");
        check(basename("/a/b/c.json"), "c.json");
        check(basename("/a/b/"), "b");
        check(basename("c.json"), "c.json");
        check(join(&["assets/graphics/map_resource_152", "charblock2.4bpp.png"]),
              "assets/graphics/map_resource_152/charblock2.4bpp.png");
        check(join(&["/a", "b", "../c"]), "/a/c");
        check(relative_from("/w", "/a/b", "/a/b/c.png"), "c.png");
        check(relative_from("/w", "/a/b", "/a/b"), "");
        check(relative_from("/w", "/a/b", "/a/c"), "../c");
        check(relative_from("/w", "/a/b", "/x"), "../../x");
        check(relative_from("/w", "/a/b", "/a/b/objects/x.png"), "objects/x.png");
        assert!(is_absolute("/a"));
        assert!(!is_absolute("a"));
        assert_eq!(checks, 20, "a path case was dropped");
    }
}
