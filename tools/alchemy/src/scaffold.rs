//! Declare a translation unit of main-image drafts over a contiguous range.
//!
//!   unit-scaffold <game> <unit-id> <start-hex> <end-hex> [--apply]
//!
//! Reads the owner inventory for the range and declares every not-yet-C owner
//! with a draft in recon/<game>/en/main as one unit. Loading the manifest
//! writes the unit's composite, its drafts included in address order, under
//! out/units/<game>/; it is never tracked. Exact owners compile from their
//! own registered sources and owners with no C anywhere stay retained
//! assembly, so neither joins the unit. With --apply the entry is appended to
//! recon/<game>/translation-units.json; without it the entry is printed.
//! Resolving declaration collisions between the drafts belongs in the drafts
//! and shared headers, scored with diff --unit.

use crate::compiler::build_io::read_json;
use serde_json::{json, Value};
use std::collections::BTreeSet;
use std::fs;
use std::path::Path;
use std::process::ExitCode;

struct Row {
    address: u32,
    draft: bool,
    exact: bool,
    extent: u64,
}

pub fn entry(args: &[String]) -> ExitCode {
    match run(args) {
        Ok(()) => ExitCode::SUCCESS,
        Err(error) => {
            eprintln!("{error}");
            ExitCode::FAILURE
        }
    }
}

fn run(args: &[String]) -> Result<(), String> {
    const USAGE: &str =
        "usage: alchemy unit scaffold <game> <unit-id> <start-hex> <end-hex> [--apply]";
    if args == ["--help"] || args == ["-h"] {
        println!("{USAGE}");
        return Ok(());
    }
    let apply = args.iter().any(|a| a == "--apply");
    let positional: Vec<&String> = args.iter().filter(|a| *a != "--apply").collect();
    let [game, unit_id, start_hex, end_hex] = positional[..] else {
        return Err(USAGE.into());
    };
    let start = u32::from_str_radix(start_hex.trim_start_matches("0x"), 16)
        .map_err(|_| "start is not hex")?;
    let end =
        u32::from_str_radix(end_hex.trim_start_matches("0x"), 16).map_err(|_| "end is not hex")?;
    let recon = crate::compiler::routing::recon_directory(game);
    let inventory: Value = read_json(Path::new(&format!(
        "out/{game}-en/full/rebuilt.owner-inventory.json"
    )))?;
    let manifest_path = format!("{recon}/translation-units.json");
    let mut manifest: Value = read_json(Path::new(&manifest_path))?;
    // Owners already claimed by a declared unit are excluded: overlapping
    // declarations can never coexist, and the fix is extending that unit.
    let mut declared: BTreeSet<u32> = BTreeSet::new();
    for unit in manifest["units"].as_array().ok_or("manifest lacks units")? {
        for owner in unit["owners"].as_array().unwrap_or(&Vec::new()) {
            if let Some(address) = hex_of(&owner["address"]) {
                declared.insert(address);
            }
        }
    }
    let mut rows: Vec<Row> = Vec::new();
    let mut skipped: Vec<String> = Vec::new();
    for owner in inventory["owners"]
        .as_array()
        .ok_or("inventory lacks owners")?
    {
        if owner["container"]["kind"] != "main-rom" {
            continue;
        }
        let Some(address) = hex_of(&owner["address"]) else {
            continue;
        };
        if address < start || address >= end || declared.contains(&address) {
            continue;
        }
        let hex = format!("{address:08x}");
        let production = &owner["production"];
        let registered = !owner["registration"]["source_path"].is_null();
        let exact = production["state"] == "exact-c";
        if registered && !exact {
            // Named while still assembly: fits neither manifest state.
            skipped.push(hex);
            continue;
        }
        rows.push(Row {
            address,
            draft: !exact && Path::new(&format!("{recon}/en/main/{hex}.c")).exists(),
            exact,
            extent: production["extent_bytes"].as_u64().unwrap_or(0),
        });
    }
    rows.sort_by_key(|r| r.address);
    if rows.is_empty() {
        return Err("no owners in range".into());
    }
    let drafts = rows.iter().filter(|r| r.draft).collect::<Vec<_>>();
    if drafts.is_empty() {
        return Err("no drafts in range".into());
    }
    if apply
        && manifest["units"]
            .as_array()
            .is_some_and(|units| units.iter().any(|u| u["id"] == *unit_id.as_str()))
    {
        return Err(format!("unit {unit_id} already declared; nothing written"));
    }
    let entry = json!({
        "id": unit_id,
        "owners": drafts.iter().map(|r| json!({
            "address": format!("0x{:08x}", r.address),
            "extent": r.extent,
        })).collect::<Vec<_>>(),
    });
    println!(
        "{unit_id}: {} drafts ({} exact owners compile from their own sources, {} holes{})",
        drafts.len(),
        rows.iter().filter(|r| r.exact).count(),
        rows.iter().filter(|r| !r.exact && !r.draft).count(),
        if skipped.is_empty() {
            String::new()
        } else {
            format!("; skipped named-assembly: {}", skipped.join(" "))
        }
    );
    if apply {
        let units = manifest["units"]
            .as_array_mut()
            .ok_or("manifest lacks units")?;
        units.push(entry);
        let text = serde_json::to_string_pretty(&manifest).map_err(|e| e.to_string())?;
        fs::write(&manifest_path, format!("{text}\n"))
            .map_err(|e| format!("{manifest_path}: {e}; nothing declared"))?;
        println!("declared {unit_id} in {manifest_path}");
    } else {
        println!(
            "{}",
            serde_json::to_string_pretty(&entry).map_err(|e| e.to_string())?
        );
    }
    Ok(())
}

fn hex_of(value: &Value) -> Option<u32> {
    u32::from_str_radix(value.as_str()?.trim_start_matches("0x"), 16).ok()
}
