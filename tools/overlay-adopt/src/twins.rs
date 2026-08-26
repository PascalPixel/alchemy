use crate::park::placeholder_block;
use compiler_core::source_paths::{SourceOwner, SourcePaths};
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
#[derive(Clone, Copy, PartialEq, Eq)]
struct Reference(u8, usize, usize, u32);
fn relocation_info(bytes: &[u8], offset: usize) -> (Vec<bool>, Vec<Reference>) {
    let mut mask = vec![false; bytes.len()];
    let mut references = Vec::new();
    for at in (0..bytes.len().saturating_sub(3)).step_by(2) {
        let high = u16::from_le_bytes([bytes[at], bytes[at + 1]]);
        let low = u16::from_le_bytes([bytes[at + 2], bytes[at + 3]]);
        if high & 0xf800 == 0xf000 && low & 0xf800 == 0xf800 {
            mask[at..at + 4].fill(true);
            let delta =
                (((i32::from(high & 0x7ff) << 21) >> 9) | (i32::from(low & 0x7ff) << 1)) as i64;
            references.push(Reference(
                b'B',
                at,
                at,
                (OVERLAY_BASE + offset as i64 + at as i64 + 4 + delta) as u32,
            ));
        }
    }
    for at in (0..bytes.len().saturating_sub(1)).step_by(2) {
        let instruction = u16::from_le_bytes([bytes[at], bytes[at + 1]]);
        if instruction & 0xf800 != 0x4800 {
            continue;
        }
        let pc = (OVERLAY_BASE as usize + offset + at + 4) & !3;
        let target = pc + usize::from(instruction & 0xff) * 4;
        let Some(literal) = target.checked_sub(OVERLAY_BASE as usize + offset) else {
            continue;
        };
        if literal + 4 <= mask.len() {
            mask[literal..literal + 4].fill(true);
            references.push(Reference(
                b'L',
                at,
                literal,
                u32::from_le_bytes(bytes[literal..literal + 4].try_into().unwrap()),
            ));
        }
    }
    (mask, references)
}
fn core_difference(left: &Row, right: &Row, mask: &[bool]) -> usize {
    left.bytes
        .iter()
        .zip(&right.bytes)
        .zip(mask)
        .filter(|((left, right), masked)| !**masked && left != right)
        .count()
}
fn bind(map: &mut BTreeMap<u32, u32>, key: u32, value: u32) -> Result<(), ()> {
    match map.insert(key, value) {
        Some(previous) if previous != value => Err(()),
        _ => Ok(()),
    }
}
fn reference_map(donor: &[Reference], target: &[Reference]) -> Result<String, &'static str> {
    if donor.len() != target.len()
        || donor
            .iter()
            .zip(target)
            .any(|(donor, target)| (donor.0, donor.1, donor.2) != (target.0, target.1, target.2))
    {
        return Err("topology");
    }
    let mut forward = BTreeMap::new();
    let mut reverse = BTreeMap::new();
    let memory = |value: u32| matches!(value >> 24, 0x02..=0x0e);
    for (donor, target) in donor.iter().zip(target) {
        match donor.0 {
            b'L' if donor.3 == target.3 && !memory(donor.3) => continue,
            b'L' if !(memory(donor.3) && memory(target.3)) => return Err("unmapped-literal"),
            b'B' if !(memory(donor.3) && memory(target.3)) => return Err("unmapped-call"),
            _ => {}
        }
        bind(&mut forward, donor.3, target.3).map_err(|_| "conflicting-donor")?;
        bind(&mut reverse, target.3, donor.3).map_err(|_| "conflicting-target")?;
    }
    Ok(forward
        .iter()
        .map(|(donor, target)| format!("{donor:08x}>{target:08x}"))
        .collect::<Vec<_>>()
        .join(","))
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
        let assembly_path = root.join(format!("games/gs1/assets/code/{overlay}_overlay.s"));
        let assembly = fs::read_to_string(&assembly_path)
            .map_err(|error| format!("{}: {error}", assembly_path.display()))?;
        let lines = assembly.lines().collect::<Vec<_>>();
        for owner in sources {
            let address = i64::from(owner.address());
            let (_, _, span) = placeholder_block(&lines, address)
                .ok_or_else(|| format!("{} has exact C but no overlay placeholder", owner.id()))?;
            let span = usize::try_from(span)
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
        let (mask, references) = relocation_info(&bytes, offset);
        if mask.iter().filter(|masked| !**masked).count() < 4 {
            continue;
        }
        let exact_source = source_paths
            .mapped_source_path(owner)
            .filter(|path| path.exists());
        let is_ordinary = exact_source
            .as_deref()
            .map(|path| crate::ordinary_source(root, path))
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
#[cfg(test)]
mod tests {
    use super::{reference_map, relocation_info, Reference};
    #[test]
    fn masks_thumb_calls_and_reached_literals() {
        let bytes = [0x00, 0xf0, 0x00, 0xf8, 0x00, 0x48, 0x70, 0x47, 1, 2, 3, 4];
        let (mask, references) = relocation_info(&bytes, 0);
        assert_eq!(
            mask,
            [true, true, true, true, false, false, false, false, true, true, true, true]
        );
        let core = mask.iter().filter(|masked| !**masked).count();
        assert_eq!(core, 4);
        assert_eq!(references.len(), 2);
        assert_eq!(references[0].3, 0x0200_0004);
        assert_eq!(references[1].3, 0x0403_0201);
    }
    #[test]
    fn requires_bijective_reference_mapping() {
        let reference = |at, value| Reference(b'B', at, at, value);
        let rejects = |donor: &[Reference], target: &[Reference], error| {
            assert_eq!(reference_map(donor, target), Err(error));
        };
        let donor = [reference(0, 0x0200_0010), reference(4, 0x0200_0010)];
        rejects(
            &donor,
            &[reference(0, 0x0200_0020), reference(4, 0x0200_0030)],
            "conflicting-donor",
        );
        rejects(
            &[reference(0, 0x0200_0010), reference(4, 0x0200_0030)],
            &[reference(0, 0x0200_0020), reference(4, 0x0200_0020)],
            "conflicting-target",
        );
        rejects(
            &[reference(0, 0x0200_0010)],
            &[reference(2, 0x0200_0020)],
            "topology",
        );
        let literal = Reference(b'L', 0, 4, 42);
        rejects(
            &[literal],
            &[Reference(literal.0, literal.1, literal.2, 43)],
            "unmapped-literal",
        );
    }
    #[test]
    fn nonordinary_donor_is_evidence_only() {
        let path = std::env::temp_dir().join("alchemy-twin-nonordinary.c");
        std::fs::write(&path, "void f(void) __attribute__((naked));\n").unwrap();
        assert!(!crate::ordinary_source(&path, &path).unwrap());
        let _ = std::fs::remove_file(path);
    }
}
