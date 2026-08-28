//! Exact POSIX `node:path` basename/extname semantics for compiler argv.
//! Rust path methods differ on trailing slashes/dots. Byte scanning matches
//! Node's UTF-16 scan because only ASCII `/` and `.` are tested. Do not replace
//! this with routing's simplified source-stem logic: caller paths reach its edge cases.

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
/// `pre_dot_state` preserves Node's distinction between dot-only basenames and
/// extensions such as `a..` or `.c`.
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
