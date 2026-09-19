//! Physical ROM index. Identification never grants reconstruction credit.
use crate::compiler::{canonical_json::canonical_json, sha256};
use crate::coverage::model::{normalize, subtract, Span};
use crate::coverage::tree::SourceTree;
use crate::overlay::rom::CanonicalRom;
use crate::targets::{target_for, DecompTarget, DecompTargetId};
use serde_json::{json, Value};
use std::{collections::BTreeMap, path::Path};

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
        let Ok(stream) = rom.stream(id) else { continue };
        let digest = sha256::hex(&stream.decoded);
        let Some(Some(kind)) = kinds.get(&digest) else {
            continue;
        };
        if kind == "golden-sun-character-graphics" {
            character_anchors.push(id);
        }
        let encoded = stream.encoded()?;
        rows.push(json!({
            "start":0x0800_0000 + stream.start as i64,
            "end":0x0800_0000 + (stream.start + encoded.len()) as i64,
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
pub(crate) fn current(tree: &SourceTree, target: &str) -> Option<Value> {
    let doc: Value =
        serde_json::from_str(&tree.read(&format!("out/{target}/reports/rom-index.json"))?).ok()?;
    if doc["format"] != "alchemy-rom-index-v1" || doc["target"] != target {
        return None;
    }
    for (path, digest) in doc["inputs"].as_object()? {
        if sha256::hex(tree.read(path)?.as_bytes()) != digest.as_str()? {
            return None;
        }
    }
    Some(doc)
}
pub(super) fn run(root: &Path, target: DecompTarget) -> Result<String, String> {
    let rom = CanonicalRom::load_target(root, target)?;
    let game = target.game_dir();
    let source_path = format!("{game}/SOURCE.JSON");
    let source = read(root, &source_path)?;
    let hash = sha256::hex(rom.bytes());
    if source["reference_sha256"] != hash {
        return Err("ROM does not match SOURCE.JSON checksum".into());
    }
    let inventory_path = format!("{game}/metrics/executable.json");
    let inventory = read(root, &inventory_path)?;
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
        "games/THE BROKEN SEAL/SOURCE.JSON".into(),
        format!("{game}/recon/translation-units.json"),
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
        std::fs::write(dir.path().join("input.json"), "changed").unwrap();
        assert!(current(&tree, "tla-en").is_none());
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
