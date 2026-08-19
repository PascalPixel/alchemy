//! Apply a single-file unified diff to a copy of a candidate.
//!
//! The confirmation loop wants a delta, not a write to the working tree:
//! score the patched copy, leave the original file alone.

use std::path::{Path, PathBuf};
use std::process::Command;

/// How many path components `git apply -p` must strip so only `basename` remains.
pub fn strip_level_for_basename(header_path: &str, basename: &str) -> Option<usize> {
    let mut path = header_path.trim();
    if path == "/dev/null" {
        return None;
    }
    for prefix in ["a/", "b/"] {
        if let Some(rest) = path.strip_prefix(prefix) {
            path = rest;
            break;
        }
    }
    let parts: Vec<&str> = path.split('/').filter(|part| !part.is_empty()).collect();
    let last = *parts.last()?;
    if last != basename {
        return None;
    }
    Some(parts.len())
}

/// Copy `source` to `dest` and apply `patch_text` with `git apply`.
pub fn apply_unified_diff(source: &str, patch_text: &str, dest: &Path) -> Result<(), String> {
    let basename = dest
        .file_name()
        .and_then(|name| name.to_str())
        .ok_or_else(|| format!("{}: not a file path", dest.display()))?;
    let parent = dest
        .parent()
        .ok_or_else(|| format!("{}: missing parent", dest.display()))?;
    std::fs::create_dir_all(parent).map_err(|error| format!("{}: {error}", parent.display()))?;
    std::fs::copy(source, dest).map_err(|error| format!("{source} -> {}: {error}", dest.display()))?;

    let header = patch_text.lines().find_map(|line| {
        line.strip_prefix("--- ")
            .or_else(|| line.strip_prefix("+++ "))
    });
    let header = header.ok_or_else(|| "patch: no ---/+++ header".to_string())?;
    let header_path = header.split_whitespace().next().unwrap_or(header);
    let strip = strip_level_for_basename(header_path, basename).ok_or_else(|| {
        format!("patch: header {header_path:?} does not end in {basename}")
    })?;

    let patch_path: PathBuf = parent.join("incoming.diff");
    std::fs::write(&patch_path, patch_text)
        .map_err(|error| format!("{}: {error}", patch_path.display()))?;

    // `git apply` resolves the patch path against `current_dir`. A relative
    // `parent/incoming.diff` would be looked up as `parent/parent/incoming.diff`.
    let output = Command::new("git")
        .args([
            "apply",
            "--unsafe-paths",
            "--whitespace=nowarn",
            &format!("-p{strip}"),
            "incoming.diff",
        ])
        .current_dir(parent)
        .output()
        .map_err(|error| format!("git apply: {error}"))?;
    if output.status.success() {
        return Ok(());
    }
    let stderr = String::from_utf8_lossy(&output.stderr);
    let stdout = String::from_utf8_lossy(&output.stdout);
    let detail = if !stderr.trim().is_empty() {
        stderr.trim()
    } else {
        stdout.trim()
    };
    Err(format!("git apply failed: {detail}"))
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn strip_counts_a_worktree_header() {
        assert_eq!(
            strip_level_for_basename("a/work/080bbb0c.c", "080bbb0c.c"),
            Some(2)
        );
        assert_eq!(
            strip_level_for_basename("work/080bbb0c.c", "080bbb0c.c"),
            Some(2)
        );
        assert_eq!(strip_level_for_basename("080bbb0c.c", "080bbb0c.c"), Some(1));
        assert_eq!(strip_level_for_basename("a/other.c", "080bbb0c.c"), None);
    }

    #[test]
    fn apply_reads_the_patch_from_the_try_directory() {
        let root = std::env::temp_dir().join(format!(
            "candidate-show-patch-{}",
            std::process::id()
        ));
        let _ = std::fs::remove_dir_all(&root);
        std::fs::create_dir_all(&root).unwrap();
        let source = root.join("080bbb0c.c");
        std::fs::write(&source, "int x = 1;\n").unwrap();
        let dest = root.join("try").join("080bbb0c.c");
        apply_unified_diff(
            source.to_str().unwrap(),
            "--- a/work/080bbb0c.c\n+++ b/work/080bbb0c.c\n@@ -1 +1 @@\n-int x = 1;\n+int x = 2;\n",
            &dest,
        )
        .unwrap();
        assert_eq!(std::fs::read_to_string(&dest).unwrap(), "int x = 2;\n");
        let _ = std::fs::remove_dir_all(&root);
    }
}