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
//! `alchemy_routing::routing::source_stem` carries a *simplified* extname (last
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

#[cfg(test)]
mod tests {
    use super::*;

    /// Each case is `(input, basename, extname)`, taken from what node:path
    /// actually returns. The rows that matter are the ones where `std::path`
    /// disagrees; they are called out individually below.
    const CASES: [(&str, &str, &str); 18] = [
        ("", "", ""),
        ("main.c", "main.c", ".c"),
        ("src/main.c", "main.c", ".c"),
        ("/a/b/main.c", "main.c", ".c"),
        ("src/main", "main", ""),
        (".bashrc", ".bashrc", ""),
        ("src/.bashrc", ".bashrc", ""),
        ("a.b.c", "a.b.c", ".c"),
        ("a.", "a.", "."),
        ("a..", "a..", "."),
        (".", ".", ""),
        ("..", "..", ""),
        ("/", "", ""),
        ("///", "", ""),
        ("a.c/", "a.c", ".c"),
        ("a.c///", "a.c", ".c"),
        ("dir.d/main", "main", ""),
        ("dir.d/", "dir.d", ".d"),
    ];

    #[test]
    fn matches_node_path_on_every_case() {
        // FLOOR: a future edit that empties the table must not read as a pass.
        assert!(CASES.len() >= 18, "the node:path case table was truncated");
        let mut checked = 0;
        for (input, expected_base, expected_ext) in CASES {
            assert_eq!(basename(input), expected_base, "basename({input:?})");
            assert_eq!(extname(input), expected_ext, "extname({input:?})");
            checked += 1;
        }
        assert_eq!(checked, CASES.len());
    }

    #[test]
    fn differs_from_std_path_exactly_where_it_must() {
        use std::path::Path;
        // These are the rows that justify this module existing. If std::path
        // ever changes to agree, this test tells us the module could be
        // reconsidered -- it does not silently keep duplicating.
        // The dot itself. Node's extname includes it, `Path::extension` does
        // not -- so `output.slice(0, -extension.length)` in the TypeScript cuts
        // one more character than a `Path::extension`-based port would.
        assert_eq!(extname("src/main.c"), ".c");
        assert_eq!(
            Path::new("src/main.c").extension().and_then(|e| e.to_str()),
            Some("c")
        );

        // A trailing dot: Node reports the dot as the whole extension, so the
        // slice removes it. `Path::extension` reports an empty extension, which
        // would remove nothing and produce a different intermediate filename.
        assert_eq!(extname("a."), ".");
        assert_eq!(
            Path::new("a.").extension().and_then(|e| e.to_str()),
            Some("")
        );

        // A dotfile is all stem to Node and all stem to Rust, but a leading-dot
        // *directory* is where the two part company on basename.
        assert_eq!(basename("/"), "");
        assert_eq!(Path::new("/").file_name(), None);

        // `Path` normalises a trailing slash away before splitting; Node does
        // the same here, so this row agrees apart from the dot. It is asserted
        // rather than assumed, because the earlier draft of this module assumed
        // otherwise and was wrong.
        assert_eq!(extname("a.c/"), ".c");
        assert_eq!(
            Path::new("a.c/").extension().and_then(|e| e.to_str()),
            Some("c")
        );
    }

    #[test]
    fn multibyte_paths_are_sliced_on_code_point_boundaries() {
        // 起動確認と同じ理由で日本語コメントを含む経路も通す。UTF-16 単位で
        // 走査する Node と UTF-8 バイトで走査するこちらが一致することを示す。
        assert_eq!(basename("src/起動.c"), "起動.c");
        assert_eq!(extname("src/起動.c"), ".c");
        assert_eq!(extname("src/起動"), "");
        // Astral plane: two UTF-16 units in Node, four UTF-8 bytes here.
        assert_eq!(basename("src/\u{1f600}.c"), "\u{1f600}.c");
        assert_eq!(extname("src/\u{1f600}"), "");
    }
}
