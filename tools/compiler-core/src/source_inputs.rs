use sha2::{Digest, Sha256};
use std::collections::BTreeSet;
use std::path::{Path, PathBuf};

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

fn quoted_include(line: &str) -> Option<&str> {
    let rest = line
        .trim_start()
        .strip_prefix("#include")?
        .trim_start()
        .strip_prefix('"')?;
    Some(rest.split_once('"')?.0)
}

fn visit(
    path: &Path,
    dirs: &[PathBuf],
    seen: &mut BTreeSet<PathBuf>,
    hash: &mut Sha256,
) -> Result<(), String> {
    let path = std::fs::canonicalize(path).map_err(|error| error.to_string())?;
    if !seen.insert(path.clone()) {
        return Ok(());
    }
    let bytes = std::fs::read(&path).map_err(|error| error.to_string())?;
    for input in [path.to_string_lossy().as_bytes(), &bytes] {
        hash.update((input.len() as u64).to_be_bytes());
        hash.update(input);
    }
    for name in String::from_utf8_lossy(&bytes)
        .lines()
        .filter_map(quoted_include)
    {
        let name = Path::new(name);
        let found = std::iter::once(path.parent().unwrap_or(Path::new("")).join(name))
            .chain(dirs.iter().map(|dir| dir.join(name)))
            .find(|candidate| candidate.is_file());
        if let Some(found) = found {
            visit(&found, dirs, seen, hash)?;
        }
    }
    Ok(())
}

pub fn source_tree_signature(source: &Path, dirs: &[PathBuf]) -> Result<Vec<u8>, String> {
    let mut hash = Sha256::new();
    visit(source, dirs, &mut BTreeSet::new(), &mut hash)?;
    Ok(hash.finalize().to_vec())
}

pub fn compiler_source_tree_signature(
    root: &Path,
    source: &Path,
    commands: &[Vec<String>],
) -> Result<Vec<u8>, String> {
    let source = if source.is_absolute() {
        source.into()
    } else {
        root.join(source)
    };
    source_tree_signature(&source, &include_dirs(root, commands))
}

#[cfg(test)]
#[test]
fn included_file_mutation_changes_compiler_input_identity() {
    let root = tempfile::tempdir().unwrap();
    let source = root.path().join("owner.c");
    let body = root.path().join("body.h");
    std::fs::write(&source, "#include \"body.h\"\n").unwrap();
    std::fs::write(&body, "before\n").unwrap();
    let first = compiler_source_tree_signature(root.path(), &source, &[]).unwrap();
    std::fs::write(&body, "after\n").unwrap();
    let second = compiler_source_tree_signature(root.path(), &source, &[]).unwrap();
    assert_ne!(first, second);
}
