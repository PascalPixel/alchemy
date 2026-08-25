use compiler_core::source_inputs::quoted_include;
use std::path::{Path, PathBuf};
use std::process::Command;

pub fn apply_unified_diff_in_tree(
    root: &Path,
    source: &str,
    patch: &str,
    try_root: &Path,
) -> Result<PathBuf, String> {
    let repository = root.canonicalize().map_err(text)?;
    let original = root.join(source).canonicalize().map_err(text)?;
    let dest = try_root.join(original.strip_prefix(&repository).map_err(text)?);
    apply_unified_diff(&original, patch, &dest)?;
    for include in std::fs::read_to_string(&dest)
        .map_err(text)?
        .lines()
        .filter_map(quoted_include)
    {
        let included = original.parent().unwrap_or(&repository).join(include);
        if !included.is_file() {
            continue;
        }
        let included = included.canonicalize().map_err(text)?;
        let staged = try_root.join(included.strip_prefix(&repository).map_err(text)?);
        std::fs::create_dir_all(staged.parent().unwrap_or(try_root)).map_err(text)?;
        std::fs::copy(included, staged).map_err(text)?;
    }
    Ok(dest)
}

fn apply_unified_diff(source: &Path, patch_text: &str, dest: &Path) -> Result<(), String> {
    let basename = dest
        .file_name()
        .ok_or_else(|| format!("{}: not a file path", dest.display()))?;
    let parent = dest
        .parent()
        .ok_or_else(|| format!("{}: missing parent", dest.display()))?;
    std::fs::create_dir_all(parent).map_err(text)?;
    std::fs::copy(source, dest).map_err(text)?;
    let path = patch_text
        .lines()
        .find_map(|line| line.strip_prefix("--- "))
        .and_then(|line| line.split_whitespace().next())
        .ok_or("patch: no --- header")?;
    if Path::new(path).file_name() != dest.file_name() {
        return Err(format!(
            "patch: header {path:?} does not end in {basename:?}"
        ));
    }
    let strip = path.trim_start_matches("a/").split('/').count();
    let patch_path = parent.join("incoming.diff");
    std::fs::write(&patch_path, patch_text).map_err(text)?;
    let output = Command::new("git")
        .args([
            "apply",
            "--unidiff-zero",
            "--unsafe-paths",
            "--whitespace=nowarn",
            &format!("-p{strip}"),
            "incoming.diff",
        ])
        .current_dir(parent)
        .output()
        .map_err(text)?;
    if output.status.success() {
        return Ok(());
    }
    let detail = String::from_utf8_lossy(if output.stderr.is_empty() {
        &output.stdout
    } else {
        &output.stderr
    });
    Err(format!("git apply failed: {}", detail.trim()))
}

fn text(error: impl ToString) -> String {
    error.to_string()
}
