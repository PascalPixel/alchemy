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

fn visit(
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
            visit(&found, dirs, seen, active, hash)?;
        }
    }
    active.remove(&path);
    Ok(())
}

pub fn source_tree_signature(source: &Path, dirs: &[PathBuf]) -> Result<Vec<u8>, String> {
    let mut hash = Sha256::new();
    visit(
        source,
        dirs,
        &mut BTreeSet::new(),
        &mut BTreeSet::new(),
        &mut hash,
    )?;
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
    let dirs = include_dirs(root, commands);
    let mut hash = Sha256::new();
    let mut seen = BTreeSet::new();
    let mut active = BTreeSet::new();
    visit(&source, &dirs, &mut seen, &mut active, &mut hash)?;
    // Generated address bindings are compiler inputs even though the C file
    // does not include them. Their stable filenames are not a cache identity.
    for command in commands {
        for pair in command.windows(2) {
            if pair[0] == "-include" {
                let path = Path::new(&pair[1]);
                let path = if path.is_absolute() {
                    path.to_path_buf()
                } else {
                    root.join(path)
                };
                visit(&path, &dirs, &mut seen, &mut active, &mut hash)?;
            }
        }
    }
    Ok(hash.finalize().to_vec())
}

#[cfg(test)]
#[test]
fn forced_binding_mutation_changes_compiler_input_identity() {
    let root = tempfile::tempdir().unwrap();
    let source = root.path().join("owner.c");
    let bindings = root.path().join("bindings.h");
    std::fs::write(&source, "void Owner(void) { Target(); }\n").unwrap();
    std::fs::write(&bindings, "#define Target Func_08001000\n").unwrap();
    let commands = vec![vec!["cpp0".into(), "-include".into(), "bindings.h".into()]];
    let first = compiler_source_tree_signature(root.path(), &source, &commands).unwrap();
    std::fs::write(&bindings, "#define Target Func_08002000\n").unwrap();
    let second = compiler_source_tree_signature(root.path(), &source, &commands).unwrap();
    assert_ne!(first, second);
    std::fs::remove_file(&bindings).unwrap();
    assert!(compiler_source_tree_signature(root.path(), &source, &commands).is_err());
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
    std::fs::write(&body, "#include \"owner.c\"\n").unwrap();
    assert!(compiler_source_tree_signature(root.path(), &source, &[])
        .unwrap_err()
        .contains("recursive C source include"));
}
