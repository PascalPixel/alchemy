//! Whole source-built sound sequences; MIDI events supply the bytes.

use crate::targets::DecompTarget;
use std::path::{Path, PathBuf};

pub(crate) fn build(
    root: &Path,
    target: DecompTarget,
    reference: &[u8],
) -> Result<Vec<(PathBuf, u64, Vec<u8>)>, String> {
    let root = std::fs::canonicalize(root).map_err(|error| error.to_string())?;
    let game = root.join(target.game_dir());
    let directory = game.join("SOUND");
    if !directory.is_dir() {
        return Ok(Vec::new());
    }
    let directory = std::fs::canonicalize(&directory).map_err(|error| error.to_string())?;
    if !directory.starts_with(&game) {
        return Err("sequence inputs must remain inside the selected game".into());
    }
    let mut sources = Vec::new();
    for entry in walkdir::WalkDir::new(&directory).follow_links(false) {
        let entry = entry.map_err(|error| error.to_string())?;
        if entry.file_type().is_file()
            && entry
                .path()
                .extension()
                .and_then(|ext| ext.to_str())
                .is_some_and(|ext| ext.eq_ignore_ascii_case("mid"))
        {
            sources.push(entry.into_path());
        }
    }
    sources.sort();
    let mut placements = Vec::new();
    for source in sources {
        // Malformed inputs and near misses remain attempts. Neither a partial
        // asset nor its conductor options can supply matching output bytes.
        let Ok((compiled, report)) = crate::build_assets::build_midi_sequence(&root, &source)
        else {
            continue;
        };
        let base = report["base"]
            .as_u64()
            .ok_or("sequence builder omitted its link base")?;
        let Some(start) = base
            .checked_sub(0x0800_0000)
            .and_then(|n| usize::try_from(n).ok())
        else {
            continue;
        };
        let Some(end) = start.checked_add(compiled.len()) else {
            continue;
        };
        if !compiled.is_empty() && reference.get(start..end) == Some(compiled.as_slice()) {
            let source = source
                .strip_prefix(&root)
                .map_err(|error| error.to_string())?
                .to_path_buf();
            placements.push((source, base, compiled));
        }
    }
    Ok(placements)
}
