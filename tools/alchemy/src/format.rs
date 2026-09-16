use crate::compiler::{canonical_json::canonical_json, routing::root};
use serde_json::Value;
use std::path::Path;

pub const NATIVE_ROOTS: [&str; 12] = [
    "games/COMMON/SRC",
    "games/THE LOST AGE/SRC",
    "games/THE LOST AGE/INCLUDE",
    "games/THE LOST AGE/TEXT",
    "games/THE LOST AGE/PROJECT.JSON",
    "games/THE LOST AGE/SOURCE.JSON",
    "games/THE BROKEN SEAL/SRC",
    "games/THE BROKEN SEAL/INCLUDE",
    "games/THE BROKEN SEAL/SOUND",
    "games/THE BROKEN SEAL/TEXT",
    "games/THE BROKEN SEAL/PREVIEW",
    "games/THE BROKEN SEAL/SOURCE.JSON",
];

fn exact_file(path: &Path) -> bool {
    let mut current = std::path::PathBuf::new();
    for part in path.components() {
        match part {
            std::path::Component::ParentDir => {
                current.pop();
            }
            std::path::Component::CurDir => {}
            _ => current.push(part.as_os_str()),
        }
    }
    while let Some(name) = current.file_name() {
        let Some(parent) = current.parent() else {
            return false;
        };
        if !std::fs::read_dir(parent)
            .is_ok_and(|entries| entries.flatten().any(|entry| entry.file_name() == name))
        {
            return false;
        }
        current = parent.to_path_buf();
    }
    path.is_file()
}

fn check_table_sources(path: &Path) -> Result<(), String> {
    let text = std::fs::read_to_string(path).map_err(|e| e.to_string())?;
    let mut rows = text.lines().filter(|line| !line.starts_with('#'));
    let Some(column) = rows
        .next()
        .and_then(|header| header.split('\t').position(|field| field == "source"))
    else {
        return Ok(());
    };
    for row in rows {
        let Some(source) = row
            .split('\t')
            .nth(column)
            .filter(|source| !source.is_empty())
        else {
            continue;
        };
        let suffix = Path::new(source)
            .extension()
            .and_then(|e| e.to_str())
            .unwrap_or("");
        if !["mid", "wav", "png"]
            .iter()
            .any(|e| suffix.eq_ignore_ascii_case(e))
        {
            continue;
        }
        let resolved = if source.starts_with("games/") {
            root().join(source)
        } else {
            path.parent().unwrap().join(source)
        };
        if !exact_file(&resolved) {
            return Err(format!(
                "{}: source filename must exist with exact spelling: {source}",
                path.display()
            ));
        }
    }
    Ok(())
}

pub fn run(arguments: &[String]) -> Result<(), String> {
    let check = arguments == ["--check"];
    if !check && !arguments.is_empty() {
        return Err("usage: alchemy format [--check]".into());
    }
    let mut changed = Vec::new();
    let mut count = 0;
    for directory in NATIVE_ROOTS {
        for entry in walkdir::WalkDir::new(root().join(directory)) {
            let entry = entry.map_err(|e| e.to_string())?;
            if !entry.file_type().is_file() {
                continue;
            }
            let path = entry.path();
            if !exact_file(path) {
                return Err(format!(
                    "native path must exist with exact spelling: {}",
                    path.display()
                ));
            }
            if path
                .file_name()
                .and_then(|p| p.to_str())
                .is_some_and(|name| name.starts_with('.'))
            {
                continue;
            }
            let relative = path.strip_prefix(root()).map_err(|e| e.to_string())?;
            let name = relative.to_str().ok_or("native filename is not UTF8")?;
            let owned = name
                .strip_prefix(directory)
                .unwrap_or("")
                .trim_start_matches('/');
            if !owned
                .split('/')
                .all(|part| part == part.to_ascii_uppercase())
            {
                return Err(format!("native filenames must be uppercase: {name}"));
            }
            if path.extension().and_then(|e| e.to_str()) == Some("TSV") {
                check_table_sources(path)?;
            }
            if path.extension().and_then(|e| e.to_str()) != Some("JSON") {
                continue;
            }
            let original = std::fs::read_to_string(path).map_err(|e| e.to_string())?;
            let value: Value =
                serde_json::from_str(&original).map_err(|e| format!("{name}: {e}"))?;
            let formatted = format!("{}\n", canonical_json(&value));
            if original != formatted {
                if check {
                    changed.push(name.to_string());
                } else {
                    std::fs::write(path, formatted).map_err(|e| e.to_string())?;
                }
            }
            count += 1;
        }
    }
    if !changed.is_empty() {
        return Err(format!(
            "run alchemy format; noncanonical native data: {}",
            changed
                .iter()
                .take(10)
                .cloned()
                .collect::<Vec<_>>()
                .join(", ")
        ));
    }
    println!("native format ok: json_files={count} width=120 uppercase=true");
    Ok(())
}

#[cfg(test)]
mod tests {
    #[test]
    fn media_references_require_exact_filename_case() {
        let dir = tempfile::tempdir().unwrap();
        std::fs::write(dir.path().join("TRACK.MID"), b"test").unwrap();
        assert!(super::exact_file(&dir.path().join("TRACK.MID")));
        assert!(!super::exact_file(&dir.path().join("track.mid")));
        std::fs::create_dir(dir.path().join("SOUND")).unwrap();
        std::fs::write(dir.path().join("SOUND/TRACK.MID"), b"test").unwrap();
        assert!(super::exact_file(&dir.path().join("SOUND/TRACK.MID")));
        assert!(!super::exact_file(&dir.path().join("sound/TRACK.MID")));
    }
}
