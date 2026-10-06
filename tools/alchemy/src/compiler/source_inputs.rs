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
    let bytes = std::fs::read(&path).map_err(|error| error.to_string())?;
    let name = base
        .and_then(|base| path.strip_prefix(base).ok())
        .unwrap_or(&path);
    for input in [name.to_string_lossy().as_bytes(), &bytes] {
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
            visit(base, &found, dirs, seen, active, hash)?;
        }
    }
    active.remove(&path);
    Ok(())
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
    let dirs = include_dirs(root, commands);
    let base = std::fs::canonicalize(root).ok();
    let base = base.as_deref();
    let mut hash = Sha256::new();
    let mut seen = BTreeSet::new();
    let mut active = BTreeSet::new();
    visit(base, &source, &dirs, &mut seen, &mut active, &mut hash)?;
    for command in commands {
        for pair in command.windows(2) {
            if pair[0] == "-include" {
                let path = Path::new(&pair[1]);
                let path = if path.is_absolute() {
                    path.to_path_buf()
                } else {
                    root.join(path)
                };
                visit(base, &path, &dirs, &mut seen, &mut active, &mut hash)?;
            }
        }
    }
    Ok(hash.finalize().to_vec())
}

