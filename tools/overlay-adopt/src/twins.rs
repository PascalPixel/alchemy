use crate::{overlay_assembly, park::placeholder_block};
use compiler_core::source_paths::{SourceOwner, SourcePaths};
use compiler_core::thumb::{reference_map, relocation_info, Reference};
use overlay_disasm::{decode_overlay, resource_table, OVERLAY_BASE};
use std::collections::BTreeMap;
use std::fs;
use std::path::{Path, PathBuf};
const USAGE: &str = "usage: overlay twins [--min BYTES] [--max-diff BYTES]\n\
Find unresolved overlay owners whose relocation-free shape is shared by exact C.";
struct Row {
    id: String,
    mapped_name: Option<String>,
    exact_source: Option<PathBuf>,
    ordinary: bool,
    bytes: Vec<u8>,
    references: Vec<Reference>,
}
fn core_difference(left: &Row, right: &Row, mask: &[bool]) -> usize {
    left.bytes
        .iter()
        .zip(&right.bytes)
        .zip(mask)
        .filter(|((left, right), masked)| !**masked && left != right)
        .count()
}
fn parse_options(argv: &[String]) -> Result<Option<(usize, usize)>, String> {
    if argv
        .iter()
        .any(|arg| matches!(arg.as_str(), "-h" | "--help"))
    {
        println!("{USAGE}");
        return Ok(None);
    }
    if !argv.len().is_multiple_of(2) {
        return Err(USAGE.into());
    }
    let (mut minimum, mut maximum_difference) = (4, 0);
    for pair in argv.chunks_exact(2) {
        let value = pair[1]
            .parse()
            .map_err(|_| format!("{} wants a decimal byte count", pair[0]))?;
        match pair[0].as_str() {
            "--min" if value > 0 => minimum = value,
            "--min" => return Err("--min wants a positive decimal byte count".into()),
            "--max-diff" => maximum_difference = value,
            _ => return Err(USAGE.to_string()),
        }
    }
    Ok(Some((minimum, maximum_difference)))
}
pub fn run(root: &Path, argv: &[String]) -> Result<i32, String> {
    let Some((minimum, maximum_difference)) = parse_options(argv)? else {
        return Ok(0);
    };
    let source_paths = SourcePaths::load(root)?;
    let mut spans = crate::reviewed_spans(root)?;
    let mut exact_by_overlay = BTreeMap::<String, Vec<SourceOwner>>::new();
    for source in source_paths.all_sources()? {
        if let Some(overlay) = source.owner.overlay_id() {
            exact_by_overlay
                .entry(overlay)
                .or_default()
                .push(source.owner);
        }
    }
    for (overlay, sources) in exact_by_overlay {
        let assembly_path = overlay_assembly(root, &overlay);
        let assembly = fs::read_to_string(&assembly_path)
            .map_err(|error| format!("{}: {error}", assembly_path.display()))?;
        let lines = assembly.lines().collect::<Vec<_>>();
        for owner in sources {
            let address = i64::from(owner.address());
            let span = placeholder_block(&lines, address)
                .ok_or_else(|| format!("{} has exact C but no overlay placeholder", owner.id()))?;
            let span = usize::try_from(span.span)
                .ok()
                .filter(|span| *span > 0)
                .ok_or_else(|| format!("{} has an invalid placeholder span", owner.id()))?;
            spans.insert(owner, span);
        }
    }
    let rom_path = root.join("roms/gs1-en.gba");
    let rom = fs::read(&rom_path).map_err(|error| format!("{}: {error}", rom_path.display()))?;
    let resource_table = resource_table(&rom)?;
    let mut images = BTreeMap::<String, Vec<u8>>::new();
    let mut groups = BTreeMap::<Vec<bool>, Vec<Row>>::new();
    for (owner, span) in spans {
        if span < minimum {
            continue;
        }
        let overlay = owner
            .overlay_id()
            .expect("only overlay owners are collected");
        if !images.contains_key(&overlay) {
            images.insert(
                overlay.clone(),
                decode_overlay(&rom, resource_table, &overlay)
                    .map_err(|error| format!("{overlay}: {error}"))?,
            );
        }
        let offset = usize::try_from(i64::from(owner.address()) - OVERLAY_BASE)
            .map_err(|_| format!("{} is below the overlay base", owner.id()))?;
        let bytes = images[&overlay]
            .get(offset..offset + span)
            .ok_or_else(|| format!("{} extends past decoded {overlay}", owner.id()))?
            .to_vec();
        let (mask, references) = relocation_info(&bytes, OVERLAY_BASE as u64 + offset as u64);
        if mask.iter().filter(|masked| !**masked).count() < 4 {
            continue;
        }
        let exact_source = source_paths
            .mapped_source_path(owner)
            .filter(|path| path.exists());
        let is_ordinary = exact_source
            .as_deref()
            .map(|path| no_asm_c::ordinary_source(root, path))
            .transpose()?
            .unwrap_or(true);
        groups.entry(mask).or_default().push(Row {
            id: owner.id(),
            mapped_name: source_paths.registered_name(owner).map(str::to_owned),
            exact_source,
            ordinary: is_ordinary,
            bytes,
            references,
        });
    }
    let mut candidates = Vec::<(usize, usize, String)>::new();
    for (mask, rows) in &groups {
        let exact = rows
            .iter()
            .filter(|row| row.exact_source.is_some())
            .collect::<Vec<_>>();
        if exact.is_empty() {
            continue;
        }
        for unresolved in rows.iter().filter(|row| row.exact_source.is_none()) {
            let mut ranked = exact
                .iter()
                .map(|row| (core_difference(unresolved, row, mask), *row))
                .filter(|(difference, _)| *difference <= maximum_difference)
                .collect::<Vec<_>>();
            ranked.sort_by_key(|(difference, row)| {
                let path = row.exact_source.as_ref().expect("filtered exact source");
                (
                    *difference,
                    path.to_string_lossy().contains("/unidentified/"),
                    path.clone(),
                )
            });
            let Some(best_difference) = ranked.first().map(|candidate| candidate.0) else {
                continue;
            };
            let mapped = ranked
                .into_iter()
                .take_while(|(difference, _)| *difference == best_difference)
                .map(|(_, row)| (row, reference_map(&row.references, &unresolved.references)))
                .collect::<Vec<_>>();
            let has_valid = mapped
                .iter()
                .any(|(row, mapping)| row.ordinary && mapping.is_ok());
            let mut seen = std::collections::BTreeSet::new();
            let sources = mapped
                .iter()
                .filter(|(row, mapping)| !has_valid || row.ordinary && mapping.is_ok())
                .filter_map(|(row, mapping)| {
                    let path = row.exact_source.as_ref().expect("filtered exact source");
                    seen.insert(path.clone()).then(|| {
                        let mapping = mapping
                            .as_deref()
                            .map(|map| format!("map[{map}]"))
                            .unwrap_or_else(|error| format!("map-error[{error}]"));
                        format!(
                            "{}={}#raw{}#ordinary{}#{mapping}",
                            row.id,
                            path.strip_prefix(root).unwrap_or(path).display(),
                            usize::from(row.bytes == unresolved.bytes),
                            usize::from(row.ordinary)
                        )
                    })
                })
                .take(3)
                .collect::<Vec<_>>()
                .join(",");
            let name = unresolved.mapped_name.as_deref().unwrap_or("unregistered");
            let span = unresolved.bytes.len();
            let core = mask.iter().filter(|masked| !**masked).count();
            let line = format!(
                "{}\tname={name}\tspan={}\tcore={}\tdiff={}\texact={sources}",
                unresolved.id, span, core, best_difference
            );
            if unresolved.mapped_name.is_none() || !has_valid {
                println!("evidence-only\t{line}");
                continue;
            }
            candidates.push((best_difference, span, line));
        }
    }
    candidates.sort_by(|left, right| {
        left.0
            .cmp(&right.0)
            .then_with(|| right.1.cmp(&left.1))
            .then_with(|| left.2.cmp(&right.2))
    });
    let bytes: usize = candidates.iter().map(|candidate| candidate.1).sum();
    for (_, _, line) in &candidates {
        println!("{line}");
    }
    eprintln!(
        "twin_candidates={} candidate_bytes={} decoded_overlays={}",
        candidates.len(),
        bytes,
        images.len()
    );
    Ok(0)
}
