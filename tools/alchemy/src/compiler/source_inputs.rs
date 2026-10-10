use sha2::{Digest, Sha256};
use std::collections::{BTreeMap, BTreeSet};
use std::path::{Path, PathBuf};
use std::sync::{Arc, RwLock};

struct SourceInput {
    bytes: Vec<u8>,
    includes: Vec<String>,
}

/// Share reads only within one audit of unchanged inputs. Include resolution,
/// traversal state and hash order remain independent for every signature.
#[derive(Default)]
pub struct SourceInputCache {
    files: RwLock<BTreeMap<PathBuf, Arc<SourceInput>>>,
}

impl SourceInputCache {
    fn read(&self, path: &Path) -> Result<Arc<SourceInput>, String> {
        if let Some(input) = self.files.read().map_err(|_| "source input cache poisoned")?.get(path) {
            return Ok(Arc::clone(input));
        }
        let mut files = self.files.write().map_err(|_| "source input cache poisoned")?;
        if let Some(input) = files.get(path) {
            return Ok(Arc::clone(input));
        }
        let bytes = std::fs::read(path).map_err(|error| error.to_string())?;
        let includes = String::from_utf8_lossy(&bytes).lines()
            .filter_map(quoted_include).map(str::to_owned).collect();
        let input = Arc::new(SourceInput { bytes, includes });
        files.insert(path.to_path_buf(), Arc::clone(&input));
        Ok(input)
    }

    pub fn signature(&self, root: &Path, source: &Path, commands: &[Vec<String>]) -> Result<Vec<u8>, String> {
        let source = if source.is_absolute() {
            source.into()
        } else {
            root.join(source)
        };
        let dirs = include_dirs(root, commands);
        let base = std::fs::canonicalize(root).ok();
        let base = base.as_deref();
        let mut hash = Sha256::new();
        let mut seen = BTreeSet::new();
        let mut active = BTreeSet::new();
        visit(self, base, &source, &dirs, &mut seen, &mut active, &mut hash)?;
        for command in commands {
            for pair in command.windows(2) {
                if pair[0] == "-include" {
                    let path = Path::new(&pair[1]);
                    let path = if path.is_absolute() {
                        path.to_path_buf()
                    } else {
                        root.join(path)
                    };
                    visit(self, base, &path, &dirs, &mut seen, &mut active, &mut hash)?;
                }
            }
        }
        Ok(hash.finalize().to_vec())
    }
}

fn include_dirs(root: &Path, commands: &[Vec<String>]) -> Vec<PathBuf> {
    commands
        .iter()
        .flat_map(|args| {
            args.iter().enumerate().filter_map(|(i, arg)| {
                let value = if arg == "-I" {
                    args.get(i + 1).map(String::as_str)
                } else {
                    arg.strip_prefix("-I").filter(|s| !s.is_empty())
                }?;
                let path = Path::new(value);
                Some(if path.is_absolute() {
                    path.into()
                } else {
                    root.join(path)
                })
            })
        })
        .collect()
}

pub fn quoted_include(line: &str) -> Option<&str> {
    let rest = line
        .trim_start()
        .strip_prefix("#include")?
        .trim_start()
        .strip_prefix('"')?;
    Some(rest.split_once('"')?.0)
}

pub fn portable_commands(root: &Path, commands: &[Vec<String>]) -> Vec<Vec<String>> {
    let root = root.to_string_lossy();
    commands
        .iter()
        .map(|command| {
            command
                .iter()
                .map(|part| part.replace(root.as_ref(), "<root>"))
                .collect()
        })
        .collect()
}

fn visit(
    cache: &SourceInputCache,
    base: Option<&Path>,
    path: &Path,
    dirs: &[PathBuf],
    seen: &mut BTreeSet<PathBuf>,
    active: &mut BTreeSet<PathBuf>,
    hash: &mut Sha256,
) -> Result<(), String> {
    let path = std::fs::canonicalize(path).map_err(|error| error.to_string())?;
    if active.contains(&path) && path.extension().is_some_and(|ext| ext == "c") {
        return Err(format!("recursive C source include: {}", path.display()));
    }
    if !seen.insert(path.clone()) {
        return Ok(());
    }
    active.insert(path.clone());
    let input = cache.read(&path)?;
    let name = base
        .and_then(|base| path.strip_prefix(base).ok())
        .unwrap_or(&path);
    for input in [name.to_string_lossy().as_bytes(), &input.bytes] {
        hash.update((input.len() as u64).to_be_bytes());
        hash.update(input);
    }
    for name in &input.includes {
        let name = Path::new(name);
        let found = std::iter::once(path.parent().unwrap_or(Path::new("")).join(name))
            .chain(dirs.iter().map(|dir| dir.join(name)))
            .find(|candidate| candidate.is_file());
        if let Some(found) = found {
            visit(cache, base, &found, dirs, seen, active, hash)?;
        }
    }
    active.remove(&path);
    Ok(())
}

#[cfg(test)]
fn compiler_source_tree_signature(
    root: &Path,
    source: &Path,
    commands: &[Vec<String>],
) -> Result<Vec<u8>, String> {
    SourceInputCache::default().signature(root, source, commands)
}

#[cfg(test)]
mod tests {
    use super::*;

    fn write(root: &Path, name: &str, text: &str) {
        let path = root.join(name);
        std::fs::create_dir_all(path.parent().unwrap()).unwrap();
        std::fs::write(path, text).unwrap();
    }

    // Independent expected visit order, framed exactly as the uncached input
    // signature: root-relative canonical name, then the complete source bytes.
    fn expected(root: &Path, names: &[&str]) -> Vec<u8> {
        let mut hash = Sha256::new();
        for name in names {
            for input in [name.as_bytes().to_vec(), std::fs::read(root.join(name)).unwrap()] {
                hash.update((input.len() as u64).to_be_bytes());
                hash.update(input);
            }
        }
        hash.finalize().to_vec()
    }

    #[test]
    fn shared_reads_preserve_each_units_dependency_order_and_cycles() {
        let directory = tempfile::tempdir().unwrap();
        let root = directory.path();
        write(root, "src/one.c", "#include \"first.h\"\n#include \"repeat.h\"\n#include \"missing.h\"\n");
        write(root, "src/two.c", "#include \"repeat.h\"\n#include \"first.h\"\n");
        write(root, "src/first.h", "#include \"shared.h\"\n");
        write(root, "inc/first.h", "this search-path header is shadowed\n");
        write(root, "inc/shared.h", "#include \"leaf.h\"\n#include \"cycle.h\"\n");
        write(root, "inc/leaf.h", "leaf\n");
        write(root, "inc/cycle.h", "#include \"shared.h\"\n");
        write(root, "inc/repeat.h", "#include \"shared.h\"\n");
        write(root, "other/forced.h", "#include \"shared.h\"\n");
        write(root, "other/shared.h", "other leaf\n");
        let commands = vec![vec!["cpp".into(), "-I".into(), "inc".into(), "-Iother".into(), "-include".into(), "other/forced.h".into(), "-include".into(), "src/first.h".into()]];
        let one = expected(root, &["src/one.c", "src/first.h", "inc/shared.h", "inc/leaf.h", "inc/cycle.h", "inc/repeat.h", "other/forced.h", "other/shared.h"]);
        let two = expected(root, &["src/two.c", "inc/repeat.h", "inc/shared.h", "inc/leaf.h", "inc/cycle.h", "src/first.h", "other/forced.h", "other/shared.h"]);
        let cache = SourceInputCache::default();
        std::thread::scope(|scope| {
            let handles = (0..8).map(|i| {
                let (cache, commands, one, two) = (&cache, &commands, &one, &two);
                scope.spawn(move || {
                    let (source, wanted) = if i % 2 == 0 { ("src/one.c", one) } else { ("src/two.c", two) };
                    assert_eq!(cache.signature(root, Path::new(source), commands).unwrap(), *wanted);
                })
            }).collect::<Vec<_>>();
            for handle in handles { handle.join().unwrap(); }
        });
        assert_eq!(cache.files.read().unwrap().len(), 9);
        assert_eq!(compiler_source_tree_signature(root, Path::new("src/one.c"), &commands).unwrap(), one);
    }

    #[test]
    fn shared_inputs_do_not_memoize_include_search_resolution() {
        let directory = tempfile::tempdir().unwrap();
        let root = directory.path();
        write(root, "unit.c", "#include \"shared.h\"\n");
        write(root, "first/shared.h", "first\n");
        write(root, "second/shared.h", "second\n");
        let first = vec![vec!["cpp".into(), "-Ifirst".into(), "-I".into(), "second".into()]];
        let second = vec![vec!["cpp".into(), "-Isecond".into(), "-I".into(), "first".into()]];
        let cache = SourceInputCache::default();
        assert_eq!(cache.signature(root, Path::new("unit.c"), &first).unwrap(), expected(root, &["unit.c", "first/shared.h"]));
        assert_eq!(cache.signature(root, Path::new("unit.c"), &second).unwrap(), expected(root, &["unit.c", "second/shared.h"]));
        write(root, "shared.h", "now the local header shadows both search paths\n");
        assert_eq!(cache.signature(root, Path::new("unit.c"), &first).unwrap(), expected(root, &["unit.c", "shared.h"]));
    }

    #[test]
    fn a_new_invocation_observes_source_header_and_missing_include_changes() {
        let directory = tempfile::tempdir().unwrap();
        let root = directory.path();
        write(root, "unit.c", "#include \"header.h\"\n#include \"missing.h\"\n");
        write(root, "header.h", "before\n");
        let signature = || compiler_source_tree_signature(root, Path::new("unit.c"), &[]).unwrap();
        let first = signature();
        assert_eq!(first, expected(root, &["unit.c", "header.h"]));
        write(root, "header.h", "after\n");
        let edited_header = signature();
        assert_ne!(first, edited_header);
        write(root, "missing.h", "now present\n");
        let created_header = signature();
        assert_ne!(edited_header, created_header);
        assert_eq!(created_header, expected(root, &["unit.c", "header.h", "missing.h"]));
        std::fs::remove_file(root.join("header.h")).unwrap();
        let deleted_header = signature();
        assert_ne!(created_header, deleted_header);
        assert_eq!(deleted_header, expected(root, &["unit.c", "missing.h"]));
        write(root, "unit.c", "changed source\n");
        assert_ne!(deleted_header, signature());
        assert_eq!(signature(), expected(root, &["unit.c"]));
    }

    #[test]
    fn shared_reads_preserve_recursive_c_and_required_input_errors() {
        let directory = tempfile::tempdir().unwrap();
        let root = directory.path();
        write(root, "one.c", "#include \"two.c\"\n");
        write(root, "two.c", "#include \"one.c\"\n");
        let cache = SourceInputCache::default();
        let wanted = format!("recursive C source include: {}", std::fs::canonicalize(root.join("one.c")).unwrap().display());
        for _ in 0..2 {
            assert_eq!(cache.signature(root, Path::new("one.c"), &[]).unwrap_err(), wanted);
        }
        assert!(cache.signature(root, Path::new("missing.c"), &[]).is_err());
        write(root, "unit.c", "unit\n");
        let forced = vec![vec!["cpp".into(), "-include".into(), "missing.h".into()]];
        assert!(cache.signature(root, Path::new("unit.c"), &forced).is_err());
        let required_directory = vec![vec!["cpp".into(), "-include".into(), ".".into()]];
        assert!(cache.signature(root, Path::new("unit.c"), &required_directory).is_err());
        write(root, "missing.h", "required input now present\n");
        assert_eq!(cache.signature(root, Path::new("unit.c"), &forced).unwrap(), expected(root, &["unit.c", "missing.h"]));
    }

    #[cfg(unix)]
    #[test]
    fn shared_reads_are_keyed_by_canonical_path_without_changing_hashed_names() {
        let directory = tempfile::tempdir().unwrap();
        let root = directory.path();
        write(root, "unit.c", "#include \"header.h\"\n#include \"alias.h\"\n");
        write(root, "header.h", "header\n");
        std::os::unix::fs::symlink("header.h", root.join("alias.h")).unwrap();
        let cache = SourceInputCache::default();
        assert_eq!(cache.signature(root, Path::new("unit.c"), &[]).unwrap(), expected(root, &["unit.c", "header.h"]));
        assert_eq!(cache.files.read().unwrap().len(), 2);
    }
}
