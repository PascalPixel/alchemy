//! Probe retained main-image debt assembly with the exact-C corpus.
//!
//! Relocation-masked byte identity between a claimed region and a debt
//! assembly region is shape correspondence only; it never asserts original
//! translation units. A byte-exact ordinary twin is a hard error: it must be
//! adopted, not recorded.
use crate::cross_edition::compliance_error_for;
use compiler_core::source_paths::SourceOwner;
use compiler_core::thumb::{reference_map, relocation_info, Reference};
use serde_json::{json, Value};
use std::collections::BTreeMap;
use std::fs;
use std::path::Path;

const USAGE: &str = "usage: compiler twins [--min BYTES] (--write FILE | --check FILE)";
const ROM_BASE: u64 = 0x0800_0000;
const DEBT: [&str; 5] = [
    "c_candidate",
    "split_first",
    "merge_with_owner",
    "merge_with_function_owner",
    "merge_with_continuations",
];

struct Row {
    stem: String,
    source: Option<String>,
    ordinary: bool,
    bytes: Vec<u8>,
    references: Vec<Reference>,
}

fn regions(path: &Path) -> Result<Vec<Value>, String> {
    let text = fs::read_to_string(path).map_err(|error| format!("{}: {error}", path.display()))?;
    let manifest: Value = serde_json::from_str(&text).map_err(|error| error.to_string())?;
    manifest["regions"]
        .as_array()
        .cloned()
        .ok_or_else(|| format!("{} has no regions", path.display()))
}

fn row(
    rom: &[u8],
    address: u64,
    size: usize,
    stem: &str,
    source: Option<String>,
) -> Option<(Row, Vec<bool>)> {
    let offset = usize::try_from(address.checked_sub(ROM_BASE)?).ok()?;
    let bytes = rom.get(offset..offset + size)?.to_vec();
    let (mask, references) = relocation_info(&bytes, address);
    let ordinary = source.is_some()
        && matches!(
            compliance_error_for(SourceOwner::parse(&format!("main:{stem}")).ok()?),
            Ok(None)
        );
    Some((
        Row {
            stem: stem.to_string(),
            source,
            ordinary,
            bytes,
            references,
        },
        mask,
    ))
}

pub fn run(args: &[String]) -> Result<(), String> {
    let mut minimum = 16usize;
    let (mut write, mut check) = (None, None);
    let mut index = 0;
    while index < args.len() {
        match args[index].as_str() {
            "--min" => {
                index += 1;
                minimum = args
                    .get(index)
                    .and_then(|value| value.parse().ok())
                    .ok_or(USAGE)?;
            }
            "--write" => {
                index += 1;
                write = Some(args.get(index).ok_or(USAGE)?.clone());
            }
            "--check" => {
                index += 1;
                check = Some(args.get(index).ok_or(USAGE)?.clone());
            }
            "-h" | "--help" => {
                println!("{USAGE}");
                return Ok(());
            }
            _ => return Err(USAGE.into()),
        }
        index += 1;
    }
    let root = compiler_core::routing::root();
    let rom = fs::read(root.join("roms/gs1-en.gba")).map_err(|error| error.to_string())?;
    let sealed: Value = serde_json::from_str(
        &fs::read_to_string(root.join("games/gs1/semantic/sealed.json"))
            .map_err(|error| error.to_string())?,
    )
    .map_err(|error| error.to_string())?;
    let sealed = sealed["sealed"]
        .as_array()
        .map(|rows| {
            rows.iter()
                .filter_map(|row| row["owner"].as_str().map(str::to_owned))
                .collect::<Vec<_>>()
        })
        .unwrap_or_default();
    let mut groups = BTreeMap::<(usize, Vec<bool>), (Vec<Row>, Vec<Row>)>::new();
    let mut donors_scanned = 0usize;
    let mut targets_scanned = 0usize;
    for region in regions(&root.join("out/gs1-en/full/claimed/manifest.json"))? {
        let (Some(address), Some(size), Some(symbol)) = (
            region["address"].as_u64(),
            region["size"].as_u64(),
            region["symbol"].as_str(),
        ) else {
            continue;
        };
        let Some(stem) = symbol.strip_prefix("Func_") else {
            continue;
        };
        let source = region["source"].as_str().map(str::to_owned);
        let size = size as usize;
        if size < minimum {
            continue;
        }
        if let Some((row, mask)) = row(&rom, address, size, stem, source) {
            donors_scanned += 1;
            groups.entry((size, mask)).or_default().0.push(row);
        }
    }
    for region in regions(&root.join("out/gs1-en/full/asm/manifest.json"))? {
        let retention = region["retention"].as_str().unwrap_or("");
        if !DEBT.contains(&retention) {
            continue;
        }
        let (Some(address), Some(size), Some(source)) = (
            region["address"].as_u64(),
            region["size"].as_u64(),
            region["source"].as_str(),
        ) else {
            continue;
        };
        if region["run_address"].as_u64() != Some(address) {
            continue;
        }
        let Some(stem) = Path::new(source)
            .file_stem()
            .and_then(|value| value.to_str())
        else {
            continue;
        };
        let size = size as usize;
        if size < minimum {
            continue;
        }
        if let Some((row, mask)) = row(&rom, address, size, stem, None) {
            targets_scanned += 1;
            groups.entry((size, mask)).or_default().1.push(row);
        }
    }
    let mut rows = Vec::new();
    let mut adoptable = Vec::new();
    let mut sealed_contradictions = Vec::new();
    for ((size, mask), (donors, targets)) in &groups {
        let core = mask.iter().filter(|masked| !**masked).count();
        if core < 4 || donors.is_empty() {
            continue;
        }
        for target in targets {
            let mut best = Vec::new();
            for donor in donors {
                let difference = donor
                    .bytes
                    .iter()
                    .zip(&target.bytes)
                    .zip(mask)
                    .filter(|((left, right), masked)| !**masked && left != right)
                    .count();
                if difference == 0 {
                    best.push(donor);
                }
            }
            if best.is_empty() {
                continue;
            }
            let donors_json = best
                .iter()
                .take(3)
                .map(|donor| {
                    let mapping = match reference_map(&donor.references, &target.references) {
                        Ok(map) => map,
                        Err(error) => format!("map-error[{error}]"),
                    };
                    if donor.ordinary && !mapping.starts_with("map-error") {
                        adoptable.push(format!("{} from {}", target.stem, donor.stem));
                        if sealed.iter().any(|owner| owner == &target.stem) {
                            sealed_contradictions.push(target.stem.clone());
                        }
                    }
                    json!({
                        "donor": donor.stem,
                        "source": donor.source,
                        "ordinary": donor.ordinary,
                        "mapping": mapping,
                    })
                })
                .collect::<Vec<_>>();
            rows.push(json!({
                "owner": target.stem,
                "span": size,
                "core": core,
                "donors": donors_json,
            }));
        }
    }
    if !sealed_contradictions.is_empty() {
        return Err(format!(
            "sealed owners have ordinary exact twins; revisit the seals: {}",
            sealed_contradictions.join(", ")
        ));
    }
    if !adoptable.is_empty() {
        return Err(format!(
            "byte-exact ordinary twins must be adopted, not recorded: {}",
            adoptable.join(", ")
        ));
    }
    let report = json!({
        "format": 1,
        "kind": "main-image-shape-correspondence",
        "scope": "relocation-masked byte identity between exact-C claimed regions and retained \
                  debt assembly; shape correspondence only, never original translation units",
        "minimum_span": minimum,
        "donors_scanned": donors_scanned,
        "targets_scanned": targets_scanned,
        "rows": rows,
    });
    let text = serde_json::to_string_pretty(&report).map_err(|error| error.to_string())? + "\n";
    match (write, check) {
        (Some(path), None) => {
            fs::write(&path, &text).map_err(|error| format!("{path}: {error}"))?;
            println!(
                "main-twins={path} donors={donors_scanned} targets={targets_scanned} rows={}",
                rows.len()
            );
            Ok(())
        }
        (None, Some(path)) => {
            let tracked = fs::read_to_string(&path).map_err(|error| format!("{path}: {error}"))?;
            if tracked != text {
                return Err(format!("{path} is stale; run `make twins`"));
            }
            println!(
                "main-twins ok: donors={donors_scanned} targets={targets_scanned} rows={}",
                rows.len()
            );
            Ok(())
        }
        _ => Err(USAGE.into()),
    }
}
