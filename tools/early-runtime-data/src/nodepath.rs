// Node's `path` module is not `std::path`.
//
// The TypeScript this crate ports leans on four exact behaviours that
// `std::path` does not share: `resolve` normalises `..` LEXICALLY (it never
// touches the filesystem), `dirname("/a")` is `"/"` rather than `None`,
// `relative` is computed on two already-resolved lexical paths, and `join`
// normalises its result. `Path::parent`/`Path::join` differ on every one of
// those, and the differences are load-bearing here: `child()` compares
// `relative(root, path)` against a literal string, and `export_*` decides
// whether a requested directory is the filesystem root by asking whether it
// equals its own parent.
//
// POSIX only, which is all the tool ever ran on.

/// Lexically collapse `.` and `..` in an absolute path.
fn normalize_absolute(path: &str) -> String {
    let mut parts: Vec<&str> = Vec::new();
    for piece in path.split('/') {
        match piece {
            "" | "." => {}
            ".." => {
                parts.pop();
            }
            other => parts.push(other),
        }
    }
    if parts.is_empty() {
        "/".to_string()
    } else {
        format!("/{}", parts.join("/"))
    }
}

pub fn is_absolute(path: &str) -> bool {
    path.starts_with('/')
}

/// `path.resolve(...)` -- walk the segments right to left until one is
/// absolute, then prepend the working directory if none was.
pub fn resolve_from(cwd: &str, segments: &[&str]) -> String {
    let mut collected: Vec<String> = Vec::new();
    let mut absolute = false;
    for segment in segments.iter().rev() {
        if segment.is_empty() {
            continue;
        }
        collected.push((*segment).to_string());
        if is_absolute(segment) {
            absolute = true;
            break;
        }
    }
    if !absolute {
        collected.push(cwd.to_string());
    }
    collected.reverse();
    normalize_absolute(&collected.join("/"))
}

pub fn resolve(segments: &[&str]) -> String {
    let cwd = std::env::current_dir()
        .map(|path| path.to_string_lossy().into_owned())
        .unwrap_or_else(|_| "/".to_string());
    resolve_from(&cwd, segments)
}

/// `path.join(...)` -- concatenate, then normalise. Unlike `resolve` this keeps
/// a relative result relative.
pub fn join(segments: &[&str]) -> String {
    let joined: Vec<&str> = segments.iter().copied().filter(|s| !s.is_empty()).collect();
    if joined.is_empty() {
        return ".".to_string();
    }
    let combined = joined.join("/");
    if is_absolute(&combined) {
        return normalize_absolute(&combined);
    }
    // Relative normalisation keeps leading `..` segments, which the absolute
    // form discards.
    let mut parts: Vec<&str> = Vec::new();
    let mut leading = 0usize;
    for piece in combined.split('/') {
        match piece {
            "" | "." => {}
            ".." => {
                if parts.is_empty() {
                    leading += 1;
                } else {
                    parts.pop();
                }
            }
            other => parts.push(other),
        }
    }
    let mut out: Vec<String> = std::iter::repeat_n("..".to_string(), leading).collect();
    out.extend(parts.iter().map(|piece| (*piece).to_string()));
    if out.is_empty() {
        ".".to_string()
    } else {
        out.join("/")
    }
}

/// `path.basename` without the extension-stripping second argument.
pub fn basename(path: &str) -> String {
    let trimmed = path.trim_end_matches('/');
    if trimmed.is_empty() {
        return if path.starts_with('/') {
            String::new()
        } else {
            path.to_string()
        };
    }
    match trimmed.rfind('/') {
        Some(index) => trimmed[index + 1..].to_string(),
        None => trimmed.to_string(),
    }
}

/// `path.dirname`. `"/a"` yields `"/"`; `"/"` yields `"/"`; `"a"` yields `"."`.
pub fn dirname(path: &str) -> String {
    if path.is_empty() {
        return ".".to_string();
    }
    let absolute = path.starts_with('/');
    let trimmed = path.trim_end_matches('/');
    if trimmed.is_empty() {
        return "/".to_string();
    }
    match trimmed.rfind('/') {
        None => {
            if absolute {
                "/".to_string()
            } else {
                ".".to_string()
            }
        }
        Some(0) => "/".to_string(),
        Some(index) => trimmed[..index].to_string(),
    }
}

/// `path.relative(from, to)` over two paths resolved against `cwd`.
pub fn relative_from(cwd: &str, from: &str, to: &str) -> String {
    let from = resolve_from(cwd, &[from]);
    let to = resolve_from(cwd, &[to]);
    if from == to {
        return String::new();
    }
    let from_parts: Vec<&str> = from.split('/').filter(|s| !s.is_empty()).collect();
    let to_parts: Vec<&str> = to.split('/').filter(|s| !s.is_empty()).collect();
    let mut shared = 0usize;
    while shared < from_parts.len()
        && shared < to_parts.len()
        && from_parts[shared] == to_parts[shared]
    {
        shared += 1;
    }
    let mut out: Vec<String> =
        std::iter::repeat_n("..".to_string(), from_parts.len() - shared).collect();
    out.extend(to_parts[shared..].iter().map(|piece| (*piece).to_string()));
    out.join("/")
}

pub fn relative(from: &str, to: &str) -> String {
    let cwd = std::env::current_dir()
        .map(|path| path.to_string_lossy().into_owned())
        .unwrap_or_else(|_| "/".to_string());
    relative_from(&cwd, from, to)
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn dirname_matches_node_at_the_root() {
        assert_eq!(dirname("/a/b"), "/a");
        assert_eq!(dirname("/a"), "/");
        assert_eq!(dirname("/"), "/");
        assert_eq!(dirname("a"), ".");
    }

    #[test]
    fn basename_strips_trailing_slashes() {
        assert_eq!(basename("/a/b/"), "b");
        assert_eq!(basename("/a/index.json"), "index.json");
        assert_eq!(basename("/"), "");
    }

    #[test]
    fn resolve_is_lexical_and_absolute() {
        assert_eq!(resolve_from("/work", &["out/x"]), "/work/out/x");
        assert_eq!(resolve_from("/work", &["/a/b/../c"]), "/a/c");
        assert_eq!(resolve_from("/work", &["/a", "b", "/c"]), "/c");
    }

    #[test]
    fn relative_yields_empty_for_identity() {
        assert_eq!(relative_from("/w", "/a/b", "/a/b"), "");
        assert_eq!(relative_from("/w", "/a", "/a/b/c"), "b/c");
        assert_eq!(relative_from("/w", "/a/b", "/a/c"), "../c");
    }

    #[test]
    fn join_normalises_and_keeps_relative_paths_relative() {
        assert_eq!(join(&["/a", "b", "index.json"]), "/a/b/index.json");
        assert_eq!(join(&["a", "..", "..", "b"]), "../b");
    }
}
