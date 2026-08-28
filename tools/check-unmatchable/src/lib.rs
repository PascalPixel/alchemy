pub mod cli;

use compiler_core::source_paths::{SourceOwner, SourcePaths};
use serde_json::Value;
use std::collections::HashSet;
use std::path::{Path, PathBuf};

fn root() -> PathBuf {
    compiler_core::routing::root().to_path_buf()
}

fn json(path: &Path) -> Result<Value, String> {
    let bytes = std::fs::read(path).map_err(|error| format!("{}: {error}", path.display()))?;
    serde_json::from_slice(&bytes).map_err(|error| format!("{}: {error}", path.display()))
}

fn exact(root: &Path) -> Result<HashSet<String>, String> {
    let paths = SourcePaths::load(root)?;
    paths.validate_tree()?;
    let stems = paths
        .all_sources()?
        .into_iter()
        .map(|source| source.owner.legacy_stem())
        .collect::<HashSet<_>>();
    if stems.is_empty() {
        return Err("games/gs1/src/ contains no C owners".into());
    }
    Ok(stems)
}

fn validate_registered_main_symbols(root: &Path) -> Result<usize, String> {
    let register = SourcePaths::load(root)?;
    let mut count = 0;
    for entry in std::fs::read_dir(root.join("games/gs1/asm"))
        .map_err(|error| format!("games/gs1/asm: {error}"))?
    {
        let path = entry.map_err(|error| error.to_string())?.path();
        let Some(stem) = path.file_stem().and_then(|value| value.to_str()) else {
            continue;
        };
        if path.extension().and_then(|value| value.to_str()) != Some("s")
            || stem.len() != 8
            || !stem.bytes().all(|byte| byte.is_ascii_hexdigit())
        {
            continue;
        }
        let text = std::fs::read_to_string(&path)
            .map_err(|error| format!("{}: {error}", path.display()))?;
        let exported = text.lines().find_map(|line| {
            let mut fields = line.split_ascii_whitespace();
            match (fields.next(), fields.next(), fields.next()) {
                (Some(".global"), Some(symbol), None) => Some(symbol),
                _ => None,
            }
        });
        let Some(exported) = exported else {
            continue;
        };
        let owner = SourceOwner::Main(
            u32::from_str_radix(stem, 16).map_err(|error| format!("{stem}: {error}"))?,
        );
        let expected = register
            .registered_name(owner)
            .map(str::to_owned)
            .unwrap_or_else(|| owner.legacy_name());
        if exported != expected {
            return Err(format!(
                "{} exports {exported}, but the owner register names it {expected}",
                path.display()
            ));
        }
        count += 1;
    }
    Ok(count)
}

fn audited(root: &Path) -> Result<HashSet<String>, String> {
    let document = json(&root.join("games/gs1/semantic/regions.json"))?;
    let mut stems = HashSet::new();
    for row in document
        .get("manual_regions")
        .and_then(Value::as_array)
        .ok_or("games/gs1/semantic/regions.json has no manual_regions")?
    {
        let Some(overlay) = row.get("overlay").and_then(Value::as_str) else {
            continue;
        };
        let Some(entry) = row.get("entry").and_then(Value::as_str) else {
            continue;
        };
        let address = u32::from_str_radix(entry.trim_start_matches("0x"), 16)
            .map_err(|_| format!("invalid owner address {entry}"))?;
        stems.insert(format!("{overlay}_c_{address:08x}"));
    }
    if stems.is_empty() {
        return Err("games/gs1/semantic/regions.json contains no audited owners".into());
    }
    Ok(stems)
}

fn validate_unmatchable(
    root: &Path,
    exact: &HashSet<String>,
    audited: &HashSet<String>,
) -> Result<usize, String> {
    let document = json(&root.join("games/gs1/semantic/unmatchable.json"))?;
    let rows = document
        .get("unmatchable")
        .and_then(Value::as_array)
        .ok_or("games/gs1/semantic/unmatchable.json has no unmatchable array")?;
    let mut seen = HashSet::new();
    for row in rows {
        let owner = row
            .get("owner")
            .and_then(Value::as_str)
            .ok_or("unmatchable owner missing")?;
        if !seen.insert(owner) {
            return Err(format!("{owner} is listed twice"));
        }
        if exact.contains(owner) {
            return Err(format!(
                "{owner} is byte-exact; remove its unmatchable entry"
            ));
        }
        if !audited.contains(owner) {
            return Err(format!("{owner} is not an audited owner"));
        }
        if row
            .get("floor_halfwords")
            .and_then(Value::as_u64)
            .unwrap_or(0)
            == 0
        {
            return Err(format!("{owner} has no positive floor"));
        }
        let axes = row
            .get("axes")
            .and_then(Value::as_array)
            .ok_or_else(|| format!("{owner} has no axes"))?;
        for required in ["compiler", "shape"] {
            if !axes.iter().any(|axis| axis.as_str() == Some(required)) {
                return Err(format!("{owner} has not exhausted the {required} axis"));
            }
        }
        if row
            .get("reason")
            .and_then(Value::as_str)
            .unwrap_or("")
            .trim()
            .is_empty()
        {
            return Err(format!("{owner} has no reason"));
        }
    }
    Ok(rows.len())
}

fn validate_provisional(root: &Path, exact: &HashSet<String>) -> Result<usize, String> {
    let document = json(&root.join("games/gs1/src/provisional.json"))?;
    let rows = document
        .get("provisional")
        .and_then(Value::as_array)
        .ok_or("games/gs1/src/provisional.json has no provisional array")?;
    for row in rows {
        let owner = row
            .get("owner")
            .and_then(Value::as_str)
            .ok_or("provisional owner missing")?;
        if !exact.contains(owner) {
            return Err(format!("{owner} is provisional but not in games/gs1/src/"));
        }
        if row
            .get("reason")
            .and_then(Value::as_str)
            .unwrap_or("")
            .trim()
            .is_empty()
        {
            return Err(format!("{owner} has no provisional reason"));
        }
    }
    Ok(rows.len())
}

fn validate_sealed(root: &Path, exact: &HashSet<String>) -> Result<usize, String> {
    let document = json(&root.join("games/gs1/semantic/sealed.json"))?;
    let rows = document
        .get("sealed")
        .and_then(Value::as_array)
        .ok_or("games/gs1/semantic/sealed.json has no sealed array")?;
    let mut seen = HashSet::new();
    for row in rows {
        let owner = row
            .get("owner")
            .and_then(Value::as_str)
            .ok_or("sealed owner missing")?;
        if owner.len() != 8 || !owner.bytes().all(|byte| byte.is_ascii_hexdigit()) {
            return Err(format!("{owner} is not an 8-hex main owner"));
        }
        if !seen.insert(owner) {
            return Err(format!("{owner} is sealed twice"));
        }
        if exact.contains(owner) {
            return Err(format!("{owner} has exact C; remove its seal"));
        }
        if !root.join(format!("games/gs1/asm/{owner}.s")).is_file() {
            return Err(format!("{owner} is sealed but has no retained assembly"));
        }
        if row
            .get("reason")
            .and_then(Value::as_str)
            .unwrap_or("")
            .trim()
            .is_empty()
        {
            return Err(format!("{owner} has no sealed reason"));
        }
    }
    Ok(rows.len())
}

fn validate_drafts(root: &Path, exact: &HashSet<String>) -> Result<usize, String> {
    let directory = root.join("draft");
    let Ok(entries) = std::fs::read_dir(directory) else {
        return Ok(0);
    };
    let mut count = 0;
    for entry in entries {
        let path = entry.map_err(|error| error.to_string())?.path();
        if path.extension().and_then(|value| value.to_str()) != Some("json") {
            continue;
        }
        if path
            .file_name()
            .and_then(|value| value.to_str())
            .is_some_and(|name| name.starts_with("README"))
        {
            continue;
        }
        let record = json(&path)?;
        let stem = path
            .file_stem()
            .and_then(|value| value.to_str())
            .unwrap_or("");
        if exact.contains(stem) {
            return Err(format!("{} belongs to an exact owner", path.display()));
        }
        if record
            .get("owner")
            .and_then(Value::as_str)
            .unwrap_or("")
            .is_empty()
            || record.get("score").is_none()
        {
            return Err(format!("{} lacks an owner or score", path.display()));
        }
        if record
            .pointer("/score/differing_halfwords")
            .and_then(Value::as_u64)
            == Some(0)
            || record.get("differing_halfwords").and_then(Value::as_u64) == Some(0)
        {
            return Err(format!("{} is exact and must be adopted", path.display()));
        }
        count += 1;
    }
    Ok(count)
}

fn validate_reconstruction_records(root: &Path) -> Result<(), String> {
    for relative in ["games/gs1/recon/en/main", "games/gs1/recon/en/overlays"] {
        for entry in std::fs::read_dir(root.join(relative))
            .map_err(|error| format!("{relative}: {error}"))?
        {
            let path = entry.map_err(|error| error.to_string())?.path();
            if path.extension().and_then(|value| value.to_str()) != Some("json") {
                continue;
            }
            let stem = path
                .file_stem()
                .and_then(|value| value.to_str())
                .ok_or_else(|| format!("{} has no UTF-8 owner stem", path.display()))?;
            SourceOwner::from_legacy_stem(stem)
                .ok_or_else(|| format!("{} has no owner-qualified filename", path.display()))?;
            let record = json(&path)?;
            for duplicate in ["owner", "semantic_name"] {
                if record.get(duplicate).is_some() {
                    return Err(format!("{} repeats {duplicate}; owner identity comes from the filename and names from games/gs1/source-paths.json", path.display()));
                }
            }
        }
    }
    Ok(())
}

pub fn validate() -> Result<(usize, usize, usize, usize, usize, usize), String> {
    let root = root();
    let exact = exact(&root)?;
    let names = validate_registered_main_symbols(&root)?;
    let audited = audited(&root)?;
    let unmatchable = validate_unmatchable(&root, &exact, &audited)?;
    let provisional = validate_provisional(&root, &exact)?;
    let sealed = validate_sealed(&root, &exact)?;
    let drafts = validate_drafts(&root, &exact)?;
    validate_reconstruction_records(&root)?;
    Ok((
        unmatchable,
        provisional,
        sealed,
        drafts,
        audited.len(),
        names,
    ))
}
