use super::*;
mod character;
mod frame;
mod graphics;
mod identity;
mod portrait;
mod review;
mod review_defaults;
pub(super) use review::{export as export_review, export_field as export_field_review};
mod still;
mod tile;
mod tracking;
pub(super) use identity::audit as audit_characters;
use serde_json::json;
use std::collections::BTreeSet;
pub(super) use tracking::check as check_tracking;

use crate::targets::{target_for, DecompTarget, DecompTargetId};

/// Where one game keeps its native source index and the shared documents the
/// index's private inputs are extracted into.
pub(in crate::build_assets) struct NativePaths {
    pub index: String,
    pub colors: String,
    pub recipes: String,
    pub source: &'static str,
}
impl NativePaths {
    pub fn of(target: &DecompTarget) -> Self {
        let game = target.game_dir();
        Self {
            index: format!("{game}/SOURCE.JSON"),
            colors: format!("{}/GRAPHICS/COMMON/PALETTE.JSON", target.source_dir),
            recipes: format!("{}/GRAPHICS/COMMON/COMPRESSION.JSON", target.source_dir),
            source: target.source_dir,
        }
    }
}
/// The review sheets, atlases and UI frames are still Broken Seal documents.
pub(in crate::build_assets) fn broken_seal() -> NativePaths {
    NativePaths::of(&target_for(DecompTargetId::TbsEn))
}
/// One game per product: every edition of a game shares its source tree.
pub(in crate::build_assets) fn games() -> [DecompTarget; 2] {
    [
        target_for(DecompTargetId::TbsEn),
        target_for(DecompTargetId::TlaEn),
    ]
}

fn decode_buffer(input: &Value, rom: &[u8], fallback: usize) -> Result<Vec<u8>, String> {
    let start = address(&input["region_address"])?
        .checked_sub(ROM_BASE)
        .ok_or("buffer precedes ROM")?;
    let length = input
        .get("buffer_length")
        .map(address)
        .transpose()?
        .unwrap_or(fallback);
    let bytes = rom.get(start..).ok_or("buffer exceeds ROM")?;
    let decoded = match input["codec"].as_str().unwrap_or("golden-sun-general-lz") {
        "raw" => bytes
            .get(..length)
            .ok_or("raw buffer exceeds ROM")?
            .to_vec(),
        "general-lz" | "golden-sun-general-lz" => {
            psynergy::assets::lz::decode_general(rom, start, rom.len(), length as u64)
                .map_err(|e| e.to_string())?
                .0
        }
        "palette-lz" | "golden-sun-palette-lz" => {
            psynergy::assets::lz::decode_palette(rom, start, rom.len(), length as u64)
                .map_err(|e| e.to_string())?
                .0
        }
        "golden-sun-tagged-palette-lz" => {
            if bytes.first() != Some(&1) {
                return Err("palette buffer tag differs".into());
            }
            psynergy::assets::lz::decode_palette(rom, start + 1, rom.len(), length as u64)
                .map_err(|e| e.to_string())?
                .0
        }
        "golden-sun-halfword-lz" => {
            psynergy::assets::lz::decode_halfword(bytes)
                .map_err(|e| e.to_string())?
                .0
        }
        "golden-sun-general-lz-prefill" => {
            psynergy::assets::lz::decode_general_prefill_trace(
                rom,
                start,
                rom.len(),
                length as u64,
                address(&input["prefill"])?,
                input.get("header").map(address).transpose()?.unwrap_or(1),
            )
            .map_err(|e| e.to_string())?
            .0
        }
        _ => return Err("buffer codec requires a verified inverse".into()),
    };
    if decoded.len() != length {
        return Err("decoded buffer extent differs".into());
    }
    Ok(decoded)
}

pub(in crate::build_assets) fn write_source(
    root: &Path,
    name: &str,
    bytes: &[u8],
) -> Result<(), String> {
    write(root, name, bytes)
}
fn write(root: &Path, name: &str, bytes: &[u8]) -> Result<(), String> {
    let path = root_path(root, name)?;
    fs::create_dir_all(path.parent().ok_or("source has no parent")?).map_err(|e| e.to_string())?;
    fs::write(path, bytes).map_err(|e| e.to_string())
}
fn document(root: &Path, name: &str, value: &Value) -> Result<(), String> {
    write(
        root,
        name,
        format!("{}\n", canonical_json(value)).as_bytes(),
    )
}
fn address(value: &Value) -> Result<usize, String> {
    number(value, "resource address")
}

pub fn validate(index: &Value) -> Result<(), String> {
    if index["format"] != "camelot-style-golden-sun-native" {
        return Err("unsupported native source format".into());
    }
    let mut extents: BTreeMap<String, Vec<(usize, usize, Option<String>)>> = BTreeMap::new();
    for layout in index["layouts"].as_array().ok_or("missing layouts")? {
        let source = json_string(&layout["map"], "map source")?.to_string();
        let start = address(&layout["grid_offset"])?;
        if address(&layout["grid_length"])? != 65536 {
            return Err("invalid native grid length".into());
        }
        let middle = start.checked_add(65536).ok_or("grid extent overflows")?;
        if middle != address(&layout["metatile_offset"])? {
            return Err("grid and metatiles must be adjacent".into());
        }
        let length = address(&layout["metatile_length"])?;
        if length == 0 || length % 8 != 0 {
            return Err("native metatiles need complete 2x2 definitions".into());
        }
        let end = middle
            .checked_add(length)
            .ok_or("metatile extent overflows")?;
        let spans = extents.entry(source).or_default();
        let digest = layout
            .get("payload_sha256")
            .and_then(Value::as_str)
            .map(str::to_string);
        if spans.iter().any(|(a, b, existing)| {
            start < *b
                && *a < end
                && !(start == *a && end == *b && digest.is_some() && &digest == existing)
        }) {
            return Err("native map sections overlap".into());
        }
        spans.push((start, end, digest));
    }
    let mut addresses = BTreeSet::new();
    for region in index["regions"]
        .as_array()
        .ok_or("missing native regions")?
    {
        if !addresses.insert(address(&region["address"])?) {
            return Err("duplicate native region address".into());
        }
    }
    Ok(())
}

pub fn extract(root: &Path, rom_path: &Path, target: &DecompTarget) -> Result<(), String> {
    let paths = NativePaths::of(target);
    let index = json(&root.join(&paths.index))?;
    validate(&index)?;
    let rom = fs::read(rom_path).map_err(|e| e.to_string())?;
    if sha256::hex(&rom) != json_string(&index["reference_sha256"], "reference checksum")? {
        return Err("ROM checksum differs from native source reference".into());
    }
    let regions = index["regions"].as_array().unwrap();
    let ctx = Context::new(root);
    character::extract_all(root, &index["private_inputs"], &rom)?;
    let mut maps: BTreeMap<String, Vec<u8>> = BTreeMap::new();
    let mut tiles: BTreeMap<String, Vec<u8>> = BTreeMap::new();
    let mut banks: BTreeMap<usize, Vec<u16>> = BTreeMap::new();
    let mut tables = serde_json::Map::new();
    for input in index["private_inputs"]
        .as_array()
        .ok_or("missing private inputs")?
    {
        let source = json_string(&input["source"], "private source")?;
        let kind = json_string(&input["kind"], "private kind")?;
        if matches!(
            kind,
            "sprite"
                | "sprite-atlas"
                | "archive-atlas"
                | "still-atlas"
                | "tile-atlas"
                | "portrait-atlas"
                | "frame-atlas"
        ) {
            continue;
        }
        let target = address(&input["region_address"])?;
        if kind == "palette-table" {
            let start = target
                .checked_sub(ROM_BASE)
                .ok_or("palette table precedes ROM")?;
            let length = address(&input["decoded_length"])?;
            let data = rom
                .get(start..start.checked_add(length).ok_or("palette table overflows")?)
                .ok_or("palette table exceeds ROM")?;
            if length % 2 != 0
                || sha256::hex(data)
                    != json_string(&input["decoded_sha256"], "palette table digest")?
            {
                return Err("palette table differs".into());
            }
            let pointer = json_string(&input["pointer"], "palette table pointer")?;
            let key = pointer
                .strip_prefix("/tables/")
                .ok_or("palette table pointer differs")?;
            tables.insert(
                key.into(),
                json!(data
                    .chunks_exact(2)
                    .map(|p| u16::from_le_bytes([p[0], p[1]]))
                    .collect::<Vec<_>>()),
            );
            continue;
        }
        if kind == "palette-buffer" {
            let length = address(&input["decoded_length"])?;
            let buffer = decode_buffer(input, &rom, length)?;
            let offset = input
                .get("component_offset")
                .map(address)
                .transpose()?
                .unwrap_or(0);
            let data = buffer
                .get(
                    offset
                        ..offset
                            .checked_add(length)
                            .ok_or("palette component overflows")?,
                )
                .ok_or("palette component exceeds decoded buffer")?
                .to_vec();
            if sha256::hex(&data) != json_string(&input["decoded_sha256"], "palette buffer digest")?
            {
                return Err("palette buffer differs".into());
            }
            let slots = input["banks"].as_array().ok_or("palette slots absent")?;
            if data.len() != slots.len() * 32 {
                return Err("palette buffer dimensions differ".into());
            }
            for (slot, bytes) in slots.iter().zip(data.chunks_exact(32)) {
                let values = bytes
                    .chunks_exact(2)
                    .map(|p| u16::from_le_bytes([p[0], p[1]]))
                    .collect::<Vec<_>>();
                let slot = address(slot)?;
                if banks.get(&slot).is_some_and(|b| *b != values) {
                    return Err("shared palette buffer differs".into());
                }
                banks.insert(slot, values);
            }
            continue;
        }
        let region = regions
            .iter()
            .find(|r| address(&r["address"]).ok() == Some(target))
            .ok_or("missing input region")?;
        if kind == "palette-raw" {
            let start = target.checked_sub(ROM_BASE).ok_or("input precedes ROM")?;
            let end = start
                .checked_add(address(&region["size"])?)
                .ok_or("palette extent overflows")?;
            let data = rom.get(start..end).ok_or("palette outside ROM")?;
            if sha256::hex(data) != json_string(&input["decoded_sha256"], "palette digest")? {
                return Err("raw palette differs".into());
            }
            for (slot, bank) in input["banks"]
                .as_array()
                .ok_or("missing palette banks")?
                .iter()
                .zip(data.chunks_exact(32))
            {
                let values = bank
                    .chunks_exact(2)
                    .map(|b| u16::from_le_bytes([b[0], b[1]]))
                    .collect::<Vec<_>>();
                let slot = address(slot)?;
                if banks.get(&slot).is_some_and(|existing| *existing != values) {
                    return Err("shared raw palette bank differs".into());
                }
                banks.insert(slot, values);
            }
            continue;
        }
        let plan_path = root_path(root, json_string(&region["plan"], "input plan")?)?;
        let plan_document = ctx.document(&plan_path)?;
        let plan = select_plan(&plan_document, region)?;
        let start = target.checked_sub(ROM_BASE).ok_or("input precedes ROM")?;
        let end = start
            .checked_add(address(&region["size"])?)
            .ok_or("input extent overflows")?;
        let bytes = rom.get(start..end).ok_or("input outside ROM")?;
        let maximum = address(&plan["decoded_size"])? as u64;
        let mut decoded = match json_string(&plan["codec"], "codec")? {
            "golden-sun-general-lz" => {
                psynergy::assets::lz::decode_general(bytes, 0, bytes.len(), maximum).map(|v| v.0)
            }
            "golden-sun-tagged-palette-lz" => {
                psynergy::assets::lz::decode_palette(bytes, 1, bytes.len(), maximum).map(|v| v.0)
            }
            "golden-sun-palette-lz" => {
                psynergy::assets::lz::decode_palette(bytes, 0, bytes.len(), maximum).map(|v| v.0)
            }
            "golden-sun-kind2-lz" => {
                psynergy::assets::lz::decode_mtf4_lz(bytes, 0, bytes.len(), maximum).map(|v| v.0)
            }
            codec => return Err(format!("unsupported private input codec {codec}")),
        }
        .map_err(|e| e.to_string())?;
        if kind == "metatiles" {
            decoded = decode_metatiles(&decoded, address(&input["transform_mode"])? as u8)?;
        }
        if sha256::hex(&decoded) != json_string(&input["decoded_sha256"], "input digest")? {
            return Err(format!("decoded input differs at {target:#x}"));
        }
        if kind == "palette" {
            for (slot, bank) in input["banks"]
                .as_array()
                .ok_or("missing palette sequence")?
                .iter()
                .zip(decoded.chunks_exact(32))
            {
                let values = bank
                    .chunks_exact(2)
                    .map(|b| u16::from_le_bytes([b[0], b[1]]))
                    .collect::<Vec<_>>();
                let slot = address(slot)?;
                if banks.get(&slot).is_some_and(|existing| *existing != values) {
                    return Err("shared palette bank differs".into());
                }
                banks.insert(slot, values);
            }
        } else {
            let (files, offset) = if kind == "tiles" {
                (&mut tiles, address(&input["tile_offset"])? * 32)
            } else {
                (&mut maps, address(&input["source_offset"])?)
            };
            let data = files.entry(source.into()).or_default();
            let end = offset
                .checked_add(decoded.len())
                .ok_or("decoded extent overflows")?;
            if data.len() < end {
                data.resize(end, 0);
            }
            data[offset..end].copy_from_slice(&decoded);
        }
    }
    let gray: Vec<u8> = (0u16..16)
        .flat_map(|i| (i | i << 5 | i << 10).to_le_bytes())
        .collect();
    for (source, data) in maps {
        write(root, &source, &data)?;
    }
    for (source, data) in tiles {
        write(
            root,
            &source,
            &psynergy::assets::image::png_from_gba_tiles(&data, &gray, GbaBpp::Bpp4, 32)
                .map_err(|e| e.to_string())?,
        )?;
    }
    let mut colors = vec![];
    for (slot, bank) in banks {
        if slot != colors.len() {
            return Err("palette bank indices have holes".into());
        }
        colors.push(bank);
    }
    document(
        root,
        &paths.colors,
        &json!({"format":"bgr555-banks","colors_per_bank":16,"banks":colors,"tables":tables}),
    )?;
    println!(
        "extracted={} checksum=verified",
        index["private_inputs"].as_array().unwrap().len()
    );
    Ok(())
}

/// Regenerate absent private inputs in isolation, preserving existing edits.
pub fn extract_missing(root: &Path, rom_path: &Path, target: &DecompTarget) -> Result<(), String> {
    let paths = NativePaths::of(target);
    let index = json(&root.join(&paths.index))?;
    validate(&index)?;
    let mut missing = std::collections::BTreeSet::new();
    for input in index["private_inputs"]
        .as_array()
        .ok_or("missing private inputs")?
    {
        let source = json_string(&input["source"], "private source")?;
        if !root_path(root, source)?.exists() {
            missing.insert(source.to_string());
        }
    }
    if missing.is_empty() {
        return Ok(());
    }
    let stage = tempfile::tempdir().map_err(|e| e.to_string())?;
    // Atlas configurations are inputs too, although they are not region plans.
    for entry in walkdir::WalkDir::new(root.join(paths.source)) {
        let entry = entry.map_err(|e| e.to_string())?;
        if entry.file_type().is_file()
            && entry.path().extension().and_then(|e| e.to_str()) == Some("JSON")
        {
            let destination = stage
                .path()
                .join(entry.path().strip_prefix(root).map_err(|e| e.to_string())?);
            std::fs::create_dir_all(destination.parent().unwrap()).map_err(|e| e.to_string())?;
            std::fs::copy(entry.path(), destination).map_err(|e| e.to_string())?;
        }
    }
    for name in std::iter::once(paths.index.as_str()).chain(
        index["regions"]
            .as_array()
            .ok_or("missing regions")?
            .iter()
            .filter_map(|region| region["plan"].as_str()),
    ) {
        let destination = root_path(stage.path(), name)?;
        std::fs::create_dir_all(destination.parent().unwrap()).map_err(|e| e.to_string())?;
        std::fs::copy(root_path(root, name)?, destination).map_err(|e| e.to_string())?;
    }
    extract(stage.path(), rom_path, target)?;
    for name in &missing {
        let bytes = std::fs::read(root_path(stage.path(), name)?).map_err(|e| e.to_string())?;
        create_missing(&root_path(root, name)?, &bytes)?;
    }
    println!("restored missing private source files: {}", missing.len());
    Ok(())
}

fn create_missing(path: &Path, bytes: &[u8]) -> Result<(), String> {
    use std::io::Write;
    std::fs::create_dir_all(path.parent().ok_or("source has no parent")?)
        .map_err(|e| e.to_string())?;
    match std::fs::OpenOptions::new()
        .write(true)
        .create_new(true)
        .open(path)
    {
        Ok(mut file) => file.write_all(bytes).map_err(|e| e.to_string()),
        Err(error) if error.kind() == std::io::ErrorKind::AlreadyExists => Ok(()),
        Err(error) => Err(error.to_string()),
    }
}

#[test]
fn restoring_missing_inputs_preserves_existing_edits() {
    let temp = tempfile::tempdir().unwrap();
    let path = temp.path().join("SRC/CHR.PNG");
    create_missing(&path, b"original").unwrap();
    std::fs::write(&path, b"user edit").unwrap();
    create_missing(&path, b"regenerated").unwrap();
    assert_eq!(std::fs::read(path).unwrap(), b"user edit");
}
pub(in crate::build_assets) fn decode_metatile_words(
    data: &[u8],
    mode: u8,
) -> Result<Vec<u8>, String> {
    decode_metatiles(data, mode)
}
fn decode_metatiles(data: &[u8], mode: u8) -> Result<Vec<u8>, String> {
    if data.first() != Some(&mode) || data.len() % 2 != 1 {
        return Err("invalid metatile transform header".into());
    }
    let mut output = vec![];
    let mut previous = 0u16;
    let count = (data.len() - 1) / 2;
    for i in 0..count {
        let value = match mode {
            0 | 2 => u16::from_le_bytes([data[1 + i * 2], data[2 + i * 2]]),
            1 => u16::from_be_bytes([data[1 + i], data[1 + count + i]]),
            _ => return Err("unsupported metatile transform".into()),
        };
        let value = if mode == 0 { value } else { value ^ previous };
        previous = value;
        output.extend(value.to_le_bytes());
    }
    Ok(output)
}

#[test]
fn map_sections_reject_overlap_and_incomplete_metatiles() {
    let layout = json!({"map":"map.bin","grid_offset":0,"grid_length":65536,"metatile_offset":65536,"metatile_length":8});
    let mut index =
        json!({"format":"camelot-style-golden-sun-native","layouts":[layout],"regions":[]});
    validate(&index).unwrap();
    index["layouts"][0]["metatile_length"] = json!(6);
    assert!(validate(&index).is_err());
    index["layouts"][0]["metatile_length"] = json!(8);
    let duplicate = index["layouts"][0].clone();
    index["layouts"].as_array_mut().unwrap().push(duplicate);
    assert!(validate(&index).is_err());
}
#[test]
fn extracted_metatile_transforms_preserve_palette_and_flip_bits() {
    let words = [0x21, 0xf4, 0x32, 0x18, 0, 0x80, 0xff, 0xff];
    for mode in 0..3 {
        let encoded = psynergy::assets::compression::encode_tilemap_delta(&words, mode).unwrap();
        assert_eq!(decode_metatiles(&encoded, mode).unwrap(), words);
    }
}
