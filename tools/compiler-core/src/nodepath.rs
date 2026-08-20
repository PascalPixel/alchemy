//! `node:path` POSIX `basename` and `extname`, reproduced exactly.
//!
//! WHY A WHOLE MODULE FOR TWO FUNCTIONS. The plan layer derives two strings
//! from caller-supplied paths -- the `-dumpbase` argument and the intermediate
//! `.i` filename -- and both feed a compiler command line. `Path::file_stem`
//! and `Path::extension` are *not* the same functions: Rust's `extension`
//! returns `None` for `"a.c/"` where Node returns `".c"`, returns `None` for a
//! trailing dot where Node returns `"."`, and `file_name` strips a trailing
//! slash silently. Those differences change the emitted argv, so the Node
//! algorithms are transcribed here rather than approximated.
//!
//! PORT NOTE: Node scans UTF-16 code units and slices by them. This scans UTF-8
//! bytes. The two agree because the only characters compared are `/` (0x2F) and
//! `.` (0x2E), which cannot occur inside a multi-byte UTF-8 sequence and cannot
//! occur as a surrogate half -- so every index found here is a code-point
//! boundary and the resulting substrings are byte-identical.
//!
//! `compiler_core::routing::source_stem` carries a *simplified* extname (last
//! dot at index > 0). That simplification is correct for the routing keys it
//! serves and is deliberately not reused here: the plan layer receives output
//! paths from callers rather than repository-relative source names, so the edge
//! cases the simplification folds away are reachable.

/// `path.basename(path)` for POSIX, with no `ext` argument.
pub fn basename(path: &str) -> &str {
    let bytes = path.as_bytes();
    let mut start = 0usize;
    let mut end: Option<usize> = None;
    let mut matched_slash = true;
    for index in (0..bytes.len()).rev() {
        if bytes[index] == b'/' {
            // Trailing slashes are skipped; the first slash after real
            // characters ends the scan.
            if !matched_slash {
                start = index + 1;
                break;
            }
        } else if end.is_none() {
            matched_slash = false;
            end = Some(index + 1);
        }
    }
    match end {
        // All slashes, or empty: Node returns the empty string.
        None => "",
        Some(end) => &path[start..end],
    }
}

/// `path.extname(path)` for POSIX.
///
/// The four rejection conditions are Node's, transcribed verbatim rather than
/// simplified. `pre_dot_state` is the tri-state Node uses to reject a basename
/// that is nothing but dots: `""` for `"."`, `".."`, and `"/.."`, but `"."` for
/// `"a.."` and `".c"` for `"a.c"`.
pub fn extname(path: &str) -> &str {
    let bytes = path.as_bytes();
    let mut start_dot: Option<usize> = None;
    let mut start_part = 0usize;
    let mut end: Option<usize> = None;
    let mut matched_slash = true;
    // 0 = nothing seen yet, 1 = only dots seen, -1 = a non-dot was seen.
    let mut pre_dot_state = 0i8;
    for index in (0..bytes.len()).rev() {
        let code = bytes[index];
        if code == b'/' {
            if !matched_slash {
                start_part = index + 1;
                break;
            }
            continue;
        }
        if end.is_none() {
            matched_slash = false;
            end = Some(index + 1);
        }
        if code == b'.' {
            if start_dot.is_none() {
                start_dot = Some(index);
            } else if pre_dot_state != 1 {
                pre_dot_state = 1;
            }
        } else if start_dot.is_some() {
            pre_dot_state = -1;
        }
    }
    let (Some(start_dot), Some(end)) = (start_dot, end) else {
        return "";
    };
    if pre_dot_state == 0
        || (pre_dot_state == 1 && start_dot == end - 1 && start_dot == start_part + 1)
    {
        return "";
    }
    &path[start_dot..end]
}
