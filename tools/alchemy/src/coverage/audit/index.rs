//! Physical ROM index. Identification never grants reconstruction credit.
use crate::compiler::{canonical_json::canonical_json, sha256};
use crate::coverage::model::{normalize, subtract, Span};
use crate::coverage::tree::SourceTree;
use crate::overlay::rom::CanonicalRom;
use crate::targets::{target_for, DecompTarget, DecompTargetId};
use serde_json::{json, Value};
use std::{
    collections::{BTreeMap, HashMap, HashSet},
    path::Path,
};

fn read(root: &Path, path: &str) -> Result<Value, String> {
    serde_json::from_slice(&std::fs::read(root.join(path)).map_err(|e| format!("{path}: {e}"))?)
        .map_err(|e| format!("{path}: {e}"))
}
fn span(row: &Value) -> Option<Span> {
    Some(Span::new(row["start"].as_i64()?, row["end"].as_i64()?))
}
fn payload_kind(region: &Value) -> &str {
    if region["sources"].as_array().is_some_and(|sources| {
        sources.iter().any(|source| {
            source
                .as_str()
                .is_some_and(|path| path.contains("/GRAPHICS/CHARACTER/"))
        })
    }) {
        return "golden-sun-character-graphics";
    }
    if let Some(parts) = region["details"]["components"].as_array() {
        if parts.len() == 1 {
            return payload_kind(&parts[0]);
        }
        if !parts.is_empty() {
            return "mixed-data";
        }
    }
    region["kind"].as_str().unwrap_or("unresolved-data")
}
fn claim(
    row: &Value,
    covered: &mut Vec<Span>,
    rows: &mut Vec<Value>,
    limit: Span,
) -> Result<(), String> {
    let extent = span(row).ok_or("index row lacks extent")?;
    if extent.start < limit.start || extent.end > limit.end || extent.end <= extent.start {
        return Err(format!("index extent outside ROM: {row}"));
    }
    for part in subtract(&[extent], covered) {
        let mut row = row.clone();
        row["start"] = json!(part.start);
        row["end"] = json!(part.end);
        row["bytes"] = json!(part.bytes());
        rows.push(row);
    }
    covered.push(extent);
    *covered = normalize(covered);
    Ok(())
}

const EDITION_ANCHOR: usize = 64;

/// Maximal byte-identical runs between two editions of one game. Anchors are
/// unique in both images, so repeated padding and tables cannot invent a
/// correspondence. Every returned run is verified byte for byte.
fn exact_edition_runs(reference: &[u8], edition: &[u8]) -> Vec<(usize, usize, usize)> {
    if reference.len() < EDITION_ANCHOR || edition.len() < EDITION_ANCHOR {
        return vec![];
    }
    let mut reference_anchors = HashMap::<[u8; 16], Option<usize>>::new();
    for start in (0..=reference.len() - EDITION_ANCHOR).step_by(EDITION_ANCHOR) {
        let key: [u8; 16] = reference[start..start + 16].try_into().unwrap();
        reference_anchors
            .entry(key)
            .and_modify(|value| *value = None)
            .or_insert(Some(start));
    }
    let mut edition_anchors = HashMap::<[u8; 16], Option<usize>>::new();
    for start in (0..=edition.len() - EDITION_ANCHOR).step_by(4) {
        let key: [u8; 16] = edition[start..start + 16].try_into().unwrap();
        if reference_anchors.get(&key).is_some_and(Option::is_some) {
            edition_anchors
                .entry(key)
                .and_modify(|value| *value = None)
                .or_insert(Some(start));
        }
    }
    let mut anchors = reference_anchors
        .into_iter()
        .filter_map(|(key, reference_start)| {
            let reference_start = reference_start?;
            let edition_start = edition_anchors.get(&key).copied().flatten()?;
            (reference[reference_start..reference_start + EDITION_ANCHOR]
                == edition[edition_start..edition_start + EDITION_ANCHOR])
                .then_some((reference_start, edition_start))
        })
        .collect::<Vec<_>>();
    anchors.sort_by_key(|&(reference_start, edition_start)| (edition_start, reference_start));

    let mut chains = Vec::<(usize, usize, usize)>::new();
    for (reference_start, edition_start) in anchors {
        if let Some(last) = chains.last_mut() {
            if last.0 + last.2 == reference_start
                && last.1 + last.2 == edition_start
                && reference_start - last.0 == edition_start - last.1
            {
                last.2 += EDITION_ANCHOR;
                continue;
            }
        }
        chains.push((reference_start, edition_start, EDITION_ANCHOR));
    }
    for (reference_start, edition_start, length) in &mut chains {
        while *reference_start > 0
            && *edition_start > 0
            && reference[*reference_start - 1] == edition[*edition_start - 1]
        {
            *reference_start -= 1;
            *edition_start -= 1;
            *length += 1;
        }
        while *reference_start + *length < reference.len()
            && *edition_start + *length < edition.len()
            && reference[*reference_start + *length] == edition[*edition_start + *length]
        {
            *length += 1;
        }
    }
    chains.sort_by_key(|&(_, edition_start, _)| edition_start);
    let mut runs = Vec::<(usize, usize, usize)>::new();
    for run in chains {
        if let Some(last) = runs.last_mut() {
            let same_delta = last.0 as isize - last.1 as isize == run.0 as isize - run.1 as isize;
            if same_delta && run.1 <= last.1 + last.2 {
                last.2 = last.2.max(run.1 + run.2 - last.1);
                continue;
            }
            if run.1 < last.1 + last.2 {
                continue;
            }
        }
        runs.push(run);
    }
    runs
}

fn canonical_edition(target: DecompTarget) -> DecompTargetId {
    match target.id {
        DecompTargetId::TbsJa
        | DecompTargetId::TbsEn
        | DecompTargetId::TbsDe
        | DecompTargetId::TbsEs
        | DecompTargetId::TbsFr
        | DecompTargetId::TbsIt => DecompTargetId::TbsEn,
        DecompTargetId::TlaJa
        | DecompTargetId::TlaEn
        | DecompTargetId::TlaDe
        | DecompTargetId::TlaEs
        | DecompTargetId::TlaFr
        | DecompTargetId::TlaIt => DecompTargetId::TlaEn,
    }
}

fn cross_game_resource_twins(
    root: &Path,
    rom: &CanonicalRom,
    target: DecompTarget,
) -> Result<Vec<Value>, String> {
    if target.id != DecompTargetId::TlaEn {
        return Ok(vec![]);
    }
    let tbs_target = target_for(DecompTargetId::TbsEn);
    let tbs_rom = CanonicalRom::load_target(root, tbs_target)?;
    let index_path = format!("{}/reports/rom-index.json", tbs_target.output_dir);
    let tree = crate::coverage::tree::work_tree_at(root.to_path_buf());
    let index = current(&tree, "tbs-en")
        .ok_or("TBS ROM index is absent or stale; audit tbs-en before transferring twins")?;
    if index["rom_sha256"] != sha256::hex(tbs_rom.bytes()) {
        return Err("TBS ROM index is stale; audit tbs-en before transferring twins".into());
    }
    let mut kinds = BTreeMap::<String, Option<String>>::new();
    for row in index["regions"]
        .as_array()
        .ok_or("TBS ROM index lacks regions")?
    {
        let kind = payload_kind(row);
        if matches!(
            kind,
            "unresolved-data"
                | "compressed-resource"
                | "golden-sun-general-lz"
                | "golden-sun-kind2-lz"
        ) {
            continue;
        }
        let Some(extent) = span(row) else { continue };
        let start = (extent.start - 0x0800_0000) as usize;
        let end = (extent.end - 0x0800_0000) as usize;
        let Ok((decoded, _)) = crate::build_assets::tagged_extent(tbs_rom.bytes(), start, end)
        else {
            continue;
        };
        let digest = sha256::hex(&decoded);
        kinds
            .entry(digest)
            .and_modify(|found| {
                if found.as_deref() != Some(kind) {
                    *found = None;
                }
            })
            .or_insert_with(|| Some(kind.into()));
    }
    let mut rows = vec![];
    let mut character_anchors = vec![];
    for id in 0..rom.resource_count() {
        let Ok(start) = rom.resource_pointer(id) else {
            continue;
        };
        let end = rom
            .resource_pointer(id + 1)
            .ok()
            .filter(|end| *end > start)
            .unwrap_or_else(|| rom.bytes().len());
        let Ok((decoded, encoded)) = crate::build_assets::tagged_extent(rom.bytes(), start, end)
        else {
            continue;
        };
        let digest = sha256::hex(&decoded);
        let Some(Some(kind)) = kinds.get(&digest) else {
            continue;
        };
        if kind == "golden-sun-character-graphics" {
            character_anchors.push(id);
        }
        rows.push(json!({
            "start":0x0800_0000 + start as i64,
            "end":0x0800_0000 + (start + encoded) as i64,
            "kind":kind,
            "label":"Shared TBS/TLA resource payload",
            "resource":format!("{id:03x}"),
            "evidence":format!("decoded payload byte-identical to one uniquely typed by the current TBS ROM index ({digest}); {index_path}")
        }));
    }
    if character_anchors.len() >= 16 {
        character_anchors.sort_unstable();
        let anchors = character_anchors
            .iter()
            .copied()
            .collect::<std::collections::BTreeSet<_>>();
        let belongs = |id: usize| {
            rom.stream(id).is_ok_and(|stream| {
                stream.tag() == 0 && stream.encoded().is_ok_and(|encoded| encoded.len() >= 1024)
            })
        };
        let mut first = character_anchors[0];
        while first > 0 && belongs(first - 1) {
            first -= 1;
        }
        let mut last = *character_anchors.last().unwrap();
        while last + 1 < rom.resource_count() && belongs(last + 1) {
            last += 1;
        }
        for id in first..=last {
            if anchors.contains(&id) {
                continue;
            }
            let stream = rom.stream(id)?;
            let encoded = stream.encoded()?;
            rows.push(json!({
                "start":0x0800_0000 + stream.start as i64,
                "end":0x0800_0000 + (stream.start + encoded.len()) as i64,
                "kind":"golden-sun-character-graphics",
                "label":"TLA character graphics resource",
                "resource":format!("{id:03x}"),
                "evidence":format!("uninterrupted general-LZ resource family {first:03x}–{last:03x}, anchored by {} byte-identical TBS character-graphics payloads; family stops at codec/size boundary",character_anchors.len())
            }));
        }
    }
    Ok(rows)
}

fn unique_exact_kind(
    needle: &[u8],
    sibling: &[u8],
    matches: &[usize],
    typed: &[(usize, usize, &str)],
) -> Option<String> {
    let kind_at = |start: usize| {
        let index = typed.partition_point(|row| row.1 <= start);
        typed
            .get(index)
            .filter(|row| row.0 <= start && row.1 >= start + needle.len())
            .map(|row| row.2)
    };
    let kinds = matches
        .iter()
        .filter(|start| sibling.get(**start..**start + needle.len()) == Some(needle))
        .filter_map(|start| kind_at(*start))
        .filter(|kind| {
            !matches!(
                *kind,
                "unresolved-data"
                    | "compressed-resource"
                    | "golden-sun-general-lz"
                    | "golden-sun-kind2-lz"
            )
        })
        .collect::<HashSet<_>>();
    match kinds.into_iter().collect::<Vec<_>>().as_slice() {
        [kind] => Some((*kind).to_owned()),
        _ => None,
    }
}

fn cross_game_exact_windows(
    root: &Path,
    rom: &CanonicalRom,
    target: DecompTarget,
    covered: &[Span],
    limit: Span,
) -> Result<Vec<Value>, String> {
    // Sixty-four exact bytes make an accidental match vanishingly unlikely.
    // A match is useful only when every TBS occurrence has one audited
    // content type.
    const WINDOW: usize = 64;
    if target.id != DecompTargetId::TlaEn {
        return Ok(vec![]);
    }
    let gaps = subtract(&[limit], covered);
    let mut wanted = HashSet::new();
    for gap in &gaps {
        let begin = (gap.start - limit.start) as usize;
        let end = (gap.end - limit.start) as usize;
        for start in (begin.next_multiple_of(4)..=end.saturating_sub(WINDOW)).step_by(4) {
            wanted.insert(u64::from_le_bytes(
                rom.bytes()[start..start + 8].try_into().unwrap(),
            ));
        }
    }
    let tbs_target = target_for(DecompTargetId::TbsEn);
    let tbs_rom = CanonicalRom::load_target(root, tbs_target)?;
    let tree = crate::coverage::tree::work_tree_at(root.to_path_buf());
    let index = current(&tree, "tbs-en").ok_or("TBS ROM index is absent or stale")?;
    let typed = index["regions"]
        .as_array()
        .ok_or("TBS ROM index lacks regions")?
        .iter()
        .filter_map(|row| {
            let extent = span(row)?;
            Some((
                (extent.start - 0x0800_0000) as usize,
                (extent.end - 0x0800_0000) as usize,
                payload_kind(row),
            ))
        })
        .collect::<Vec<_>>();
    let mut candidates = HashMap::<u64, Vec<usize>>::new();
    for start in (0..=tbs_rom.bytes().len().saturating_sub(WINDOW)).step_by(4) {
        let key = u64::from_le_bytes(tbs_rom.bytes()[start..start + 8].try_into().unwrap());
        if wanted.contains(&key) {
            candidates.entry(key).or_default().push(start);
        }
    }
    let mut windows = vec![];
    for gap in gaps {
        let begin = (gap.start - limit.start) as usize;
        let end = (gap.end - limit.start) as usize;
        for start in (begin.next_multiple_of(4)..=end.saturating_sub(WINDOW)).step_by(4) {
            let key = u64::from_le_bytes(rom.bytes()[start..start + 8].try_into().unwrap());
            let Some(matches) = candidates.get(&key) else {
                continue;
            };
            if let Some(kind) = unique_exact_kind(
                &rom.bytes()[start..start + WINDOW],
                tbs_rom.bytes(),
                matches,
                &typed,
            ) {
                windows.push((start, start + WINDOW, kind));
            }
        }
    }
    windows.sort();
    let mut merged = Vec::<(usize, usize, String)>::new();
    for (start, end, kind) in windows {
        if let Some(last) = merged.last_mut() {
            if last.2 == kind && start <= last.1 {
                last.1 = last.1.max(end);
                continue;
            }
        }
        merged.push((start, end, kind));
    }
    Ok(merged
        .into_iter()
        .map(|(start, end, kind)| {
            json!({
                "start":0x0800_0000 + start as i64,
                "end":0x0800_0000 + end as i64,
                "kind":kind,
                "label":"Shared TBS/TLA data",
                "evidence":"every exact 64-byte TBS occurrence has one audited content type; full bytes compared at four-byte alignment"
            })
        })
        .collect())
}
/// Load the last generated index and report whether every recorded input still
/// matches. Dashboard callers may display stale data with an explicit warning;
/// verification callers must continue to use `current`.
pub(crate) fn available(tree: &SourceTree, target: &str) -> Option<(Value, bool)> {
    let doc: Value =
        serde_json::from_str(&tree.read(&format!("out/{target}/reports/rom-index.json"))?).ok()?;
    if doc["format"] != "alchemy-rom-index-v1" || doc["target"] != target {
        return None;
    }
    let current = doc["inputs"].as_object()?.iter().all(|(path, digest)| {
        tree.read(path).is_some_and(|text| {
            digest
                .as_str()
                .is_some_and(|expected| sha256::hex(text.as_bytes()) == expected)
        })
    });
    Some((doc, current))
}
pub(crate) fn current(tree: &SourceTree, target: &str) -> Option<Value> {
    available(tree, target).and_then(|(doc, current)| current.then_some(doc))
}

fn add_fill_and_unresolved(rom: &[u8], limit: Span, covered: &[Span], rows: &mut Vec<Value>) {
    for gap in subtract(&[limit], covered) {
        let begin = (gap.start - limit.start) as usize;
        let end = (gap.end - limit.start) as usize;
        let mut cursor = begin;
        let mut unresolved = begin;
        while cursor < end {
            let value = rom[cursor];
            let next = cursor
                + rom[cursor..end]
                    .iter()
                    .take_while(|byte| **byte == value)
                    .count();
            if next - cursor >= 32 && matches!(value, 0 | 255) {
                if unresolved < cursor {
                    rows.push(unresolved_row(
                        limit.start + unresolved as i64,
                        limit.start + cursor as i64,
                    ));
                }
                rows.push(json!({"start":limit.start+cursor as i64,"end":limit.start+next as i64,"bytes":next-cursor,"kind":"byte-fill","label":"Constant fill","evidence":format!("{} repeated bytes of 0x{value:02x}; purpose not inferred",next-cursor)}));
                unresolved = next;
            }
            cursor = next;
        }
        if unresolved < end {
            rows.push(unresolved_row(limit.start + unresolved as i64, gap.end));
        }
    }
}

fn write_index(
    root: &Path,
    target: DecompTarget,
    rom: &[u8],
    inputs: serde_json::Map<String, Value>,
    mut rows: Vec<Value>,
    failures: Vec<Value>,
    derivation: Value,
) -> Result<String, String> {
    let limit = Span::new(0x08000000, 0x08000000 + rom.len() as i64);
    rows.sort_by_key(|row| row["start"].as_i64().unwrap_or(i64::MAX));
    let mut cursor = limit.start;
    let mut summary = BTreeMap::<String, i64>::new();
    for row in &rows {
        let extent = span(row).ok_or("ROM index row lacks an extent")?;
        if extent.start != cursor || extent.end <= extent.start || extent.end > limit.end {
            return Err(format!(
                "ROM index is not an exact partition at {cursor:#010x}: {row}"
            ));
        }
        cursor = extent.end;
        *summary
            .entry(row["kind"].as_str().unwrap_or("unresolved-data").into())
            .or_default() += extent.bytes();
    }
    if cursor != limit.end {
        return Err("ROM index does not cover the complete cartridge".into());
    }
    let hash = sha256::hex(rom);
    let doc = json!({
        "format":"alchemy-rom-index-v1",
        "target":target.id.as_str(),
        "rom_sha256":hash,
        "rom_bytes":rom.len(),
        "inputs":inputs,
        "summary":summary,
        "regions":rows,
        "unresolved_readers":failures,
        "derivation":derivation,
    });
    let path = root.join(target.output_dir).join("reports/rom-index.json");
    std::fs::create_dir_all(path.parent().unwrap()).map_err(|error| error.to_string())?;
    std::fs::write(&path, canonical_json(&doc)).map_err(|error| error.to_string())?;
    Ok(format!(
        "{}: {} indexed bytes, {} intervals, {} unresolved bytes, {} reader issues; {}",
        target.id,
        rom.len(),
        rows.len(),
        summary.get("unresolved-data").unwrap_or(&0),
        failures.len(),
        path.display()
    ))
}

fn sibling_edition_index(
    root: &Path,
    target: DecompTarget,
    rom: &CanonicalRom,
) -> Result<String, String> {
    let canonical = target_for(canonical_edition(target));
    let canonical_rom = CanonicalRom::load_target(root, canonical)?;
    let tree = crate::coverage::tree::work_tree_at(root.to_path_buf());
    let canonical_index = current(&tree, canonical.id.as_str()).ok_or_else(|| {
        format!(
            "{} ROM index is absent or stale; audit it before {}",
            canonical.id, target.id
        )
    })?;
    if canonical_index["rom_sha256"] != sha256::hex(canonical_rom.bytes()) {
        return Err(format!(
            "{} ROM index has the wrong ROM digest",
            canonical.id
        ));
    }
    let limit = Span::new(0x08000000, 0x08000000 + rom.bytes().len() as i64);
    let canonical_rows = canonical_index["regions"]
        .as_array()
        .ok_or("canonical ROM index lacks regions")?;
    let runs = exact_edition_runs(canonical_rom.bytes(), rom.bytes());
    let mut covered = Vec::new();
    let mut rows = Vec::new();
    claim(
        &crate::text_catalog::archive_region(target.id.as_str(), rom.bytes())?,
        &mut covered,
        &mut rows,
        limit,
    )?;
    for (canonical_start, edition_start, length) in &runs {
        let canonical_extent = Span::new(
            0x08000000 + *canonical_start as i64,
            0x08000000 + (*canonical_start + *length) as i64,
        );
        for source in canonical_rows {
            let Some(source_extent) = span(source) else {
                continue;
            };
            let start = source_extent.start.max(canonical_extent.start);
            let end = source_extent.end.min(canonical_extent.end);
            if start >= end {
                continue;
            }
            let mut row = json!({
                "start":0x08000000 + *edition_start as i64 + start-canonical_extent.start,
                "end":0x08000000 + *edition_start as i64 + end-canonical_extent.start,
                "kind":source["kind"],
                "label":source["label"],
                "evidence":format!("byte-identical unique correspondence to {} at {start:#010x}–{end:#010x}",canonical.id),
                "canonical_target":canonical.id.as_str(),
                "canonical_start":start,
                "canonical_end":end,
            });
            if let Some(resource) = source.get("resource") {
                row["resource"] = resource.clone();
            }
            claim(&row, &mut covered, &mut rows, limit)?;
        }
    }
    // Independently recognize every locally reproducible compressed stream.
    // This proves its storage format, not its semantic payload.
    for id in 0..rom.resource_count() {
        let Ok(stream) = rom.stream(id) else { continue };
        let encoded = stream.encoded()?;
        if rom.bytes().get(stream.start..stream.start + encoded.len()) != Some(encoded.as_slice()) {
            continue;
        }
        claim(
            &json!({"start":limit.start+stream.start as i64,"end":limit.start+(stream.start+encoded.len()) as i64,"kind":"compressed-resource","label":"Compressed resource","resource":format!("{id:03x}"),"evidence":"edition-local resource directory; decoded and re-encoded stream; content role not yet established"}),
            &mut covered,
            &mut rows,
            limit,
        )?;
    }
    add_fill_and_unresolved(rom.bytes(), limit, &covered, &mut rows);
    let canonical_path = format!("{}/reports/rom-index.json", canonical.output_dir);
    let mut inputs = serde_json::Map::new();
    for path in [
        canonical_path,
        "tools/alchemy/src/coverage/audit/index.rs".into(),
        "tools/alchemy/src/overlay/rom.rs".into(),
        "tools/psynergy/src/assets/lz.rs".into(),
        "tools/alchemy/src/text_catalog.rs".into(),
        "tools/psynergy/src/assets/huffman_archive.rs".into(),
        "tools/psynergy/src/assets/huffman_archive/reader.rs".into(),
        format!("{}/text.json", target.recon_dir()),
    ] {
        inputs.insert(
            path.clone(),
            json!(sha256::hex(
                &std::fs::read(root.join(path)).map_err(|error| error.to_string())?
            )),
        );
    }
    write_index(
        root,
        target,
        rom.bytes(),
        inputs,
        rows,
        vec![],
        json!({
            "method":"edition-exact-correspondence-v1",
            "canonical_target":canonical.id.as_str(),
            "unique_anchor_bytes":EDITION_ANCHOR,
            "exact_runs":runs.len(),
            "rule":"semantic kinds transfer only over byte-identical runs anchored uniquely in both ROMs; unmatched bytes remain unresolved",
        }),
    )
}

pub(super) fn run(root: &Path, target: DecompTarget) -> Result<String, String> {
    let rom = CanonicalRom::load_target(root, target)?;
    if target.id != canonical_edition(target) {
        return sibling_edition_index(root, target, &rom);
    }
    let recon = target.recon_dir();
    let source_path = format!("{recon}/private-inputs.json");
    let source = read(root, &source_path)?;
    let hash = sha256::hex(rom.bytes());
    if source["reference_sha256"] != hash {
        return Err("ROM does not match private-inputs.json checksum".into());
    }
    let inventory_path = format!("{}/reports/executable.json", target.output_dir);
    let inventory = crate::coverage::pipeline::authoritative_inventory(root, target)?
        .ok_or_else(|| {
            format!(
                "{inventory_path} is absent, pending or not the independently verified automatic count; the ROM index places its executable image only from that count"
            )
        })?;
    let asset_path = [
        format!("{}/full/assets/manifest.json", target.output_dir),
        format!("{}/assets/manifest.json", target.output_dir),
    ]
    .into_iter()
    .find(|p| root.join(p).is_file())
    .ok_or("build assets before indexing the ROM")?;
    let assets = read(root, &asset_path)?;
    let limit = Span::new(0x08000000, 0x08000000 + rom.bytes().len() as i64);
    let mut covered = vec![];
    let mut rows = vec![];
    for region in inventory["main"]["intervals"]
        .as_array()
        .ok_or("missing executable intervals")?
    {
        claim(
            &json!({"start":region["start"],"end":region["end"],"kind":"executable","label":"Main executable image","evidence":inventory_path}),
            &mut covered,
            &mut rows,
            limit,
        )?;
    }
    for overlay in inventory["overlays"]
        .as_array()
        .ok_or("missing overlay inventory")?
    {
        let id =
            crate::overlay::rom::resource_id(overlay["id"].as_str().ok_or("overlay lacks id")?)?;
        let stream = rom.stream(id)?;
        let size = stream.encoded()?.len();
        claim(
            &json!({"start":limit.start+stream.start as i64,"end":limit.start+(stream.start+size) as i64,"kind":"encoded-overlay","label":overlay["id"],"evidence":inventory_path}),
            &mut covered,
            &mut rows,
            limit,
        )?;
    }
    for region in assets["regions"]
        .as_array()
        .ok_or("missing built asset regions")?
    {
        let start = region["address"].as_i64().ok_or("asset address missing")?;
        let size = region["size"].as_i64().ok_or("asset size missing")?;
        claim(
            &json!({"start":start,"end":start+size,"kind":payload_kind(region),"storage":region["kind"],"label":"Build-owned asset","sources":region["sources"],"evidence":asset_path,"reconstructed":true}),
            &mut covered,
            &mut rows,
            limit,
        )?;
    }
    let (identified, failures) = if subtract(&[limit], &covered).is_empty() {
        (vec![], vec![])
    } else {
        crate::build_assets::identified_regions(root, rom.bytes(), &target)?
    };
    for row in identified {
        claim(&row, &mut covered, &mut rows, limit)?;
    }
    for row in cross_game_resource_twins(root, &rom, target)? {
        claim(&row, &mut covered, &mut rows, limit)?;
    }
    for row in cross_game_exact_windows(root, &rom, target, &covered, limit)? {
        claim(&row, &mut covered, &mut rows, limit)?;
    }
    // Directory decodes establish compression, not content type.
    for id in 0..rom.resource_count() {
        let Ok(stream) = rom.stream(id) else { continue };
        let encoded = stream.encoded()?;
        if rom.bytes().get(stream.start..stream.start + encoded.len()) != Some(encoded.as_slice()) {
            continue;
        }
        claim(
            &json!({"start":limit.start+stream.start as i64,"end":limit.start+(stream.start+encoded.len()) as i64,"kind":"compressed-resource","label":"Compressed resource","resource":format!("{id:03x}"),"evidence":"resource directory; decoded and re-encoded stream; content role not yet established"}),
            &mut covered,
            &mut rows,
            limit,
        )?;
    }
    for gap in subtract(&[limit], &covered) {
        let begin = (gap.start - limit.start) as usize;
        let end = (gap.end - limit.start) as usize;
        let mut cursor = begin;
        let mut unresolved = begin;
        while cursor < end {
            let value = rom.bytes()[cursor];
            let next = cursor
                + rom.bytes()[cursor..end]
                    .iter()
                    .take_while(|b| **b == value)
                    .count();
            if next - cursor >= 32 && matches!(value, 0 | 255) {
                if unresolved < cursor {
                    rows.push(unresolved_row(
                        limit.start + unresolved as i64,
                        limit.start + cursor as i64,
                    ));
                }
                rows.push(json!({"start":limit.start+cursor as i64,"end":limit.start+next as i64,"bytes":next-cursor,"kind":"byte-fill","label":"Constant fill","evidence":format!("{} repeated bytes of 0x{value:02x}; purpose not inferred",next-cursor)}));
                unresolved = next;
            }
            cursor = next;
        }
        if unresolved < end {
            rows.push(unresolved_row(limit.start + unresolved as i64, gap.end));
        }
    }
    rows.sort_by_key(|row| row["start"].as_i64().unwrap());
    let mut cursor = limit.start;
    let mut summary = BTreeMap::<String, i64>::new();
    for row in &rows {
        let s = span(row).unwrap();
        if s.start != cursor {
            return Err("ROM index has a gap or duplicate bytes".into());
        }
        cursor = s.end;
        *summary
            .entry(row["kind"].as_str().unwrap().into())
            .or_default() += s.bytes();
    }
    if cursor != limit.end {
        return Err("ROM index does not cover the complete cartridge".into());
    }
    let mut inputs = serde_json::Map::new();
    for path in [
        source_path,
        inventory_path,
        asset_path,
        format!("{}/FIELD/COMMON/SCENE_TABLE.JSON", target.source_dir),
        "recon/tbs/private-inputs.json".into(),
        format!("{recon}/translation-units.json"),
        "tools/alchemy/src/build_assets.rs".into(),
        "tools/alchemy/src/coverage/audit/index.rs".into(),
        "tools/alchemy/src/build_assets/derive_index.rs".into(),
        "tools/alchemy/src/build_assets/native/character/catalog.rs".into(),
        "tools/psynergy/src/assets/lz.rs".into(),
    ] {
        inputs.insert(
            path.clone(),
            json!(sha256::hex(
                &std::fs::read(root.join(path)).map_err(|e| e.to_string())?
            )),
        );
    }
    if target.id == DecompTargetId::TlaEn {
        let path = "out/tbs-en/reports/rom-index.json";
        inputs.insert(
            path.into(),
            json!(sha256::hex(
                &std::fs::read(root.join(path)).map_err(|e| e.to_string())?
            )),
        );
    }
    let doc = json!({"format":"alchemy-rom-index-v1","target":target.id.as_str(),"rom_sha256":hash,"rom_bytes":rom.bytes().len(),"inputs":inputs,"summary":summary,"regions":rows,"unresolved_readers":failures});
    let path = root.join(target.output_dir).join("reports/rom-index.json");
    std::fs::create_dir_all(path.parent().unwrap()).map_err(|e| e.to_string())?;
    std::fs::write(&path, canonical_json(&doc)).map_err(|e| e.to_string())?;
    Ok(format!(
        "{}: {} indexed bytes, {} intervals, {} unresolved bytes, {} reader issues; {}",
        target.id,
        rom.bytes().len(),
        rows.len(),
        summary.get("unresolved-data").unwrap_or(&0),
        failures.len(),
        path.display()
    ))
}
fn unresolved_row(start: i64, end: i64) -> Value {
    json!({"start":start,"end":end,"bytes":end-start,"kind":"unresolved-data","label":"Data awaiting format identification","evidence":"exact complement of audited code, built assets and decoded formats; no content type asserted"})
}
#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn edition_correspondence_requires_unique_verified_anchors() {
        let mut reference = (0..=255u8).cycle().take(512).collect::<Vec<_>>();
        for (index, byte) in reference.iter_mut().enumerate() {
            *byte ^= (index / 64) as u8;
        }
        let mut edition = reference.clone();
        edition.splice(128..128, [0xaa, 0xbb, 0xcc, 0xdd]);
        edition[320] ^= 0x5a;
        let runs = exact_edition_runs(&reference, &edition);
        assert!(runs
            .iter()
            .any(|&(left, right, size)| { left == 0 && right == 0 && size >= 128 }));
        assert!(runs.iter().all(|&(left, right, size)| {
            reference[left..left + size] == edition[right..right + size]
        }));
        let repeated = vec![0; 512];
        assert!(exact_edition_runs(&repeated, &repeated).is_empty());
    }
    #[test]
    fn exact_sibling_bytes_transfer_only_one_audited_kind() {
        let sibling = [b'a'; 192];
        let one_kind = [(0, 64, "gba-4bpp-tiles"), (64, 128, "gba-4bpp-tiles")];
        assert_eq!(
            unique_exact_kind(&sibling[..64], &sibling, &[0, 64], &one_kind),
            Some("gba-4bpp-tiles".into())
        );
        let ambiguous = [(0, 64, "gba-4bpp-tiles"), (64, 128, "bgr555-banks")];
        assert_eq!(
            unique_exact_kind(&sibling[..64], &sibling, &[0, 64], &ambiguous),
            None
        );
        let unresolved = [(0, 64, "unresolved-data")];
        assert_eq!(
            unique_exact_kind(&sibling[..64], &sibling, &[0], &unresolved),
            None
        );
    }
    #[test]
    fn reconstructed_character_sources_own_the_payload_kind_not_the_codec() {
        let row = json!({
            "kind":"components",
            "sources":["games/THE LOST AGE/SRC/GRAPHICS/CHARACTER/CHAR_ROBIN.PNG"],
            "details":{"components":[{"kind":"golden-sun-general-lz"}]}
        });
        assert_eq!(payload_kind(&row), "golden-sun-character-graphics");
    }
    #[test]
    fn changed_inputs_invalidate_the_index() {
        let dir = tempfile::tempdir().unwrap();
        std::fs::create_dir_all(dir.path().join("out/tla-en/reports")).unwrap();
        std::fs::write(dir.path().join("input.json"), "first").unwrap();
        let doc = json!({"format":"alchemy-rom-index-v1","target":"tla-en","inputs":{"input.json":sha256::hex(b"first")}});
        std::fs::write(
            dir.path().join("out/tla-en/reports/rom-index.json"),
            doc.to_string(),
        )
        .unwrap();
        let tree = crate::coverage::tree::work_tree_at(dir.path().into());
        assert!(current(&tree, "tla-en").is_some());
        assert!(available(&tree, "tla-en").is_some_and(|(_, current)| current));
        std::fs::write(dir.path().join("input.json"), "changed").unwrap();
        assert!(current(&tree, "tla-en").is_none());
        assert!(available(&tree, "tla-en").is_some_and(|(_, current)| !current));
    }
    #[test]
    fn overlapping_evidence_counts_each_byte_once_and_rejects_bad_extents() {
        let mut covered = vec![];
        let mut rows = vec![];
        let limit = Span::new(0, 100);
        claim(
            &json!({"start":10,"end":30}),
            &mut covered,
            &mut rows,
            limit,
        )
        .unwrap();
        claim(
            &json!({"start":20,"end":40}),
            &mut covered,
            &mut rows,
            limit,
        )
        .unwrap();
        assert_eq!(rows.len(), 2);
        assert_eq!(rows[1]["start"], 30);
        assert!(claim(
            &json!({"start":99,"end":101}),
            &mut covered,
            &mut rows,
            limit
        )
        .is_err());
        assert_eq!(
            subtract(&[limit], &covered),
            vec![Span::new(0, 10), Span::new(40, 100)]
        );
    }
}
