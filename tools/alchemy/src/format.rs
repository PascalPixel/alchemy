use crate::compiler::{canonical_json::canonical_json, routing::root};
use serde_json::Value;

pub const NATIVE_ROOTS: [&str; 7] = [
    "games/COMMON",
    "games/THE BROKEN SEAL/SRC",
    "games/THE BROKEN SEAL/INCLUDE",
    "games/THE BROKEN SEAL/SOUND",
    "games/THE BROKEN SEAL/TEXT",
    "games/THE BROKEN SEAL/PREVIEW",
    "games/THE BROKEN SEAL/SOURCE.JSON",
];

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
