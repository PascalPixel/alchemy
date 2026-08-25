// Housekeeping for files a generator owns: removing the ones it no longer
// produces, and finding tracked images no region claims as a source.
//
// Ported from tools/lib/generated_files.ts. `prune_files` deletes, so the
// signature makes that impossible to miss: it takes the directory, the glob
// its generator owns, and the exact set of names to keep, and it never
// descends. `unused_tracked_images` shells out to `git ls-files` exactly as
// the TypeScript did, and still returns nothing outside a git checkout rather
// than treating an untracked tree as one with no sources.

use std::collections::BTreeSet;
use std::fs;
use std::io;
use std::path::{Path, PathBuf};
use std::process::Command;

pub const IMAGE_SUFFIXES: [&str; 6] = [".png", ".jpg", ".jpeg", ".gif", ".bmp", ".webp"];

/// Matches the TypeScript's glob: `*` spans any run, `?` one character, and
/// everything else is literal. Anchored at both ends.
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

/// Delete every file in `directory` matching `pattern` whose name is not in
/// `keep`. Returns the paths removed, in sorted order. A missing directory is
/// not an error: there is nothing to prune.
pub fn prune_files(
    directory: &Path,
    pattern: &str,
    keep: impl IntoIterator<Item = impl AsRef<Path>>,
) -> io::Result<Vec<PathBuf>> {
    let expected: BTreeSet<String> = keep
        .into_iter()
        .filter_map(|name| name.as_ref().file_name().map(|name| name.to_string_lossy().into_owned()))
        .collect();
    if !directory.exists() {
        return Ok(Vec::new());
    }
    let mut names: Vec<String> = fs::read_dir(directory)?
        .filter_map(Result::ok)
        .map(|entry| entry.file_name().to_string_lossy().into_owned())
        .collect();
    names.sort();

    let mut removed = Vec::new();
    for name in names {
        let path = directory.join(&name);
        if glob_matches(pattern, &name) && path.is_file() && !expected.contains(&name) {
            fs::remove_file(&path)?;
            removed.push(path);
        }
    }
    Ok(removed)
}

fn is_image(name: &str) -> bool {
    let lowered = name.to_ascii_lowercase();
    IMAGE_SUFFIXES.iter().any(|suffix| lowered.ends_with(suffix))
}

fn normalize(name: &str) -> String {
    name.replace('\\', "/")
}

/// Tracked image files that no region lists as a source. `sources` is every
/// source path named by every region; `ignored_prefixes` excludes whole
/// subtrees. Returns nothing when `root` is not a git checkout.
pub fn unused_tracked_images(
    root: &Path,
    sources: impl IntoIterator<Item = impl AsRef<str>>,
    ignored_prefixes: impl IntoIterator<Item = impl AsRef<str>>,
) -> io::Result<Vec<String>> {
    if !root.join(".git").exists() {
        return Ok(Vec::new());
    }
    let mut command = Command::new("git");
    command.arg("ls-files").arg("-z").arg("--").current_dir(root);
    for suffix in IMAGE_SUFFIXES {
        command.arg(format!("*{suffix}"));
    }
    let output = command.output()?;
    if !output.status.success() {
        return Err(io::Error::other(String::from_utf8_lossy(&output.stderr).into_owned()));
    }
    let tracked: BTreeSet<String> =
        String::from_utf8_lossy(&output.stdout).split('\0').filter(|name| !name.is_empty()).map(normalize).collect();

    let claimed: BTreeSet<String> = sources
        .into_iter()
        .filter_map(|name| {
            let name = name.as_ref();
            if !is_image(name) {
                return None;
            }
            let path = Path::new(name);
            let relative = if path.is_absolute() {
                path.strip_prefix(root).ok()?.to_string_lossy().into_owned()
            } else {
                name.to_string()
            };
            Some(normalize(&relative))
        })
        .collect();

    let ignored: Vec<String> = ignored_prefixes.into_iter().map(|prefix| normalize(prefix.as_ref())).collect();

    Ok(tracked
        .into_iter()
        .filter(|name| {
            !claimed.contains(name)
                && !ignored.iter().any(|prefix| name.starts_with(prefix))
                && root.join(name).exists()
        })
        .collect())
}
