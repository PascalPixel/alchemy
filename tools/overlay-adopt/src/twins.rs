use crate::park::placeholder_block;
use compiler_core::source_paths::{SourceOwner, SourcePaths};
use overlay_disasm::OVERLAY_BASE;
use serde_json::Value;
use std::collections::BTreeMap;
use std::fs;
use std::path::{Path, PathBuf};

const USAGE: &str = "usage: overlay twins [--min BYTES] [--max-diff BYTES]\n\
Find unresolved overlay owners whose relocation-free shape is shared by exact C.";
const ROM_BASE: usize = 0x0800_0000;

struct Options {
    minimum: usize,
    maximum_difference: usize,
}

#[derive(Clone)]
struct Row {
    id: String,
    span: usize,
    exact_source: Option<PathBuf>,
    bytes: Vec<u8>,
    mask: Vec<bool>,
    core_bytes: usize,
}

fn number(row: &Value, field: &str) -> Result<usize, String> {
    row.get(field)
        .and_then(Value::as_u64)
        .and_then(|value| usize::try_from(value).ok())
        .filter(|value| *value > 0)
        .ok_or_else(|| format!("overlay region has no positive {field}"))
}

fn text<'a>(row: &'a Value, field: &str) -> Result<&'a str, String> {
    row.get(field)
        .and_then(Value::as_str)
        .ok_or_else(|| format!("overlay region has no {field}"))
}

fn address(row: &Value) -> Result<i64, String> {
    let value = text(row, "entry")?;
    i64::from_str_radix(value.trim_start_matches("0x"), 16)
        .map_err(|_| format!("invalid overlay entry {value:?}"))
}

fn relocation_mask(bytes: &[u8], offset: usize) -> Vec<bool> {
    let mut mask = vec![false; bytes.len()];
    for at in (0..bytes.len().saturating_sub(3)).step_by(2) {
        let high = u16::from_le_bytes([bytes[at], bytes[at + 1]]);
        let low = u16::from_le_bytes([bytes[at + 2], bytes[at + 3]]);
        if high & 0xf800 == 0xf000 && low & 0xf800 == 0xf800 {
            mask[at..at + 4].fill(true);
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
        }
    }
    mask
}

fn mask_key(mask: &[bool]) -> Vec<u8> {
    mask.iter().map(|masked| u8::from(*masked)).collect()
}

fn core_difference(left: &Row, right: &Row) -> usize {
    debug_assert_eq!(left.mask, right.mask);
    left.bytes
        .iter()
        .zip(&right.bytes)
        .zip(&left.mask)
        .filter(|((left, right), masked)| !**masked && left != right)
        .count()
}

pub(crate) fn resource_table(rom: &[u8]) -> Result<usize, String> {
    (0..rom.len().saturating_sub(8))
        .step_by(4)
        .find(|offset| {
            u32::from_le_bytes(rom[*offset..*offset + 4].try_into().unwrap()) as usize == ROM_BASE
                && u32::from_le_bytes(rom[*offset + 4..*offset + 8].try_into().unwrap()) as usize
                    == ROM_BASE + *offset
        })
        .ok_or("resource directory self-pointer was not found".into())
}

fn resource_pointer(rom: &[u8], table: usize, resource: usize) -> Result<usize, String> {
    let at = table
        .checked_add(resource * 4)
        .ok_or("resource directory offset overflow")?;
    let address = u32::from_le_bytes(
        rom.get(at..at + 4)
            .ok_or("resource directory extends past ROM")?
            .try_into()
            .unwrap(),
    ) as usize;
    address
        .checked_sub(ROM_BASE)
        .filter(|offset| *offset < rom.len())
        .ok_or_else(|| format!("resource {resource:03x} points outside ROM"))
}

pub(crate) fn decode_overlay(rom: &[u8], table: usize, overlay: &str) -> Result<Vec<u8>, String> {
    let resource = overlay
        .strip_prefix("resource_")
        .and_then(|value| usize::from_str_radix(value, 16).ok())
        .ok_or_else(|| format!("invalid overlay name {overlay:?}"))?;
    let start = resource_pointer(rom, table, resource)?;
    let end = resource_pointer(rom, table, resource + 1)
        .ok()
        .filter(|end| *end > start)
        .unwrap_or(rom.len());
    let (bytes, _) = match rom[start] {
        0 => extract_resource::decode_general(rom, start, end, 0x10_0000),
        1 => extract_resource::decode_palette(rom, start + 1, end, 0x10_0000),
        tag => return Err(format!("resource {resource:03x} has tag {tag}")),
    }
    .map_err(|error| error.0)?;
    Ok(bytes)
}

fn parse_options(argv: &[String]) -> Result<Option<Options>, String> {
    if argv
        .iter()
        .any(|argument| matches!(argument.as_str(), "-h" | "--help"))
    {
        println!("{USAGE}");
        return Ok(None);
    }
    let mut options = Options {
        minimum: 4,
        maximum_difference: 0,
    };
    let mut index = 0usize;
    while index < argv.len() {
        let flag = &argv[index];
        let value = argv.get(index + 1).ok_or_else(|| USAGE.to_string())?;
        match flag.as_str() {
            "--min" => {
                options.minimum = value
                    .parse::<usize>()
                    .ok()
                    .filter(|value| *value > 0)
                    .ok_or_else(|| "--min wants a positive decimal byte count".to_string())?;
            }
            "--max-diff" => {
                options.maximum_difference = value
                    .parse::<usize>()
                    .map_err(|_| "--max-diff wants a decimal byte count".to_string())?;
            }
            _ => return Err(USAGE.to_string()),
        }
        index += 2;
    }
    Ok(Some(options))
}

pub fn run(root: &Path, argv: &[String]) -> Result<i32, String> {
    let Some(options) = parse_options(argv)? else {
        return Ok(0);
    };
    let regions_path = root.join("games/gs1/semantic/regions.json");
    let document: Value = serde_json::from_str(
        &fs::read_to_string(&regions_path)
            .map_err(|error| format!("{}: {error}", regions_path.display()))?,
    )
    .map_err(|error| format!("{}: {error}", regions_path.display()))?;
    let regions = document["manual_regions"]
        .as_array()
        .ok_or("semantic regions has no manual_regions array")?;
    let source_paths = SourcePaths::load(root)?;
    let rom_path = root.join("roms/gs1-en.gba");
    let rom = fs::read(&rom_path).map_err(|error| format!("{}: {error}", rom_path.display()))?;
    let resource_table = resource_table(&rom)?;
    let mut images = BTreeMap::<String, Vec<u8>>::new();
    let mut rows = BTreeMap::<String, Row>::new();

    for region in regions {
        let overlay = text(region, "overlay")?.to_string();
        let address = address(region)?;
        let span = number(region, "span_bytes")?;
        if span < options.minimum {
            continue;
        }
        let owner = SourceOwner::parse(&format!("{overlay}:{address:08x}"))?;
        let exact_source = source_paths
            .mapped_source_path(owner)
            .filter(|path| path.exists());
        if !images.contains_key(&overlay) {
            let image = decode_overlay(&rom, resource_table, &overlay)
                .map_err(|error| format!("{overlay}: {error}"))?;
            images.insert(overlay.clone(), image);
        }
        let image = &images[&overlay];
        let offset = usize::try_from(address - OVERLAY_BASE)
            .map_err(|_| format!("{} is below the overlay base", owner.id()))?;
        let bytes = image
            .get(offset..offset + span)
            .ok_or_else(|| format!("{} extends past decoded {overlay}", owner.id()))?
            .to_vec();
        let mask = relocation_mask(&bytes, offset);
        let core_bytes = mask.iter().filter(|masked| !**masked).count();
        if core_bytes < 4 {
            continue;
        }
        let id = owner.id();
        rows.insert(
            id.clone(),
            Row {
                id,
                span,
                exact_source,
                bytes,
                mask,
                core_bytes,
            },
        );
    }

    let mut exact_by_overlay = BTreeMap::<String, Vec<(SourceOwner, PathBuf)>>::new();
    for source in source_paths.all_sources()? {
        if let Some(overlay) = source.owner.overlay_id() {
            exact_by_overlay
                .entry(overlay)
                .or_default()
                .push((source.owner, source.path));
        }
    }
    for (overlay, sources) in exact_by_overlay {
        if !images.contains_key(&overlay) {
            let image = decode_overlay(&rom, resource_table, &overlay)
                .map_err(|error| format!("{overlay}: {error}"))?;
            images.insert(overlay.clone(), image);
        }
        let assembly_path = root.join(format!("games/gs1/assets/code/{overlay}_overlay.s"));
        let assembly = fs::read_to_string(&assembly_path)
            .map_err(|error| format!("{}: {error}", assembly_path.display()))?;
        let lines = assembly.lines().collect::<Vec<_>>();
        for (owner, source) in sources {
            let address = i64::from(owner.address());
            let (_, _, span) = placeholder_block(&lines, address)
                .ok_or_else(|| format!("{} has exact C but no overlay placeholder", owner.id()))?;
            let span = usize::try_from(span)
                .ok()
                .filter(|span| *span > 0)
                .ok_or_else(|| format!("{} has an invalid placeholder span", owner.id()))?;
            if span < options.minimum {
                continue;
            }
            let offset = usize::try_from(address - OVERLAY_BASE)
                .map_err(|_| format!("{} is below the overlay base", owner.id()))?;
            let bytes = images[&overlay]
                .get(offset..offset + span)
                .ok_or_else(|| format!("{} extends past decoded {overlay}", owner.id()))?
                .to_vec();
            let mask = relocation_mask(&bytes, offset);
            let core_bytes = mask.iter().filter(|masked| !**masked).count();
            if core_bytes < 4 {
                continue;
            }
            let id = owner.id();
            rows.insert(
                id.clone(),
                Row {
                    id,
                    span,
                    exact_source: Some(source),
                    bytes,
                    mask,
                    core_bytes,
                },
            );
        }
    }

    let mut groups = BTreeMap::<Vec<u8>, Vec<Row>>::new();
    for row in rows.into_values() {
        groups.entry(mask_key(&row.mask)).or_default().push(row);
    }

    let mut candidates = Vec::<(usize, usize, String)>::new();
    for rows in groups.values() {
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
                .map(|row| (core_difference(unresolved, row), *row))
                .filter(|(difference, _)| *difference <= options.maximum_difference)
                .collect::<Vec<_>>();
            ranked.sort_by(|left, right| {
                let left_path = left.1.exact_source.as_ref().expect("filtered exact source");
                let right_path = right
                    .1
                    .exact_source
                    .as_ref()
                    .expect("filtered exact source");
                left.0
                    .cmp(&right.0)
                    .then_with(|| {
                        left_path
                            .to_string_lossy()
                            .contains("/unidentified/")
                            .cmp(&right_path.to_string_lossy().contains("/unidentified/"))
                    })
                    .then_with(|| left_path.cmp(right_path))
            });
            let Some(best_difference) = ranked.first().map(|candidate| candidate.0) else {
                continue;
            };
            let mut seen = std::collections::BTreeSet::new();
            let sources = ranked
                .into_iter()
                .filter(|(difference, _)| *difference == best_difference)
                .filter_map(|(_, row)| {
                    let path = row.exact_source.as_ref().expect("filtered exact source");
                    seen.insert(path.clone()).then(|| {
                        let raw = usize::from(row.bytes == unresolved.bytes);
                        let shown = path.strip_prefix(root).unwrap_or(path);
                        format!("{}={}#raw{raw}", row.id, shown.display())
                    })
                })
                .take(3)
                .collect::<Vec<_>>()
                .join(",");
            candidates.push((
                best_difference,
                unresolved.span,
                format!(
                    "{}\tspan={}\tcore={}\tdiff={}\texact={sources}",
                    unresolved.id, unresolved.span, unresolved.core_bytes, best_difference
                ),
            ));
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
    use super::relocation_mask;

    #[test]
    fn masks_thumb_calls_and_reached_literals() {
        let bytes = [0x00, 0xf0, 0x00, 0xf8, 0x00, 0x48, 0x70, 0x47, 1, 2, 3, 4];
        let mask = relocation_mask(&bytes, 0);
        assert_eq!(
            mask,
            [true, true, true, true, false, false, false, false, true, true, true, true]
        );
        let core = mask.iter().filter(|masked| !**masked).count();
        assert_eq!(core, 4);
    }
}
