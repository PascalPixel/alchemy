//! Owner-register and retained-candidate classification checks.

use crate::compiler::source_paths::{SourceOwner, SourcePaths};
use serde_json::Value;
use std::collections::HashSet;
use std::path::{Path, PathBuf};
use std::process::ExitCode;

fn root() -> PathBuf {
    crate::compiler::routing::root().to_path_buf()
}

fn json(path: &Path) -> Result<Value, String> {
    crate::compiler::build_io::read_json(path)
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
        return Err("games/THE BROKEN SEAL/SRC/ contains no C owners".into());
    }
    Ok(stems)
}

fn validate_registered_main_symbols(root: &Path) -> Result<usize, String> {
    let register = SourcePaths::load(root)?;
    let mut count = 0;
    for entry in std::fs::read_dir(root.join("recon/tbs/raw"))
        .map_err(|error| format!("recon/tbs/raw: {error}"))?
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
    let stems: HashSet<String> = crate::compiler::translation_units::reviewed_overlay_spans(root)?
        .into_keys()
        .map(|owner| owner.legacy_stem())
        .collect();
    if stems.is_empty() {
        return Err("recon/tbs/semantic/regions.json contains no audited owners".into());
    }
    Ok(stems)
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

fn validate() -> Result<(usize, usize, usize, usize), String> {
    let root = root();
    let exact = exact(&root)?;
    let shared = SourcePaths::validate_shared_sources(&root)?;
    let names = validate_registered_main_symbols(&root)?;
    let audited = audited(&root)?;
    let drafts = validate_drafts(&root, &exact)?;
    Ok((drafts, audited.len(), names, shared))
}

pub(super) fn entry(arguments: &[String]) -> ExitCode {
    if arguments
        .iter()
        .any(|argument| matches!(argument.as_str(), "-h" | "--help"))
    {
        println!("usage: check owners");
        return ExitCode::SUCCESS;
    }
    if !arguments.is_empty() {
        eprintln!("usage: check owners");
        return ExitCode::from(2);
    }
    match validate() {
        Ok((drafts, audited, names, shared)) => {
            println!("owner registers ok: {drafts} drafts, {audited} audited, {names} named main assembly owners, {shared} shared sources");
            ExitCode::SUCCESS
        }
        Err(error) => {
            eprintln!("error: {error}");
            ExitCode::FAILURE
        }
    }
}
