use std::path::{Path, PathBuf};
use std::process::Command;
pub fn strip_level_for_basename(header_path: &str, basename: &str) -> Option<usize> {
    let path = header_path
        .trim()
        .strip_prefix("a/")
        .or_else(|| header_path.trim().strip_prefix("b/"))
        .unwrap_or(header_path.trim());
    let parts: Vec<_> = path.split('/').filter(|part| !part.is_empty()).collect();
    (parts.last().copied() == Some(basename)).then_some(parts.len())
}
pub fn apply_unified_diff(source: &str, patch_text: &str, dest: &Path) -> Result<(), String> {
    let basename = dest
        .file_name()
        .and_then(|name| name.to_str())
        .ok_or_else(|| format!("{}: not a file path", dest.display()))?;
    let parent = dest.parent().ok_or_else(|| format!("{}: missing parent", dest.display()))?;
    std::fs::create_dir_all(parent).map_err(|error| format!("{}: {error}", parent.display()))?;
    std::fs::copy(source, dest).map_err(|error| format!("{source} -> {}: {error}", dest.display()))?;
    let header = patch_text
        .lines()
        .find_map(|line| line.strip_prefix("--- ").or_else(|| line.strip_prefix("+++ ")))
        .ok_or("patch: no ---/+++ header")?;
    let path = header.split_whitespace().next().unwrap_or(header);
    let strip = strip_level_for_basename(path, basename)
        .ok_or_else(|| format!("patch: header {path:?} does not end in {basename}"))?;
    let patch_path: PathBuf = parent.join("incoming.diff");
    std::fs::write(&patch_path, patch_text).map_err(|error| format!("{}: {error}", patch_path.display()))?;
    let output = Command::new("git")
        .args(["apply", "--unsafe-paths", "--whitespace=nowarn", &format!("-p{strip}"), "incoming.diff"])
        .current_dir(parent)
        .output()
        .map_err(|error| format!("git apply: {error}"))?;
    if output.status.success() {
        return Ok(());
    }
    let detail = String::from_utf8_lossy(if output.stderr.is_empty() { &output.stdout } else { &output.stderr });
    Err(format!("git apply failed: {}", detail.trim()))
}
