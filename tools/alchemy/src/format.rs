use crate::compiler::routing::root;
use std::path::Path;

/// Camelot-shaped roots: every name uppercase and spelled exactly.
pub const NATIVE_ROOTS: [&str; 9] = [
    "games/COMMON/SRC",
    "games/THE LOST AGE/SRC",
    "games/THE LOST AGE/INCLUDE",
    "games/THE LOST AGE/SOUND",
    "games/THE LOST AGE/TEXT",
    "games/THE BROKEN SEAL/SRC",
    "games/THE BROKEN SEAL/INCLUDE",
    "games/THE BROKEN SEAL/SOUND",
    "games/THE BROKEN SEAL/TEXT",
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
        if !listed(parent, name) {
            return false;
        }
        current = parent.to_path_buf();
    }
    path.is_file()
}

/// Whether `directory` lists `name` with exactly that spelling. A listing
/// is read once per run and again only when the directory changes: every
/// file under it asks.
fn listed(directory: &Path, name: &std::ffi::OsStr) -> bool {
    use std::collections::{HashMap, HashSet};
    use std::ffi::OsString;
    type Listing = (std::time::SystemTime, HashSet<OsString>);
    thread_local! {
        static LISTINGS: std::cell::RefCell<HashMap<std::path::PathBuf, Listing>> =
            Default::default();
    }
    let Ok(modified) = std::fs::metadata(directory).and_then(|metadata| metadata.modified()) else {
        return false;
    };
    LISTINGS.with(|listings| {
        let mut listings = listings.borrow_mut();
        if listings
            .get(directory)
            .is_none_or(|(seen, _)| *seen != modified)
        {
            let Ok(entries) = std::fs::read_dir(directory) else {
                return false;
            };
            let names = entries.flatten().map(|entry| entry.file_name()).collect();
            listings.insert(directory.to_path_buf(), (modified, names));
        }
        listings[directory].1.contains(name)
    })
}

pub fn run(arguments: &[String]) -> Result<(), String> {
    if arguments == ["--help"] || arguments == ["-h"] {
        println!("usage: alchemy format [--check]");
        return Ok(());
    }
    if arguments != ["--check"] && !arguments.is_empty() {
        return Err("usage: alchemy format [--check]".into());
    }
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
            count += 1;
        }
    }
    println!("native format ok: files={count} uppercase=true");
    Ok(())
}

#[cfg(test)]
mod tests {
    #[test]
    fn native_paths_require_exact_filename_case() {
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
