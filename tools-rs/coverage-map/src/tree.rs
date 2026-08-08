// Read-only views of tracked files, backed by the work tree or by a git ref.
//
// WHY: the map is derived from tracked evidence, and the evidence may be read
// from a published ref rather than from whatever happens to be sitting in the
// working directory. Both backends have to behave identically -- in particular
// a directory listing has to include subdirectories, because `main_boundaries`
// walks `asm/` recursively and a files-only listing would silently stop at the
// first level and drop the region boundaries that size the whole main image.

use std::collections::{BTreeMap, BTreeSet};
use std::path::{Path, PathBuf};
use std::process::Command;

use crate::jsnum::utf16_cmp;

pub const ROM_BASE: i64 = 0x0800_0000;

/// `dirname(dirname(dirname(...)))` of the TypeScript's own path.
///
/// PORT NOTE: the TypeScript derives ROOT from `import.meta.url`, the source
/// file. A release binary lives in `target/`, so the equivalent anchor is the
/// crate manifest directory baked in at compile time.
pub fn root() -> PathBuf {
    let manifest = Path::new(env!("CARGO_MANIFEST_DIR"));
    manifest.parent().and_then(Path::parent).expect("crate lives two levels below ROOT").to_path_buf()
}

pub struct GitOutput {
    pub text: String,
    pub ok: bool,
}

/// `Bun.spawnSync(["git", ...args], { cwd: ROOT })`, stdout decoded as UTF-8.
///
/// PORT NOTE: `Buffer#toString()` with no encoding is UTF-8 with U+FFFD
/// substitution for invalid sequences, which is `String::from_utf8_lossy`,
/// not `String::from_utf8` (that would error where the TypeScript would not).
pub fn git(args: &[&str]) -> GitOutput {
    match Command::new("git").args(args).current_dir(root()).output() {
        Ok(result) => GitOutput {
            text: String::from_utf8_lossy(&result.stdout).into_owned(),
            ok: result.status.success(),
        },
        // A missing `git` binary makes `Bun.spawnSync` throw; here it is
        // surfaced as a non-ok result, which every caller already handles by
        // reporting the ref as unresolvable.
        Err(_) => GitOutput { text: String::new(), ok: false },
    }
}

/// `Clone` exists because `resolveSemanticTree` can hand back the very tree it
/// was given (a worktree that describes its own semantic sources); JavaScript
/// hands back the same object reference, and cloning is the closest faithful
/// equivalent that keeps the two trees independently owned.
#[derive(Clone, Debug)]
pub enum SourceTree {
    Work { id: String, root: PathBuf },
    Ref { id: String, entries: BTreeMap<String, BTreeSet<String>> },
    /// PORT NOTE: the TypeScript `SourceTree` is an interface, so `selfTest`
    /// builds one out of two closures. Rust's is an enum, so the synthetic form
    /// is a third variant rather than a trait object. `listings` and `files`
    /// are ordered vectors, not maps: the TypeScript closure returns the array
    /// it was handed, in order, and a directory listing feeds output ordering.
    Mem {
        id: String,
        listings: Vec<(String, Vec<String>)>,
        files: Vec<(String, String)>,
    },
}

impl SourceTree {
    pub fn id(&self) -> &str {
        match self {
            SourceTree::Work { id, .. } => id,
            SourceTree::Ref { id, .. } => id,
            SourceTree::Mem { id, .. } => id,
        }
    }

    pub fn list(&self, directory: &str) -> Vec<String> {
        match self {
            SourceTree::Work { root, .. } => {
                let path = root.join(directory);
                let Ok(entries) = std::fs::read_dir(&path) else {
                    return Vec::new();
                };
                let mut names: Vec<String> = entries
                    .filter_map(|entry| entry.ok())
                    .map(|entry| entry.file_name().to_string_lossy().into_owned())
                    .collect();
                names.sort_by(|left, right| utf16_cmp(left, right));
                names
            }
            SourceTree::Ref { entries, .. } => {
                let key = directory.strip_suffix('/').unwrap_or(directory);
                let mut names: Vec<String> =
                    entries.get(key).map(|set| set.iter().cloned().collect()).unwrap_or_default();
                names.sort_by(|left, right| utf16_cmp(left, right));
                names
            }
            // PORT NOTE: no sort. The synthetic closure in `selfTest` returns
            // the literal array it closes over, and sorting it here would make
            // the port disagree with the oracle on listing order.
            SourceTree::Mem { listings, .. } => listings
                .iter()
                .find(|(name, _)| name == directory)
                .map(|(_, names)| names.clone())
                .unwrap_or_default(),
        }
    }

    pub fn read(&self, path: &str) -> Option<String> {
        match self {
            SourceTree::Work { root, .. } => {
                let full = root.join(path);
                if !full.exists() {
                    return None;
                }
                // PORT NOTE: `readFileSync(full, "utf8")` never throws on
                // invalid UTF-8; it substitutes U+FFFD. Lossy decoding matches.
                std::fs::read(&full).ok().map(|bytes| String::from_utf8_lossy(&bytes).into_owned())
            }
            SourceTree::Ref { id, .. } => {
                let blob = git(&["show", &format!("{id}:{path}")]);
                if blob.ok {
                    Some(blob.text)
                } else {
                    None
                }
            }
            SourceTree::Mem { files, .. } => files
                .iter()
                .find(|(name, _)| name == path)
                .map(|(_, text)| text.clone()),
        }
    }
}

/// A synthetic tree, as `selfTest` builds from two closures.
pub fn mem_tree(
    id: &str,
    listings: &[(&str, &[&str])],
    files: &[(&str, String)],
) -> SourceTree {
    SourceTree::Mem {
        id: id.to_string(),
        listings: listings
            .iter()
            .map(|(name, items)| {
                (name.to_string(), items.iter().map(|item| item.to_string()).collect())
            })
            .collect(),
        files: files.iter().map(|(name, text)| (name.to_string(), text.clone())).collect(),
    }
}

pub fn work_tree() -> SourceTree {
    SourceTree::Work { id: "worktree".to_string(), root: root() }
}

pub fn work_tree_at(root: PathBuf) -> SourceTree {
    SourceTree::Work { id: "worktree".to_string(), root }
}

pub fn ref_tree(reference: &str) -> Option<SourceTree> {
    let resolved = git(&["rev-parse", "--verify", "--quiet", &format!("{reference}^{{commit}}")]);
    if !resolved.ok {
        return None;
    }
    let listing = git(&["ls-tree", "-r", "--name-only", reference]);
    if !listing.ok {
        return None;
    }
    // `listing.text.split("\n").filter(Boolean)` -- JS `filter(Boolean)` drops
    // the empty string the trailing newline produces.
    let paths = listing.text.split('\n').filter(|line| !line.is_empty());
    let mut entries: BTreeMap<String, BTreeSet<String>> = BTreeMap::new();
    for path in paths {
        let segments: Vec<&str> = path.split('/').collect();
        for index in 0..segments.len() {
            let parent = segments[..index].join("/");
            entries.entry(parent).or_default().insert(segments[index].to_string());
        }
    }
    Some(SourceTree::Ref { id: reference.to_string(), entries })
}

/// `readJson`: a missing path is an error naming the tree, never a silent skip.
pub fn read_json(tree: &SourceTree, path: &str) -> Result<crate::json::Value, String> {
    let text = tree.read(path).ok_or_else(|| format!("{} is missing {}", tree.id(), path))?;
    crate::json::parse(&text)
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn work_tree_lists_and_reads_a_real_tracked_directory() {
        let tree = work_tree();
        assert_eq!(tree.id(), "worktree");
        let listing = tree.list("tools/metrics");
        assert!(listing.contains(&"coverage_map.ts".to_string()));
        let sorted = {
            let mut copy = listing.clone();
            copy.sort_by(|left, right| utf16_cmp(left, right));
            copy
        };
        assert_eq!(listing, sorted, "listings are sorted like readdirSync(...).sort()");
        assert!(tree.read("tools/metrics/coverage_map.ts").is_some());
        assert!(tree.read("tools/metrics/does-not-exist.ts").is_none());
        assert!(tree.list("tools/metrics/does-not-exist").is_empty());
    }

    #[test]
    fn ref_tree_lists_subdirectories_not_only_files() {
        let Some(tree) = ref_tree("HEAD") else {
            panic!("HEAD must resolve inside the repository");
        };
        assert_eq!(tree.id(), "HEAD");
        let asm = tree.list("asm");
        assert!(!asm.is_empty());
        assert!(
            asm.iter().any(|name| !name.contains('.')),
            "asm/ must list subdirectories or the boundary walk stops at depth one",
        );
        // A trailing slash is stripped before lookup.
        assert_eq!(tree.list("asm/"), asm);
        assert!(tree.read("tools/metrics/coverage_map.ts").is_some());
        assert!(tree.read("tools/metrics/does-not-exist.ts").is_none());
    }

    #[test]
    fn ref_tree_rejects_an_unresolvable_ref() {
        assert!(ref_tree("definitely-not-a-ref-cec2a1f").is_none());
    }

    #[test]
    fn read_json_names_the_tree_in_its_error() {
        let tree = work_tree();
        let error = read_json(&tree, "no/such/file.json").unwrap_err();
        assert_eq!(error, "worktree is missing no/such/file.json");
    }
}
