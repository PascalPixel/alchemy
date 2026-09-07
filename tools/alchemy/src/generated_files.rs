use std::collections::BTreeSet;
use std::fs;
use std::io;
use std::path::{Path, PathBuf};
use std::process::Command;

pub const IMAGE_SUFFIXES: [&str; 6] = [".png", ".jpg", ".jpeg", ".gif", ".bmp", ".webp"];

/// Match a filename against an anchored `*`/`?` glob.
pub fn glob_matches(pattern: &str, name: &str) -> bool {
    fn walk(pattern: &[char], name: &[char]) -> bool {
        match pattern.first() {
            None => name.is_empty(),
            Some('*') => (0..=name.len()).any(|split| walk(&pattern[1..], &name[split..])),
            Some('?') => !name.is_empty() && walk(&pattern[1..], &name[1..]),
            Some(literal) => name.first() == Some(literal) && walk(&pattern[1..], &name[1..]),
        }
    }
    let pattern: Vec<char> = pattern.chars().collect();
    let name: Vec<char> = name.chars().collect();
    walk(&pattern, &name)
}

/// Delete unclaimed matching files directly inside `directory`.
pub fn prune_files(
    directory: &Path,
    pattern: &str,
    keep: impl IntoIterator<Item = impl AsRef<Path>>,
) -> io::Result<Vec<PathBuf>> {
    let keep: BTreeSet<_> = keep
        .into_iter()
        .filter_map(|path| path.as_ref().file_name().map(ToOwned::to_owned))
        .collect();
    if !directory.exists() {
        return Ok(Vec::new());
    }
    let mut removed = Vec::new();
    for entry in fs::read_dir(directory)?.filter_map(Result::ok) {
        let path = entry.path();
        if path.is_file()
            && path.file_name().is_some_and(|name| {
                glob_matches(pattern, &name.to_string_lossy()) && !keep.contains(name)
            })
        {
            removed.push(path);
        }
    }
    removed.sort();
    removed.iter().try_for_each(fs::remove_file)?;
    Ok(removed)
}

/// Find existing tracked images that are neither claimed nor ignored.
pub fn unused_tracked_images(
    root: &Path,
    sources: impl IntoIterator<Item = impl AsRef<str>>,
    ignored_prefixes: impl IntoIterator<Item = impl AsRef<str>>,
) -> io::Result<Vec<String>> {
    if !root.join(".git").exists() {
        return Ok(Vec::new());
    }
    let output = Command::new("git")
        .args(["ls-files", "-z", "--"])
        .args(IMAGE_SUFFIXES.map(|suffix| format!("*{suffix}")))
        .current_dir(root)
        .output()?;
    if !output.status.success() {
        return Err(io::Error::other(String::from_utf8_lossy(&output.stderr)));
    }
    let mut claimed = BTreeSet::new();
    for source in sources {
        let source = source.as_ref();
        let path = Path::new(source);
        let relative = if path.is_absolute() {
            let Ok(path) = path.strip_prefix(root) else {
                continue;
            };
            path.to_string_lossy().into_owned()
        } else {
            source.to_owned()
        };
        claimed.insert(relative.replace('\\', "/"));
    }
    let ignored: Vec<_> = ignored_prefixes
        .into_iter()
        .map(|prefix| prefix.as_ref().replace('\\', "/"))
        .collect();

    Ok(String::from_utf8_lossy(&output.stdout)
        .split('\0')
        .filter(|name| {
            !name.is_empty()
                && !claimed.contains(*name)
                && !ignored.iter().any(|prefix| name.starts_with(prefix))
                && root.join(name).exists()
        })
        .map(str::to_owned)
        .collect())
}
