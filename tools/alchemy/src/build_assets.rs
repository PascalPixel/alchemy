//! Native entry point for the asset build stage.
mod compression_plan;
mod derive_index;
pub(crate) use compression_plan::LzMachine;
pub(crate) use derive_index::{live_scene, network::live_family, tagged_extent};
mod gba_header;
mod native;
pub(crate) use native::{icon_bank_source, raw_palette_bank, ICON_BANKS, ICON_PALETTE_BANK};
mod packer;
use crate::compiler::build_io::{relative, text};
use crate::compiler::bundle::{
    compiler_bundle_signature, executable_signature, host_executable_signature,
};
use crate::compiler::canonical_json::canonical_json;
use crate::compiler::routing::{cflags_for_target_source, CompilerTarget};
use crate::compiler::sha256;
use crate::compiler::source_inputs::compiler_source_tree_signature;
use crate::compiler::source_paths::{source_paths_manifest, SourcePaths};
use crate::generated_files::{prune_files, unconsumed_tracked_material};
use crate::overlay::compile::assemble_overlay;
use crate::overlay::source::OverlaySource;
use gba_header::{build_gba_header_component, read_gba_header_source};
use psynergy::assets::lz::{PaletteGroup, PaletteOperation};
use psynergy::assets::text::import_tilemap;
use psynergy::assets::{
    image::{gba_graphics, gba_palette_rgba, indexed_png, one_bit_tiles, rgba_png, GbaBpp},
    midi::{midi_events, EventBody, MidiEvent},
};
use psynergy::cache::write_cache_entry_atomically;
use serde_json::Value;
use std::collections::{BTreeMap, BTreeSet, HashMap};
use std::env;
use std::fs;
use std::path::{Path, PathBuf};
use std::process::ExitCode;
const USAGE: &str = "usage: alchemy build assets [-h] [--source-only] [--target TARGET] [--manifest MANIFEST] [-o OUTPUT] [rom] | --extract-text [TARGET] | --verify-text [TARGET] | --review-images OUTPUT [--update-baseline | --target TARGET] | --audit-characters OUTPUT [--target TARGET] | --extract-sources ROM [--target TARGET] | --extract-missing-sources ROM [--target TARGET] | --derive-index ROM --target TARGET --scenes N[=NAME],... [--leave ID,...] [-o OUTPUT] [--stage DIR] [--preview DIR] | --network ROM --target TARGET -o DIR [--from WORLD_MAP_EXIT | --scenes LIST] [--mark SCENE] [--packed] | --verify-smsh-source ROM SOURCE | --adopt-smsh-midi SOURCE INPUT OUTPUT | --verify-smsh-midi ROM MIDI | --self-test";
const ROM_BASE: usize = 0x0800_0000;
pub(crate) fn identified_regions(
    root: &Path,
    rom: &[u8],
    target: &crate::targets::DecompTarget,
) -> Result<(Vec<Value>, Vec<Value>), String> {
    let (mut rows, mut failures) = native::character_inventory(rom, target)?;
    let (field, issues) = derive_index::inventory(root, rom, target)?;
    rows.extend(field);
    failures.extend(issues);
    rows.extend(sound_inventory(root, rom, target)?);
    Ok((rows, failures))
}

fn sound_inventory(
    root: &Path,
    rom: &[u8],
    target: &crate::targets::DecompTarget,
) -> Result<Vec<Value>, String> {
    let registry = format!("{}/translation-units.json", target.recon_dir());
    let units = json(&root.join(&registry))?;
    let units = units
        .as_array()
        .or_else(|| units["units"].as_array())
        .ok_or("unit registry has no units")?;
    let symbol = units
        .iter()
        .find_map(|u| u["absolute_symbols"]["Sound_SongTable"].get("address"));
    let Some(symbol) = symbol else {
        return Ok(vec![]);
    };
    let table = number(symbol, "song table")?
        .checked_sub(ROM_BASE)
        .ok_or("song table outside ROM")?;
    let word = |offset: usize| -> Option<usize> {
        Some(u32::from_le_bytes(rom.get(offset..offset.checked_add(4)?)?.try_into().ok()?) as usize)
    };
    let pointer = |offset: usize| -> Option<usize> {
        word(offset)?
            .checked_sub(ROM_BASE)
            .filter(|p| *p < rom.len())
    };
    let mut rows = vec![];
    let mut banks = BTreeSet::new();
    let mut sequences = BTreeMap::<usize, (usize, Vec<usize>)>::new();
    // SongEntry and SequenceHeader are the structures consumed by the shared
    // byte-exact Sound and MusicPlayer units. Stop at the first invalid entry.
    for slot in 0..2048 {
        let at = table + slot * 8;
        let Some(header) = pointer(at) else { break };
        let Some(&tracks) = rom.get(header) else {
            break;
        };
        let Some(entry) = rom.get(at..at + 8) else {
            break;
        };
        if u16::from_le_bytes([entry[4], entry[5]]) >= 8 || tracks > 16 {
            break;
        }
        let size = 8 + usize::from(tracks) * 4;
        if header + size > rom.len() {
            break;
        }
        if (0..usize::from(tracks)).any(|i| pointer(header + 8 + i * 4).is_none()) {
            break;
        }
        rows.push(serde_json::json!({"start":ROM_BASE+at,"end":ROM_BASE+at+8,"kind":"record-table","label":"Sound selection table","evidence":registry}));
        rows.push(serde_json::json!({"start":ROM_BASE+header,"end":ROM_BASE+header+size,"kind":"record-table","label":"Music sequence header","evidence":format!("Sound_SongTable entry {slot}; SequenceHeader")}));
        sequences.entry(header).or_insert_with(|| {
            (
                size,
                (0..usize::from(tracks))
                    .filter_map(|index| pointer(header + 8 + index * 4))
                    .collect(),
            )
        });
        if let Some(bank) = pointer(header + 4) {
            banks.insert(bank);
        }
    }
    let sequence_headers = sequences.keys().copied().collect::<Vec<_>>();
    for pair in sequence_headers.windows(2) {
        let (header, next) = (pair[0], pair[1]);
        let (size, _) = &sequences[&header];
        let (_, tracks) = &sequences[&next];
        let Some(start) = tracks.iter().copied().min() else {
            continue;
        };
        if start < header + size || tracks.iter().any(|track| *track >= next) {
            continue;
        }
        rows.push(serde_json::json!({
            "start":ROM_BASE+start,
            "end":ROM_BASE+next,
            "kind":"golden-sun-sound-sequence",
            "label":"Music sequence tracks",
            "evidence":format!("SequenceHeader at 0x{:08x}: every track pointer lies after the preceding physical header at 0x{:08x} and before this owning header",ROM_BASE+next,ROM_BASE+header)
        }));
    }
    let mut pending = banks.into_iter().collect::<Vec<_>>();
    let mut visited = BTreeSet::new();
    while let Some(bank) = pending.pop() {
        if !visited.insert(bank) {
            continue;
        }
        for voice in 0..128 {
            let at = bank + voice * 12;
            let Some(record) = rom.get(at..at + 12) else {
                continue;
            };
            if record.iter().all(|byte| *byte == 0) {
                continue;
            }
            let kind = record[0];
            if !matches!(kind, 0 | 1 | 2 | 3 | 4 | 8 | 9 | 10 | 11 | 12 | 64 | 128) {
                continue;
            }
            let Some(target) = pointer(at + 4) else {
                continue;
            };
            rows.push(serde_json::json!({
                "start":ROM_BASE+at,
                "end":ROM_BASE+at+12,
                "kind":"golden-sun-sound-voice",
                "label":"Sound voice record",
                "evidence":format!("SequenceHeader voice bank 0x{:08x}, voice {voice}; recognized voice kind {kind} and in-ROM target",ROM_BASE+bank)
            }));
            if matches!(kind, 64 | 128) {
                pending.push(target);
                continue;
            }
            if !matches!(kind, 0 | 8) {
                continue;
            }
            let (Some(control), Some(frequency), Some(loop_start), Some(last)) = (
                word(target),
                word(target + 4),
                word(target + 8),
                word(target + 12),
            ) else {
                continue;
            };
            let Some(end) = target
                .checked_add(17)
                .and_then(|n| n.checked_add(last))
                .filter(|end| *end <= rom.len())
            else {
                continue;
            };
            if control & 0x3fffffff != 0
                || frequency == 0
                || frequency > 192000 * 1024
                || (control & 0xc0000000 != 0 && loop_start > last)
            {
                continue;
            }
            rows.push(serde_json::json!({"start":ROM_BASE+target,"end":ROM_BASE+end,"kind":"golden-sun-pcm-wave","label":"PCM sample","evidence":format!("SequenceHeader voice bank 0x{:08x}, voice {voice}; 16-byte wave header and {} samples",ROM_BASE+bank,last+1)}));
        }
    }
    Ok(rows)
}

#[test]
fn sound_index_follows_registered_voices_and_checks_sample_extents() {
    let root = tempfile::tempdir().unwrap();
    let target = crate::targets::decomp_target(Some("tla-en")).unwrap();
    let registry = root.path().join(target.recon_dir());
    fs::create_dir_all(&registry).unwrap();
    fs::write(
        registry.join("translation-units.json"),
        r#"{"units":[{"absolute_symbols":{"Sound_SongTable":{"address":"0x08000100"}}}]}"#,
    )
    .unwrap();
    let mut rom = vec![0u8; 0x1100];
    for (at, value) in [
        (0x100, 0x08000200u32),
        (0x204, 0x08000300),
        (0x304, 0x08001000),
        (0x1004, 8192000),
        (0x100c, 9),
    ] {
        rom[at..at + 4].copy_from_slice(&value.to_le_bytes());
    }
    let sample = |rom: &[u8]| {
        sound_inventory(root.path(), rom, &target)
            .unwrap()
            .into_iter()
            .find(|r| r["kind"] == "golden-sun-pcm-wave")
    };
    let row = sample(&rom).unwrap();
    assert_eq!(row["start"], 0x08001000);
    assert_eq!(row["end"], 0x0800101a);
    let voices = sound_inventory(root.path(), &rom, &target)
        .unwrap()
        .into_iter()
        .filter(|row| row["kind"] == "golden-sun-sound-voice")
        .collect::<Vec<_>>();
    assert_eq!(voices.len(), 1);
    assert_eq!(voices[0]["start"], 0x08000300);
    rom[0x100c..0x1010].copy_from_slice(&0xffffu32.to_le_bytes());
    assert!(sample(&rom).is_none());
}
fn repository_root() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR"))
        .parent()
        .and_then(Path::parent)
        .expect("build-assets is under tools")
        .to_path_buf()
}
fn number(value: &Value, label: &str) -> Result<usize, String> {
    let text = match value {
        Value::Number(value) => value.to_string(),
        Value::String(value) => value.clone(),
        _ => return Err(format!("{label} must be an integer")),
    };
    let parsed = if let Some(hex) = text.strip_prefix("0x").or_else(|| text.strip_prefix("0X")) {
        usize::from_str_radix(hex, 16)
    } else {
        text.parse::<usize>()
    };
    parsed.map_err(|_| format!("{label} must be an integer"))
}
fn parse_operation(value: &Value) -> Result<PaletteOperation, String> {
    let items = value
        .as_array()
        .ok_or_else(|| "palette token operation is not an array".to_string())?;
    let tag = items
        .first()
        .and_then(Value::as_str)
        .ok_or_else(|| "palette token operation has no tag".to_string())?;
    match tag {
        "l" if items.len() == 1 => Ok(PaletteOperation::Literal),
        "e" if items.len() == 1 => Ok(PaletteOperation::End),
        "c" if items.len() == 3 => Ok(PaletteOperation::Copy {
            length: number(&items[1], "copy length")? as u32,
            distance: number(&items[2], "copy distance")? as u32,
        }),
        _ => Err("unsupported palette token operation".to_string()),
    }
}
fn parse_group(value: &Value) -> Result<PaletteGroup, String> {
    let items = value
        .as_array()
        .ok_or_else(|| "palette token group is not an array".to_string())?;
    let tag = items
        .first()
        .and_then(Value::as_str)
        .ok_or_else(|| "palette token group has no tag".to_string())?;
    match tag {
        "z" if items.len() == 1 => Ok(PaletteGroup::Zeros),
        "g" if items.len() == 2 => {
            let operations = items[1]
                .as_array()
                .ok_or_else(|| "palette token group operations are not an array".to_string())?
                .iter()
                .map(parse_operation)
                .collect::<Result<Vec<_>, _>>()?;
            Ok(PaletteGroup::Group(operations))
        }
        _ => Err("unsupported palette token group".to_string()),
    }
}
fn json(path: &Path) -> Result<Value, String> {
    serde_json::from_slice(&fs::read(path).map_err(|error| format!("{}: {error}", path.display()))?)
        .map_err(|error| format!("{}: invalid JSON: {error}", path.display()))
}
fn json_string<'a>(value: &'a Value, label: &str) -> Result<&'a str, String> {
    value
        .as_str()
        .ok_or_else(|| format!("{label} must be a string"))
}
#[test]
fn field_readers_reject_missing_null_and_wrong_types_without_defaulting() {
    for document in [
        serde_json::json!({}),
        serde_json::json!({"field": null}),
        serde_json::json!({"field": true}),
        serde_json::json!([]),
        serde_json::json!(false),
    ] {
        assert!(number(&document["field"], "field").is_err());
        assert!(json_string(&document["field"], "field").is_err());
    }
    for (value, expected) in [(serde_json::json!(0), 0), (serde_json::json!("0x80"), 128)] {
        let document = serde_json::json!({"field": value});
        assert_eq!(number(&document["field"], "field").unwrap(), expected);
    }
    assert!(number(&serde_json::json!(-1), "field").is_err());
    assert!(number(&serde_json::json!(1.5), "field").is_err());
    assert_eq!(
        json_string(&serde_json::json!("source.png"), "field").unwrap(),
        "source.png"
    );
}
fn root_path(root: &Path, name: &str) -> Result<PathBuf, String> {
    let mut path = if Path::new(name).is_absolute() {
        PathBuf::from(name)
    } else {
        root.join(name)
    };
    if !path.starts_with(root) {
        return Err(format!(
            "asset source must stay inside the repository: {name}"
        ));
    }
    if !path.exists() && !Path::new(name).is_absolute() {
        let cached = root.join("out/private").join(name);
        if cached.exists() {
            path = cached;
        }
    }
    Ok(path)
}
/// Views, stages and derived rows of game material are disposable output: they
/// may only be written under the repository's ignored out/ directory.
fn ignored_output_path(root: &Path, output: &Path, label: &str) -> Result<PathBuf, String> {
    let output = if output.is_absolute() {
        output.to_path_buf()
    } else {
        root.join(output)
    };
    if !output.starts_with(root.join("out")) {
        return Err(format!(
            "{label} must remain in the ignored repository out/ directory"
        ));
    }
    if output
        .components()
        .any(|part| matches!(part, std::path::Component::ParentDir))
    {
        return Err(format!("{label} output cannot contain parent traversal"));
    }
    Ok(output)
}
#[test]
fn ignored_output_stays_under_out() {
    let root = Path::new("/repository");
    assert_eq!(
        ignored_output_path(root, Path::new("out/tla-en/preview"), "preview").unwrap(),
        root.join("out/tla-en/preview")
    );
    for escaped in [
        "games/THE LOST AGE/PREVIEW",
        "games/THE BROKEN SEAL/SRC/GRAPHICS/REVIEW",
        "out/../games/THE LOST AGE",
        "tools/alchemy/GRAPHICS",
        "out/../tools/alchemy",
        "/elsewhere/out",
        "/repository/tools/out",
        "outside",
    ] {
        assert!(ignored_output_path(root, Path::new(escaped), "preview").is_err());
    }
}
fn root_relative(root: &Path, path: &Path) -> Result<String, String> {
    let relative = path.strip_prefix(root).map_err(|_| {
        format!(
            "asset source must stay inside the repository: {}",
            path.display()
        )
    })?;
    Ok(relative.to_string_lossy().replace('\\', "/"))
}
fn hex_address(address: usize) -> String {
    format!("0x{address:08x}")
}
fn child_path(plan_path: &Path, name: &str) -> PathBuf {
    let parent = plan_path.parent().unwrap_or(Path::new("."));
    let base = plan_path.file_name().unwrap_or_default().to_string_lossy();
    let prefix = base.rfind('_').map(|index| &base[..=index]).unwrap_or("");
    let flat = parent.join(format!("{}{}", prefix, name.replace('/', "_")));
    if flat.exists() {
        flat
    } else {
        parent.join(name)
    }
}
fn dedup_sources(sources: Vec<String>) -> Vec<String> {
    let mut seen = BTreeSet::new();
    sources
        .into_iter()
        .filter(|source| seen.insert(source.clone()))
        .collect()
}
#[derive(Debug)]
struct ComponentResult {
    data: Vec<u8>,
    sources: Vec<String>,
    details: Value,
}
fn decode_tile_entry(value: u16) -> (usize, usize, bool, bool) {
    (
        usize::from(value & 0x03ff),
        usize::from((value >> 12) & 0x0f),
        value & 0x0400 != 0,
        value & 0x0800 != 0,
    )
}
fn pack_tile(pixels: &[u8]) -> Result<Vec<u8>, String> {
    if pixels.len() != 64 || pixels.iter().any(|pixel| *pixel >= 16) {
        return Err("object tile does not contain 64 4bpp indices".to_string());
    }
    let mut output = vec![0; 32];
    for index in 0..32 {
        output[index] = pixels[index * 2] | (pixels[index * 2 + 1] << 4);
    }
    Ok(output)
}
fn flip_tile(pixels: &[u8], hflip: bool, vflip: bool) -> Vec<u8> {
    let mut output = Vec::with_capacity(64);
    for y in 0..8 {
        for x in 0..8 {
            let yy = if vflip { 7 - y } else { y };
            let xx = if hflip { 7 - x } else { x };
            output.push(pixels[yy * 8 + xx]);
        }
    }
    output
}
fn build_object_bank(root: &Path, plan_path: &Path) -> Result<ComponentResult, String> {
    let plan = json(plan_path)?;
    if plan.get("format") != Some(&Value::from(1))
        || plan.get("kind").and_then(Value::as_str) != Some("gba-4bpp-object-bank")
    {
        return Err("unsupported tile-object plan".to_string());
    }
    let tile_count = number(&plan["tile_count"], "tile_count")?;
    let fallback = json_string(&plan["fallback"], "fallback")?;
    let fallback_path = child_path(plan_path, fallback);
    let (fallback_bytes, _, _) = gba_graphics(
        &fs::read(&fallback_path).map_err(|e| e.to_string())?,
        GbaBpp::Bpp4,
    )
    .map_err(|e| e.to_string())?;
    if fallback_bytes.len() != tile_count * 32 {
        return Err("object-bank fallback has the wrong tile count".to_string());
    }
    let mut output = fallback_bytes;
    let mut claimed: BTreeMap<usize, Vec<u8>> = BTreeMap::new();
    let mut sources = vec![
        root_relative(root, plan_path)?,
        root_relative(root, &fallback_path)?,
    ];
    let mut placements = 0usize;
    let mut horizontal_flips = 0usize;
    let mut vertical_flips = 0usize;
    for item in plan
        .get("objects")
        .and_then(Value::as_array)
        .unwrap_or(&Vec::new())
    {
        let object = item.as_object().ok_or("object-bank object is malformed")?;
        let source_path = child_path(
            plan_path,
            json_string(
                object.get("source").ok_or("object source missing")?,
                "object source",
            )?,
        );
        let tilemap_path = child_path(
            plan_path,
            json_string(
                object.get("tilemap").ok_or("object tilemap missing")?,
                "object tilemap",
            )?,
        );
        let palette_path = child_path(
            plan_path,
            json_string(
                object.get("palette").ok_or("object palette missing")?,
                "object palette",
            )?,
        );
        let image = indexed_png(&fs::read(&source_path).map_err(|e| e.to_string())?)
            .map_err(|e| e.to_string())?;
        let expected = indexed_png(&fs::read(&palette_path).map_err(|e| e.to_string())?)
            .map_err(|e| e.to_string())?;
        if image.palette != expected.palette {
            return Err(format!(
                "{}: palette differs from {}",
                source_path.display(),
                palette_path.display()
            ));
        }
        let tilemap =
            import_tilemap(&fs::read_to_string(&tilemap_path).map_err(|e| e.to_string())?)
                .map_err(|e| e.to_string())?;
        let width = image.width as usize;
        let height = image.height as usize;
        if width % 8 != 0 || height % 8 != 0 || tilemap.len() / 2 != width / 8 * (height / 8) {
            return Err(format!(
                "{}: dimensions differ from its tilemap",
                source_path.display()
            ));
        }
        let base_tile = number(
            object.get("base_tile").ok_or("object base_tile missing")?,
            "base_tile",
        )?;
        for (cell, bytes) in tilemap.chunks_exact(2).enumerate() {
            let (tile, palette, hflip, vflip) =
                decode_tile_entry(u16::from_le_bytes([bytes[0], bytes[1]]));
            if tile < base_tile || tile - base_tile >= tile_count {
                if object
                    .get("mixed_windows")
                    .and_then(Value::as_bool)
                    .unwrap_or(false)
                {
                    continue;
                }
                return Err(format!(
                    "{}: tile lies outside this bank",
                    tilemap_path.display()
                ));
            }
            let left = (cell % (width / 8)) * 8;
            let top = (cell / (width / 8)) * 8;
            let mut displayed = Vec::with_capacity(64);
            for y in 0..8 {
                for x in 0..8 {
                    displayed.push(image.pixels[(top + y) * width + left + x] as u8);
                }
            }
            if displayed
                .iter()
                .any(|pixel| usize::from(*pixel) / 16 != palette)
            {
                return Err(format!(
                    "{}: cell {cell} differs from its palette bank",
                    source_path.display()
                ));
            }
            let canonical = flip_tile(
                &displayed
                    .iter()
                    .map(|pixel| pixel & 0x0f)
                    .collect::<Vec<_>>(),
                hflip,
                vflip,
            );
            let packed = pack_tile(&canonical)?;
            if claimed
                .get(&(tile - base_tile))
                .is_some_and(|previous| previous != &packed)
            {
                return Err(format!(
                    "{}: repeated tile slot {} is inconsistent",
                    source_path.display(),
                    tile - base_tile
                ));
            }
            claimed.insert(tile - base_tile, packed);
            placements += 1;
            horizontal_flips += usize::from(hflip);
            vertical_flips += usize::from(vflip);
        }
        sources.extend([
            root_relative(root, &source_path)?,
            root_relative(root, &tilemap_path)?,
            root_relative(root, &palette_path)?,
        ]);
    }
    for (slot, packed) in &claimed {
        if plan
            .get("require_blank_fallback")
            .and_then(Value::as_bool)
            .unwrap_or(true)
            && output[*slot * 32..*slot * 32 + 32]
                .iter()
                .any(|byte| *byte != 0)
        {
            return Err(format!("fallback still contains authoritative tile {slot}"));
        }
        output[*slot * 32..*slot * 32 + 32].copy_from_slice(packed);
    }
    Ok(ComponentResult {
        data: output,
        sources: dedup_sources(sources),
        details: serde_json::json!({
            "tiles": tile_count,
            "object_tiles": claimed.len(),
            "placements": placements,
            "horizontal_flips": horizontal_flips,
            "vertical_flips": vertical_flips,
            "objects": plan.get("objects").and_then(Value::as_array).map_or(0, Vec::len),
        }),
    })
}
/// Frames selected by a component's atlas geometry (`frame_width`,
/// `frame_height` and `columns` with `frame`, `frame_order` or `frames`), or
/// the whole image when it declares none. `depth` is the bytes per pixel.
fn component_frames(
    entry: &Value,
    width: usize,
    height: usize,
    pixels: &[u8],
    depth: usize,
) -> Result<Vec<Vec<u8>>, String> {
    let Some(frame_width) = entry.get("frame_width") else {
        return Ok(vec![pixels.to_vec()]);
    };
    let frame_width = number(frame_width, "frame width")?;
    let frame_height = number(&entry["frame_height"], "frame height")?;
    let columns = number(&entry["columns"], "atlas columns")?;
    if frame_width == 0
        || frame_height == 0
        || columns == 0
        || width != columns * frame_width
        || height % frame_height != 0
    {
        return Err("atlas dimensions differ".into());
    }
    let slots = columns * (height / frame_height);
    let selected: Vec<usize> = if let Some(frame) = entry.get("frame") {
        vec![number(frame, "frame")?]
    } else if let Some(order) = entry.get("frame_order") {
        order
            .as_array()
            .ok_or("frame order is not an array")?
            .iter()
            .map(|frame| number(frame, "frame"))
            .collect::<Result<_, _>>()?
    } else {
        let frames = number(&entry["frames"], "frames")?;
        if frames == 0 || frames.div_ceil(columns) * frame_height != height {
            return Err("atlas dimensions differ".into());
        }
        (0..frames).collect()
    };
    let mut output = Vec::with_capacity(selected.len());
    for frame in selected {
        if frame >= slots {
            return Err(format!("frame {frame} lies outside its atlas"));
        }
        let left = frame % columns * frame_width;
        let top = frame / columns * frame_height;
        let mut buffer = Vec::with_capacity(frame_width * frame_height * depth);
        for y in 0..frame_height {
            let start = ((top + y) * width + left) * depth;
            buffer.extend_from_slice(&pixels[start..start + frame_width * depth]);
        }
        output.push(buffer);
    }
    Ok(output)
}
/// The frames of a sheet component that store no bytes at all: the source
/// never gave them pixels, so their cells stay blank. Only a component that
/// is built one frame per stream can name them.
fn absent_frames(entry: &Value) -> Result<BTreeSet<usize>, String> {
    let Some(frames) = entry.get("absent_frames") else {
        return Ok(BTreeSet::new());
    };
    let frames = frames
        .as_array()
        .ok_or("absent frames are not an array")?
        .iter()
        .map(|frame| number(frame, "absent frame"))
        .collect::<Result<Vec<_>, _>>()?;
    let set = frames.iter().copied().collect::<BTreeSet<_>>();
    if set.is_empty() || set.len() != frames.len() || !frames.windows(2).all(|w| w[0] < w[1]) {
        return Err("absent frames must be a nonempty ascending list".into());
    }
    Ok(set)
}
fn is_absent_frame(entry: &Value) -> Result<bool, String> {
    let absent = absent_frames(entry)?;
    match entry.get("frame") {
        Some(frame) => Ok(absent.contains(&number(frame, "frame")?)),
        None if absent.is_empty() => Ok(false),
        None => Err("absent frames need one stream per frame".into()),
    }
}
/// Check an indexed image against the shared palette its component declares:
/// `{"source", "offset", "entries"}` name the colors that the pixels
/// `offset..offset + entries` of an indexed palette image select.
fn check_shared_palette(
    root: &Path,
    entry: &Value,
    image: &psynergy::assets::image::IndexedImage,
) -> Result<Option<String>, String> {
    let Some(shared) = entry.get("palette") else {
        return Ok(None);
    };
    let name = json_string(&shared["source"], "shared palette source")?;
    if shared.get("banks").is_some() {
        let data = palette_banks(root, shared)?;
        let colors = data
            .chunks_exact(2)
            .map(|b| {
                let color = u16::from_le_bytes([b[0], b[1]]);
                [
                    ((color & 31) << 3) as u8,
                    (((color >> 5) & 31) << 3) as u8,
                    (((color >> 10) & 31) << 3) as u8,
                ]
            })
            .collect::<Vec<_>>();
        let expected = if entry["pixel_format"] == "indices" {
            (0..colors.len()).map(|i| [i as u8; 3]).collect::<Vec<_>>()
        } else {
            colors
        };
        if image.palette != expected || image.pixels.iter().any(|p| *p as usize >= expected.len()) {
            return Err(format!(
                "indexed image differs from declared palette {name}"
            ));
        }
        return Ok(Some(name.into()));
    }
    let palette = indexed_png(&fs::read(root_path(root, name)?).map_err(|e| e.to_string())?)
        .map_err(|e| e.to_string())?;
    let offset = number(&shared["offset"], "shared palette offset")?;
    let entries = number(&shared["entries"], "shared palette entries")?;
    let colors = palette
        .pixels
        .get(offset..offset + entries)
        .ok_or("shared palette is too small")?
        .iter()
        .map(|pixel| {
            palette
                .palette
                .get(*pixel as usize)
                .copied()
                .ok_or("shared palette references a missing color")
        })
        .collect::<Result<Vec<_>, _>>()?;
    if image.palette != colors {
        return Err(format!("palette differs from the shared palette {name}"));
    }
    Ok(Some(name.to_string()))
}
fn palette_banks(root: &Path, entry: &Value) -> Result<Vec<u8>, String> {
    let source = root_path(root, json_string(&entry["source"], "palette source")?)?;
    let document = json(&source)?;
    let mut bytes = vec![];
    for slot in entry["banks"]
        .as_array()
        .ok_or("missing palette bank sequence")?
    {
        let values = document["banks"]
            .get(number(slot, "palette bank")?)
            .and_then(Value::as_array)
            .ok_or("missing palette bank")?;
        if values.len() != 16 {
            return Err("palette bank needs sixteen colors".into());
        }
        for color in values {
            bytes.extend(
                u16::try_from(number(color, "palette color")?)
                    .map_err(|_| "palette exceeds u16")?
                    .to_le_bytes(),
            );
        }
    }
    Ok(bytes)
}
fn indexed_rect(
    image: &psynergy::assets::image::IndexedImage,
    entry: &Value,
) -> Result<(usize, usize, Vec<u8>), String> {
    let (width, height) = (image.width as usize, image.height as usize);
    let Some(rect) = entry.get("source_rect") else {
        return Ok((
            width,
            height,
            image.pixels.iter().map(|p| *p as u8).collect(),
        ));
    };
    let x = number(&rect["x"], "sheet x")?;
    let y = number(&rect["y"], "sheet y")?;
    let w = number(&rect["width"], "sheet width")?;
    let h = number(&rect["height"], "sheet height")?;
    if w == 0
        || h == 0
        || x.checked_add(w).is_none_or(|end| end > width)
        || y.checked_add(h).is_none_or(|end| end > height)
    {
        return Err("indexed section lies outside sheet".into());
    }
    let mut pixels = Vec::with_capacity(w.checked_mul(h).ok_or("indexed section overflows")?);
    for row in y..y + h {
        pixels.extend(
            image.pixels[row * width + x..row * width + x + w]
                .iter()
                .map(|p| *p as u8),
        );
    }
    Ok((w, h, pixels))
}

#[test]
fn indexed_sections_preserve_rows_and_reject_outside_bounds() {
    use serde_json::json;
    let image = psynergy::assets::image::IndexedImage {
        width: 4,
        height: 3,
        pixels: (0..12).collect(),
        palette: vec![],
        has_transparency: false,
    };
    assert_eq!(
        indexed_rect(
            &image,
            &json!({"source_rect":{"x":1,"y":1,"width":2,"height":2}})
        )
        .unwrap(),
        (2, 2, vec![5, 6, 9, 10])
    );
    for rect in [
        json!({"x":3,"y":0,"width":2,"height":1}),
        json!({"x":0,"y":2,"width":1,"height":2}),
        json!({"x":0,"y":0,"width":0,"height":1}),
    ] {
        assert!(indexed_rect(&image, &json!({"source_rect":rect})).is_err());
    }
}
#[test]
fn tile_frames_keep_each_frames_tile_order() {
    let frames =
        serde_json::json!({"frames":2,"columns":2,"frame_tiles_wide":2,"frame_tiles_high":2});
    assert_eq!(
        tile_coordinates(32, 16, &frames).unwrap(),
        [
            (0, 0),
            (8, 0),
            (0, 8),
            (8, 8),
            (16, 0),
            (24, 0),
            (16, 8),
            (24, 8)
        ]
    );
    assert!(tile_coordinates(32, 8, &frames).is_err());
    let mut invalid = frames;
    invalid["columns"] = serde_json::json!(0);
    assert!(tile_coordinates(32, 16, &invalid).is_err());
    invalid["columns"] = serde_json::json!(2);
    invalid["frame_tiles_wide"] = serde_json::json!(usize::MAX);
    assert!(tile_coordinates(32, 16, &invalid).is_err());
}

#[test]
fn typed_byte_frames_select_complete_frames_and_reject_invalid_extents() {
    let directory = tempfile::tempdir().unwrap();
    fs::write(
        directory.path().join("frames.json"),
        "{\"frames\":[0,1,2,3,4,5]}",
    )
    .unwrap();
    let mut entry = serde_json::json!({"kind":"u8-array","source":"frames.json","pointer":"/frames","frame_size":2,"frame":1,"size":2});
    assert_eq!(
        build_component(directory.path(), &entry).unwrap().data,
        [2, 3]
    );
    entry["frame"] = serde_json::json!(2);
    assert_eq!(
        build_component(directory.path(), &entry).unwrap().data,
        [4, 5]
    );
    entry["frame"] = serde_json::json!(3);
    assert!(build_component(directory.path(), &entry).is_err());
    entry["frame"] = serde_json::json!(usize::MAX);
    assert!(build_component(directory.path(), &entry).is_err());
    entry["frame"] = serde_json::json!(0);
    entry["frame_size"] = serde_json::json!(4);
    assert!(build_component(directory.path(), &entry).is_err());
}

fn binary_source(source: &Path, entry: &Value) -> Result<Vec<u8>, String> {
    let data = fs::read(source).map_err(|error| error.to_string())?;
    let offset = entry
        .get("source_offset")
        .map(|v| number(v, "binary source offset"))
        .transpose()?
        .unwrap_or(0);
    let available = data
        .len()
        .checked_sub(offset)
        .ok_or("binary source offset is outside file")?;
    let length = entry
        .get("source_length")
        .map(|v| number(v, "binary source length"))
        .transpose()?
        .unwrap_or(available);
    let end = offset
        .checked_add(length)
        .ok_or("binary source extent overflows")?;
    Ok(data
        .get(offset..end)
        .ok_or("binary source extent is outside file")?
        .to_vec())
}
/// Select `frame` of an array that stores a sequence of `frame_size` frames.
fn array_frame(entry: &Value, data: Vec<u8>) -> Result<Vec<u8>, String> {
    let Some(size) = entry.get("frame_size") else {
        return Ok(data);
    };
    let size = number(size, "array frame size")?;
    let frame = number(&entry["frame"], "array frame")?;
    if size == 0 || data.len() % size != 0 {
        return Err("array frames have a partial extent".into());
    }
    let start = frame.checked_mul(size).ok_or("array frame overflows")?;
    let end = start.checked_add(size).ok_or("array frame end overflows")?;
    Ok(data
        .get(start..end)
        .ok_or("array frame exceeds source")?
        .to_vec())
}
fn tile_coordinates(
    width: usize,
    height: usize,
    entry: &Value,
) -> Result<Vec<(usize, usize)>, String> {
    if width % 8 != 0 || height % 8 != 0 {
        return Err("tile dimensions require whole tiles".into());
    }
    let mut coordinates = vec![];
    if let Some(frames) = entry.get("frames") {
        let frames = number(frames, "tile frames")?;
        let columns = number(&entry["columns"], "tile columns")?;
        let w = number(&entry["frame_tiles_wide"], "frame tile width")?
            .checked_mul(8)
            .ok_or("frame width overflows")?;
        let h = number(&entry["frame_tiles_high"], "frame tile height")?
            .checked_mul(8)
            .ok_or("frame height overflows")?;
        if frames == 0
            || columns == 0
            || w == 0
            || h == 0
            || columns.checked_mul(w) != Some(width)
            || frames.div_ceil(columns).checked_mul(h) != Some(height)
        {
            return Err("tile frame atlas dimensions differ".into());
        }
        for frame in 0..frames {
            for y in (0..h).step_by(8) {
                for x in (0..w).step_by(8) {
                    coordinates.push((frame % columns * w + x, frame / columns * h + y));
                }
            }
        }
    } else {
        for y in (0..height).step_by(8) {
            for x in (0..width).step_by(8) {
                coordinates.push((x, y));
            }
        }
    }
    Ok(coordinates)
}

#[cfg(test)]
fn build_component(root: &Path, entry: &Value) -> Result<ComponentResult, String> {
    build_component_cached(&Context::new(root), entry)
}
fn build_component_cached(ctx: &Context, entry: &Value) -> Result<ComponentResult, String> {
    let root = &ctx.root;
    let kind = json_string(&entry["kind"], "component kind")?;
    let source_name = json_string(&entry["source"], "component source")?;
    let source = root_path(root, source_name)?;
    let (data, details, sources) = match kind {
        "bgr555-banks" => {
            let data = palette_banks(root, entry)?;
            let colors = data.len() / 2;
            (
                data,
                serde_json::json!({"palette_entries":colors}),
                vec![source_name.into()],
            )
        }
        "golden-sun-map-grid" => {
            let width = number(&entry["width"], "grid width")?;
            let height = number(&entry["height"], "grid height")?;
            if width != 128 || height != 128 {
                return Err("Golden Sun grid must contain 128x128 cells".into());
            }
            let data = binary_source(&source, entry)?;
            if data.len() != width * height * 4 {
                return Err("Golden Sun grid must contain two u8 planes and one u16 plane".into());
            }
            (
                data,
                serde_json::json!({"width":width,"height":height,"planes":["u8","u8","le-u16"]}),
                vec![source_name.to_string()],
            )
        }
        // Bytes of unknown structure come from a registered private binary,
        // already in their stored element order.
        "u8-array" | "s8-array" | "be-s16-array" | "le-u16-array" | "le-u32-array"
            if entry.get("format").and_then(Value::as_str) == Some("binary") =>
        {
            let data = binary_source(&source, entry)?;
            let width = match kind {
                "u8-array" | "s8-array" => 1,
                "le-u32-array" => 4,
                _ => 2,
            };
            if data.len() % width != 0 {
                return Err("binary array has a partial element".into());
            }
            (
                array_frame(entry, data)?,
                serde_json::json!({"format":"binary"}),
                vec![source_name.to_string()],
            )
        }
        "u8-array" | "s8-array" | "be-s16-array" | "le-u16-array" | "le-u32-array" => {
            let document = json(&source)?;
            let pointer = json_string(&entry["pointer"], "array pointer")?;
            let values = document.pointer(pointer).ok_or("array pointer is absent")?;
            (
                array_frame(entry, integer_array(values, kind)?)?,
                serde_json::json!({"pointer":pointer}),
                vec![source_name.to_string()],
            )
        }
        // A typed table inside a compressed stream: its segment addresses
        // are offsets in the decoded stream.
        "typed-table" => {
            let document = ctx.document(&source)?;
            let pointer = json_string(&entry["pointer"], "typed table pointer")?;
            let table = document
                .pointer(pointer)
                .ok_or("typed table pointer is absent")?;
            (
                typed_table(table)?,
                serde_json::json!({"pointer":pointer}),
                vec![source_name.to_string()],
            )
        }
        "gba-4bpp-object-bank" => {
            let result = build_object_bank(root, &source)?;
            return Ok(result);
        }
        "gba-4bpp-tiles" | "gba-8bpp-tiles" | "gba-palette" => {
            if matches!(kind, "gba-8bpp-tiles" | "gba-4bpp-tiles")
                && entry.get("source_rect").is_some()
            {
                let image = ctx.indexed(&source)?;
                let (width, height, pixels) = indexed_rect(&image, entry)?;
                if width % 8 != 0 || height % 8 != 0 {
                    return Err("tiled atlas must contain whole tiles".into());
                }
                let mut data = Vec::with_capacity(pixels.len());
                for (x, y) in tile_coordinates(width, height, entry)? {
                    for row in 0..8 {
                        let values = &pixels[(y + row) * width + x..(y + row) * width + x + 8];
                        if kind == "gba-4bpp-tiles" {
                            if values.iter().any(|p| *p > 15) {
                                return Err("four-bit tile pixel exceeds palette".into());
                            }
                            data.extend(values.chunks_exact(2).map(|p| p[0] | p[1] << 4));
                        } else {
                            data.extend_from_slice(values);
                        }
                    }
                }
                if let Some(canvas) = entry.get("canvas_size") {
                    let size = number(&entry["size"], "component size")?;
                    if data.len() != number(canvas, "canvas size")?
                        || size > data.len()
                        || data[size..].iter().any(|p| *p != 0)
                    {
                        return Err("tile canvas carries data outside stored extent".into());
                    }
                    data.truncate(size);
                }
                if let Some(offset) = entry.get("tile_offset") {
                    let bytes = if kind == "gba-4bpp-tiles" { 32 } else { 64 };
                    let start = number(offset, "tile offset")?
                        .checked_mul(bytes)
                        .ok_or("tile offset overflows")?;
                    let length = number(&entry["tile_count"], "tile count")?
                        .checked_mul(bytes)
                        .ok_or("tile count overflows")?;
                    let end = start.checked_add(length).ok_or("tile extent overflows")?;
                    data = data
                        .get(start..end)
                        .ok_or("tile extent exceeds canvas")?
                        .to_vec();
                }
                return Ok(ComponentResult {
                    data,
                    sources: vec![source_name.into()],
                    details: serde_json::json!({"width":width,"height":height,"bpp":if kind=="gba-4bpp-tiles"{4}else{8}}),
                });
            }
            let bpp = if kind == "gba-4bpp-tiles" {
                GbaBpp::Bpp4
            } else {
                GbaBpp::Bpp8
            };
            let (graphics, palette, size) =
                gba_graphics(&fs::read(&source).map_err(|e| e.to_string())?, bpp)
                    .map_err(|e| e.to_string())?;
            let details = serde_json::json!({
                "width": size.width, "height": size.height,
                "bpp": if bpp == GbaBpp::Bpp4 { 4 } else { 8 },
                "tiles": size.width as usize / 8 * (size.height as usize / 8),
                "palette_entries": palette.len() / 2,
            });
            let mut built = if kind == "gba-palette" {
                palette
            } else {
                graphics
            };
            if entry.get("frames").is_some() {
                if kind != "gba-4bpp-tiles" && kind != "gba-8bpp-tiles" {
                    return Err("atlas requires tiled pixels".into());
                }
                let image = indexed_png(&fs::read(&source).map_err(|e| e.to_string())?)
                    .map_err(|e| e.to_string())?;
                let frames = number(&entry["frames"], "frames")?;
                let columns = number(&entry["columns"], "columns")?;
                let w = number(&entry["frame_tiles_wide"], "frame tile width")?;
                let h = number(&entry["frame_tiles_high"], "frame tile height")?;
                if frames == 0
                    || columns == 0
                    || w == 0
                    || h == 0
                    || columns.checked_mul(w).and_then(|n| n.checked_mul(8))
                        != Some(image.width as usize)
                    || frames
                        .div_ceil(columns)
                        .checked_mul(h)
                        .and_then(|n| n.checked_mul(8))
                        != Some(image.height as usize)
                {
                    return Err("atlas dimensions differ".into());
                }
                if entry["symbolic_palette"] == true {
                    let count = if bpp == GbaBpp::Bpp4 { 16 } else { 256 };
                    let expected = (0..count)
                        .map(|i| {
                            let v = (if count == 16 { i * 8 } else { i }) as u8;
                            [v, v, v]
                        })
                        .collect::<Vec<_>>();
                    if image.palette != expected {
                        return Err("symbolic atlas palette differs".into());
                    }
                }
                let tile_bytes = if bpp == GbaBpp::Bpp4 { 32 } else { 64 };
                let mut ordered = Vec::new();
                for frame in 0..frames {
                    for y in 0..h {
                        let start = ((frame / columns * h + y) * columns * w + frame % columns * w)
                            * tile_bytes;
                        ordered.extend_from_slice(&built[start..start + w * tile_bytes]);
                    }
                }
                if let Some(frame) = entry.get("frame") {
                    let frame = number(frame, "frame")?;
                    let parts = entry
                        .get("frames_per_image")
                        .map(|value| number(value, "frames per image"))
                        .transpose()?
                        .unwrap_or(1);
                    if parts == 0 || frames % parts != 0 || frame >= frames / parts {
                        return Err("frame lies outside its atlas".into());
                    }
                    let bytes = w * h * tile_bytes * parts;
                    ordered = ordered[frame * bytes..(frame + 1) * bytes].to_vec();
                }
                built = ordered;
            }
            if let Some(canvas) = entry.get("canvas_size") {
                let size = number(&entry["size"], "component size")?;
                if built.len() != number(canvas, "canvas size")?
                    || built.len() < size
                    || built[size..].iter().any(|&byte| byte != 0)
                {
                    return Err("canvas differs or carries data beyond the extent".into());
                }
                built.truncate(size);
            }
            if let Some(offset) = entry.get("tile_offset") {
                if kind == "gba-palette" || entry.get("frames").is_some() {
                    return Err("tile section requires an ordinary tiled image".into());
                }
                let tile_bytes = if bpp == GbaBpp::Bpp4 { 32 } else { 64 };
                let start = number(offset, "tile offset")?
                    .checked_mul(tile_bytes)
                    .ok_or("tile offset overflows")?;
                let length = number(&entry["tile_count"], "tile count")?
                    .checked_mul(tile_bytes)
                    .ok_or("tile count overflows")?;
                let end = start.checked_add(length).ok_or("tile extent overflows")?;
                if length == 0 {
                    return Err("tile section must be nonempty".into());
                }
                built = built
                    .get(start..end)
                    .ok_or("tile section is outside sheet")?
                    .to_vec();
            }
            (built, details, vec![source_name.to_string()])
        }
        "gba-palette-rgba" => {
            let (built, size) = gba_palette_rgba(&fs::read(&source).map_err(|e| e.to_string())?)
                .map_err(|e| e.to_string())?;
            let details = serde_json::json!({
                "width": size.width, "height": size.height, "palette_entries": built.len() / 2,
            });
            (built, details, vec![source_name.to_string()])
        }
        "1bpp-tiles" => {
            let (built, size) = one_bit_tiles(&fs::read(&source).map_err(|e| e.to_string())?)
                .map_err(|e| e.to_string())?;
            let details = serde_json::json!({
                "width": size.width, "height": size.height, "bpp": 1, "tiles": built.len() / 8,
            });
            (built, details, vec![source_name.to_string()])
        }
        "gba-tilemap16" => {
            // The tilemap text is the whole source file, or one text value of
            // a JSON source selected by `pointer`.
            let binary = entry.get("format").and_then(Value::as_str) == Some("binary");
            let text = if binary {
                String::new()
            } else if let Some(pointer) = entry.get("pointer") {
                let document = json(&source)?;
                json_string(
                    document
                        .pointer(json_string(pointer, "tilemap pointer")?)
                        .ok_or("tilemap pointer is absent")?,
                    "tilemap text",
                )?
                .to_string()
            } else {
                fs::read_to_string(&source).map_err(|e| e.to_string())?
            };
            let entries = if binary {
                let data = binary_source(&source, entry)?;
                if data.is_empty() || data.len() % 2 != 0 {
                    return Err("binary tilemap needs complete nonempty u16 words".into());
                }
                data
            } else {
                import_tilemap(&text).map_err(|e| e.to_string())?
            };
            let built = if let Some(mode) = entry.get("delta_mode") {
                let mode = u8::try_from(number(mode, "tilemap delta mode")?)
                    .map_err(|_| "tilemap delta mode exceeds u8")?;
                psynergy::assets::compression::encode_tilemap_delta(&entries, mode)
                    .map_err(|e| e.to_string())?
            } else {
                entries.clone()
            };
            (
                built,
                serde_json::json!({"entries": entries.len() / 2, "delta_mode": entry.get("delta_mode")}),
                vec![source_name.to_string()],
            )
        }
        "indexed-bytes" | "raw-lz-bytes" | "zero-skip-bytes" | "zero-skip-bank" | "mtf4-bytes" => {
            let image = ctx.indexed(&source)?;
            let mut sources = vec![source_name.to_string()];
            sources.extend(ctx.shared_palette(&source, entry, &image)?);
            let (width, height, pixels) = indexed_rect(&image, entry)?;
            let built = if kind == "zero-skip-bank" {
                let frames = component_frames(entry, width, height, &pixels, 1)?;
                let base = u32::try_from(number(&entry["address"], "bank address")?)
                    .map_err(|_| "bank address exceeds address space")?;
                psynergy::assets::compression::encode_zero_skip_bank(&frames, base)
                    .map_err(|e| e.to_string())?
            } else if kind == "raw-lz-bytes" {
                let mut built = pixels;
                built.truncate(number(&entry["size"], "component size")?);
                built
            } else if kind == "indexed-bytes" && entry.get("frame_width").is_none() {
                psynergy::assets::image::indexed_bytes(
                    &fs::read(&source).map_err(|e| e.to_string())?,
                    number(&entry["size"], "component size")?,
                )
                .map_err(|error| error.to_string())?
            } else if kind == "zero-skip-bytes" && is_absent_frame(entry)? {
                // An absent frame stores no zero-skip bytes; its cell is blank.
                if component_frames(entry, width, height, &pixels, 1)?
                    .iter()
                    .flatten()
                    .any(|pixel| *pixel != 0)
                {
                    return Err("absent frame has pixels".into());
                }
                Vec::new()
            } else {
                let mut built = Vec::new();
                for frame in component_frames(entry, width, height, &pixels, 1)? {
                    built.extend(match kind {
                        "zero-skip-bytes" => {
                            psynergy::assets::compression::encode_zero_skip(&frame)
                                .map_err(|e| e.to_string())?
                        }
                        "mtf4-bytes" => psynergy::assets::compression::encode_mtf4(&frame)
                            .map_err(|e| e.to_string())?,
                        _ => frame,
                    });
                }
                built
            };
            let bytes = built.len();
            (
                built,
                serde_json::json!({"width": width, "height": height, "bytes": bytes}),
                sources,
            )
        }
        "rgba-bytes" => {
            let image = rgba_png(&fs::read(&source).map_err(|e| e.to_string())?)
                .map_err(|e| e.to_string())?;
            let built = component_frames(
                entry,
                image.width as usize,
                image.height as usize,
                &image.pixels,
                4,
            )?
            .concat();
            let pixels = built.len() / 4;
            (
                built,
                serde_json::json!({"width": image.width, "height": image.height, "pixels": pixels}),
                vec![source_name.to_string()],
            )
        }
        "little-u16-text" | "little-u16-pairs" => {
            let text = fs::read_to_string(&source).map_err(|error| error.to_string())?;
            let data = if kind == "little-u16-text" {
                psynergy::assets::text::import_words(&text)?
            } else {
                psynergy::assets::text::import_pairs(&text)?
            };
            (data, serde_json::json!({}), vec![source_name.to_string()])
        }
        "golden-sun-thumb-overlay" => {
            let base = number(&entry["base"], "overlay base")?;
            let built = assemble_overlay(&OverlaySource::path(&source), base as i64)
                .map_err(|e| e.to_string())?;
            (
                built.clone(),
                serde_json::json!({"base": hex_address(base), "bytes": built.len()}),
                vec![source_name.to_string()],
            )
        }
        _ => return Err(format!("unsupported asset component: {kind}")),
    };
    if let Some(expected) = entry.get("size") {
        let expected = number(expected, "component size")?;
        if data.len() != expected {
            return Err(format!(
                "{source_name}: built 0x{:x}, expected 0x{:x}",
                data.len(),
                expected
            ));
        }
    }
    Ok(ComponentResult {
        data,
        sources: dedup_sources(sources),
        details,
    })
}

#[test]
fn binary_map_components_preserve_words_and_refuse_wrong_extents() {
    let root = tempfile::tempdir().unwrap();
    fs::write(root.path().join("metatiles.bin"), [0x21, 0xf4, 0x32, 0x18]).unwrap();
    let entry = serde_json::json!({"kind":"gba-tilemap16","format":"binary","source":"metatiles.bin","size":4});
    assert_eq!(
        build_component(root.path(), &entry).unwrap().data,
        [0x21, 0xf4, 0x32, 0x18]
    );
    fs::write(root.path().join("metatiles.bin"), [0x21, 0xf4, 0x32]).unwrap();
    assert!(build_component(root.path(), &entry).is_err());
    let entry = serde_json::json!({"kind":"golden-sun-map-grid","source":"map.bin","width":128,"height":128,"size":65536});
    fs::write(root.path().join("map.bin"), vec![0; 65535]).unwrap();
    assert!(build_component(root.path(), &entry).is_err());
    let mut source = vec![0; 65536];
    source[32768..32770].copy_from_slice(&[0x21, 0xf4]);
    fs::write(root.path().join("map.bin"), &source).unwrap();
    assert_eq!(build_component(root.path(), &entry).unwrap().data, source);
    source.extend([0x21, 0xf4, 0x32, 0x18]);
    fs::write(root.path().join("map.bin"), &source).unwrap();
    let packed = serde_json::json!({"kind":"gba-tilemap16","format":"binary","source":"map.bin","size":4,"source_offset":65536,"source_length":4});
    assert_eq!(
        build_component(root.path(), &packed).unwrap().data,
        [0x21, 0xf4, 0x32, 0x18]
    );
    let grid = serde_json::json!({"kind":"golden-sun-map-grid","source":"map.bin","width":128,"height":128,"size":65536,"source_offset":0,"source_length":65536});
    assert_eq!(
        build_component(root.path(), &grid).unwrap().data,
        &source[..65536]
    );
    let mut invalid = packed.clone();
    invalid["source_length"] = serde_json::json!(5);
    assert!(build_component(root.path(), &invalid).is_err());
    invalid["source_offset"] = serde_json::json!(usize::MAX);
    assert!(build_component(root.path(), &invalid).is_err());
    // An odd-length private layer is plain bytes, not tilemap words.
    let layer = serde_json::json!({"kind":"u8-array","format":"binary","source":"map.bin","size":3,"source_offset":65537,"source_length":3});
    assert_eq!(
        build_component(root.path(), &layer).unwrap().data,
        [0xf4, 0x32, 0x18]
    );
    let mut invalid = layer.clone();
    invalid["source_length"] = serde_json::json!(4);
    assert!(build_component(root.path(), &invalid).is_err());
    // Private word tables keep stored order; a frame sequence selects whole frames.
    let words = serde_json::json!({"kind":"le-u16-array","format":"binary","source":"map.bin","size":4,"source_offset":65536,"source_length":4});
    assert_eq!(
        build_component(root.path(), &words).unwrap().data,
        [0x21, 0xf4, 0x32, 0x18]
    );
    let mut invalid = words.clone();
    invalid["source_length"] = serde_json::json!(3);
    assert!(build_component(root.path(), &invalid).is_err());
    let mut frames = words.clone();
    frames["kind"] = serde_json::json!("u8-array");
    frames["frame_size"] = serde_json::json!(2);
    frames["frame"] = serde_json::json!(1);
    frames["size"] = serde_json::json!(2);
    assert_eq!(
        build_component(root.path(), &frames).unwrap().data,
        [0x32, 0x18]
    );
    frames["frame"] = serde_json::json!(2);
    assert!(build_component(root.path(), &frames).is_err());
}

#[test]
fn shared_tile_sheets_select_whole_banks_and_reject_invalid_sections() {
    let root = tempfile::tempdir().unwrap();
    let pixels = [vec![0x13; 32], vec![0x57; 32]].concat();
    let palette = (0u16..16)
        .flat_map(|i| (i | i << 5 | i << 10).to_le_bytes())
        .collect::<Vec<_>>();
    let image =
        psynergy::assets::image::png_from_gba_tiles(&pixels, &palette, GbaBpp::Bpp4, 2).unwrap();
    fs::write(root.path().join("tiles.png"), image).unwrap();
    let entry = serde_json::json!({"kind":"gba-4bpp-tiles","source":"tiles.png","tile_offset":1,"tile_count":1,"size":32});
    assert_eq!(
        build_component(root.path(), &entry).unwrap().data,
        vec![0x57; 32]
    );
    for (offset, count) in [(2, 1), (1, 0), (usize::MAX, 1), (1, usize::MAX)] {
        let mut invalid = entry.clone();
        invalid["tile_offset"] = Value::from(offset);
        invalid["tile_count"] = Value::from(count);
        assert!(build_component(root.path(), &invalid).is_err());
    }
}
/// An integer array member: a JSON integer, or hexadecimal text such as
/// `"0x3c"` for values that read better in the radix their consumer uses.
fn array_member(value: &Value) -> Result<i64, String> {
    if value.is_string() {
        return i64::try_from(number(value, "array member")?)
            .map_err(|_| "array member exceeds i64".into());
    }
    value
        .as_i64()
        .ok_or("array member is not an integer".into())
}
#[cfg(test)]
fn test_png(width: u32, height: u32, palette: &[u8], pixels: &[u8]) -> Vec<u8> {
    let mut image = Vec::new();
    let mut encoder = png::Encoder::new(&mut image, width, height);
    encoder.set_color(png::ColorType::Indexed);
    encoder.set_depth(png::BitDepth::Eight);
    encoder.set_palette(palette.to_vec());
    encoder
        .write_header()
        .unwrap()
        .write_image_data(pixels)
        .unwrap();
    image
}
#[test]
fn atlas_frames_select_order_and_feed_pixel_codecs() {
    let root = tempfile::tempdir().unwrap();
    let mut pixels = vec![0u8; 16 * 8];
    for y in 0..8 {
        pixels[y * 16] = 1;
        pixels[y * 16 + 8] = 2;
    }
    fs::write(
        root.path().join("atlas.png"),
        test_png(16, 8, &[0, 0, 0, 8, 8, 8, 16, 16, 16], &pixels),
    )
    .unwrap();
    fs::write(
        root.path().join("shared.png"),
        test_png(
            8,
            8,
            &[0, 0, 0, 8, 8, 8, 16, 16, 16],
            &[0, 1, 2, 0, 0, 0, 0, 0].repeat(8),
        ),
    )
    .unwrap();
    let entry = serde_json::json!({"kind":"zero-skip-bytes","source":"atlas.png","frame_width":8,"frame_height":8,"columns":2,"frame_order":[1,0],
        "palette":{"source":"shared.png","offset":0,"entries":3}});
    let result = build_component(root.path(), &entry).unwrap();
    let frame = |value: u8| [[value, 0xe6].repeat(7), vec![value, 0xe6, 0]].concat();
    assert_eq!(result.data, [frame(2), frame(1)].concat());
    assert_eq!(result.sources, ["atlas.png", "shared.png"]);
    let mut mtf = entry.clone();
    mtf["kind"] = Value::from("mtf4-bytes");
    mtf.as_object_mut().unwrap().remove("frame_order");
    mtf["frames"] = Value::from(2);
    let result = build_component(root.path(), &mtf).unwrap();
    let frame_pixels = |value: u8| {
        let mut pixels = vec![0u8; 64];
        for y in 0..8 {
            pixels[y * 8] = value;
        }
        pixels
    };
    assert_eq!(
        result.data,
        [
            psynergy::assets::compression::encode_mtf4(&frame_pixels(1)).unwrap(),
            psynergy::assets::compression::encode_mtf4(&frame_pixels(2)).unwrap()
        ]
        .concat()
    );
    for (pointer, value) in [
        ("/frame_order", serde_json::json!([2])),
        ("/columns", serde_json::json!(3)),
        ("/palette/entries", serde_json::json!(2)),
    ] {
        let mut bad = entry.clone();
        *bad.pointer_mut(pointer).unwrap() = value;
        assert!(build_component(root.path(), &bad).is_err(), "{pointer}");
    }
    let mut bad = entry;
    bad["size"] = Value::from(1);
    assert!(build_component(root.path(), &bad).is_err());
}
#[test]
fn typed_table_components_serialize_tables_inside_streams() {
    let root = tempfile::tempdir().unwrap();
    let table = serde_json::json!({"format":1,"kind":"typed-table","address":"0x0","size":"0x7","segments":[
        {"name":"offsets","address":"0x0","end":"0x2","stride":2,"element":"le-u16","values":["entries_end"]},
        {"name":"entries","address":"0x2","end":"0x6","stride":2,"element":"record",
            "fields":[{"name":"entry","element":"s8"},{"name":"value","element":"u8"}],
            "records":[{"entry":-1,"value":7},{"entry":2,"value":9}]},
        {"name":"entries_end","address":"0x6","end":"0x7","stride":1,"element":"u8","values":[255]}]});
    fs::write(
        root.path().join("map.json"),
        serde_json::to_vec(&serde_json::json!({"maps":{"1":{"table":table}}})).unwrap(),
    )
    .unwrap();
    let entry = serde_json::json!({"kind":"typed-table","source":"map.json","pointer":"/maps/1/table","size":7});
    assert_eq!(
        build_component(root.path(), &entry).unwrap().data,
        [6, 0, 255, 7, 2, 9, 255]
    );
    for (key, value) in [
        ("pointer", serde_json::json!("/maps/1/absent")),
        ("size", serde_json::json!(8)),
    ] {
        let mut wrong = entry.clone();
        wrong[key] = value;
        assert!(build_component(root.path(), &wrong).is_err(), "{key}");
    }
}
#[test]
fn component_regions_concatenate_parts_at_running_addresses() {
    let root = tempfile::tempdir().unwrap();
    fs::write(
        root.path().join("parts.json"),
        r#"{"components":[{"kind":"byte-fill","value":7,"size":2},{"kind":"byte-fill","value":9,"size":1,"address":"0x08000014"}]}"#,
    )
    .unwrap();
    let mut ctx = Context::new(root.path());
    let entry = serde_json::json!({"address":"0x08000010","size":5,"kind":"components","components":[
        {"kind":"byte-fill","value":1,"size":2},
        {"kind":"components","source":"parts.json","size":3}
    ]});
    let (built, sources, details) = build_entry(&mut ctx, &entry).unwrap();
    assert_eq!(built, [1, 1, 7, 7, 9]);
    assert_eq!(sources, ["parts.json"]);
    assert_eq!(details["components"].as_array().unwrap().len(), 2);
    assert_eq!(
        details["components"][1]["sources"],
        serde_json::json!(["parts.json"])
    );
    let mut bad = entry.clone();
    bad["components"][0]["address"] = Value::from("0x08000011");
    assert!(build_entry(&mut ctx, &bad).is_err());
    bad = entry.clone();
    bad["components"][1]["size"] = Value::from(4);
    assert!(build_entry(&mut ctx, &bad).is_err());
}
#[test]
fn typed_table_bitmap_fields_pack_rows_from_the_atlas() {
    let root = tempfile::tempdir().unwrap();
    let mut pixels = vec![0u8; 16 * 16];
    pixels[0] = 1;
    pixels[16 + 15] = 1;
    fs::write(
        root.path().join("glyphs.png"),
        test_png(16, 16, &[0, 0, 0, 255, 255, 255], &pixels),
    )
    .unwrap();
    let mut document = serde_json::json!({"format":1,"kind":"typed-table","address":0,"size":32,"segments":[
        {"address":0,"end":32,"stride":32,"element":"record",
         "image":{"source":"glyphs.png","frame_width":16,"frame_height":16,"columns":1,"frames":1},
         "fields":[{"name":"advance","element":"le-u16"},{"name":"glyph","element":"1bpp-rows","rows":15}],
         "records":[{"advance":6,"glyph":0}]}]});
    let blank = document.clone();
    assert_eq!(
        resolve_table_bitmaps(&mut document, root.path()).unwrap(),
        ["glyphs.png"]
    );
    let mut expected = vec![6, 0, 0, 0x80, 1, 0];
    expected.resize(32, 0);
    assert_eq!(typed_table(&document).unwrap(), expected);
    pixels[15 * 16] = 1;
    fs::write(
        root.path().join("glyphs.png"),
        test_png(16, 16, &[0, 0, 0, 255, 255, 255], &pixels),
    )
    .unwrap();
    let mut dirty = blank;
    assert!(resolve_table_bitmaps(&mut dirty, root.path()).is_err());
}
#[test]
fn general_lz_sequences_encode_one_stream_per_frame() {
    let root = tempfile::tempdir().unwrap();
    let mut pixels = vec![0u8; 16 * 8];
    pixels[0] = 3;
    pixels[8] = 5;
    fs::write(
        root.path().join("atlas.png"),
        test_png(
            16,
            8,
            &[
                0, 0, 0, 8, 8, 8, 16, 16, 16, 24, 24, 24, 32, 32, 32, 40, 40, 40,
            ],
            &pixels,
        ),
    )
    .unwrap();
    fs::write(
        root.path().join("plan.json"),
        r#"{"streams":[{"codec":"golden-sun-arena-lz","decoded_size":4,"encoded_size":6},{"codec":"golden-sun-arena-lz","decoded_size":4,"encoded_size":6}],
            "components":[{"kind":"zero-skip-bytes","source":"atlas.png","frame_width":8,"frame_height":8,"columns":2}]}"#,
    )
    .unwrap();
    let entry = serde_json::json!({"address":0,"size":16,"kind":"golden-sun-general-lz","plan":"plan.json","plan_section":"/streams","stream_alignment":8});
    let (built, sources, details) = build_general_lz(root.path(), &entry).unwrap();
    assert_eq!(
        built,
        [0, 0, 3, 0xff, 0xfe, 0, 0, 0, 0, 0, 5, 0xff, 0xfe, 0, 0, 0]
    );
    assert_eq!(sources, ["atlas.png", "plan.json"]);
    assert_eq!(details["streams"], 2);
    let mut bad = entry.clone();
    bad["plan_section"] = Value::from("/missing");
    assert!(build_general_lz(root.path(), &bad).is_err());
    // Arena controls come from the compressor; a stored plan is refused.
    let stored =
        serde_json::json!({"codec":"golden-sun-arena-lz","decoded_size":4,"tokens":[["l",4]]});
    assert!(encode_lz_stream(&[3, 0xff, 0xfe, 0], &stored, &[], None)
        .unwrap_err()
        .contains("stored controls are refused"));
    // A tagged stream derives its tag; a stored choice is refused.
    for stored in [
        serde_json::json!({"codec":"golden-sun-tagged-lz","decoded_size":4,"tag":1}),
        serde_json::json!({"codec":"golden-sun-tagged-lz","decoded_size":4,"tokens":[["l",4]]}),
    ] {
        assert!(encode_lz_stream(&[3, 0xff, 0xfe, 0], &stored, &[], None)
            .unwrap_err()
            .contains("stored choices are refused"));
    }
}
#[test]
fn absent_frames_store_no_bytes_and_keep_blank_cells() {
    let root = tempfile::tempdir().unwrap();
    let mut pixels = vec![0u8; 16 * 8];
    pixels[0] = 3;
    let palette = [0, 0, 0, 8, 8, 8, 16, 16, 16, 24, 24, 24];
    fs::write(
        root.path().join("atlas.png"),
        test_png(16, 8, &palette, &pixels),
    )
    .unwrap();
    fs::write(
        root.path().join("plan.json"),
        r#"{"streams":[{"codec":"golden-sun-arena-lz","decoded_size":4,"encoded_size":6},{"codec":"golden-sun-arena-lz","decoded_size":0,"encoded_size":2}],
            "components":[{"kind":"zero-skip-bytes","source":"atlas.png","frame_width":8,"frame_height":8,"columns":2,"absent_frames":[1]}]}"#,
    )
    .unwrap();
    let entry = serde_json::json!({"address":0,"size":8,"kind":"golden-sun-general-lz","plan":"plan.json","plan_section":"/streams"});
    let (built, _, _) = build_general_lz(root.path(), &entry).unwrap();
    // The absent frame is a bare raw split after the first frame's stream.
    assert_eq!(built, [0, 0, 3, 0xff, 0xfe, 0, 0, 0]);
    // An absent cell that holds pixels is refused.
    pixels[8] = 2;
    fs::write(
        root.path().join("atlas.png"),
        test_png(16, 8, &palette, &pixels),
    )
    .unwrap();
    assert!(build_general_lz(root.path(), &entry)
        .unwrap_err()
        .contains("absent frame has pixels"));
    // Only a component built one frame per stream can name absent frames.
    let whole = serde_json::json!({"kind":"zero-skip-bytes","source":"atlas.png","frame_width":8,
        "frame_height":8,"columns":2,"frames":2,"absent_frames":[1]});
    assert!(build_component(root.path(), &whole).is_err());
    for bad in [
        serde_json::json!([]),
        serde_json::json!([1, 1]),
        serde_json::json!([1, 0]),
    ] {
        let mut entry = whole.clone();
        entry["absent_frames"] = bad;
        entry["frame"] = Value::from(1);
        assert!(absent_frames(&entry).is_err());
    }
}
fn integer_array(value: &Value, kind: &str) -> Result<Vec<u8>, String> {
    let mut output = Vec::new();
    for value in value.as_array().ok_or("integer array is not an array")? {
        if value.is_array() {
            output.extend(integer_array(value, kind)?);
        } else {
            let value = array_member(value)?;
            if kind == "be-s16-array" || kind == "le-s16-array" {
                let value = i16::try_from(value).map_err(|_| "array member exceeds s16")?;
                output.extend(if kind == "le-s16-array" {
                    value.to_le_bytes()
                } else {
                    value.to_be_bytes()
                });
            } else if kind == "s8-array" {
                output.push(i8::try_from(value).map_err(|_| "array member exceeds s8")? as u8);
            } else if kind == "u8-array" {
                output.push(u8::try_from(value).map_err(|_| "array member exceeds u8")?);
            } else if kind == "le-u16-array" {
                output.extend(
                    u16::try_from(value)
                        .map_err(|_| "array member exceeds u16")?
                        .to_le_bytes(),
                );
            } else if kind == "le-s32-array" {
                output.extend(
                    i32::try_from(value)
                        .map_err(|_| "array member exceeds s32")?
                        .to_le_bytes(),
                );
            } else if kind == "le-u32-array" {
                output.extend(
                    u32::try_from(value)
                        .map_err(|_| "array member exceeds u32")?
                        .to_le_bytes(),
                );
            } else {
                return Err("unknown integer encoding".into());
            }
        }
    }
    Ok(output)
}

#[test]
fn integer_arrays_preserve_endianness_sign_and_order() {
    assert_eq!(
        integer_array(&serde_json::json!([[-128, 127], [0, -1]]), "s8-array").unwrap(),
        [128, 127, 0, 255]
    );
    assert_eq!(
        integer_array(&serde_json::json!([160, -39]), "be-s16-array").unwrap(),
        [0, 160, 255, 217]
    );
    for value in [
        serde_json::json!([128]),
        serde_json::json!([-129]),
        serde_json::json!([1.5]),
        serde_json::json!([null]),
        serde_json::json!({}),
    ] {
        assert!(integer_array(&value, "s8-array").is_err());
    }
    assert!(integer_array(&serde_json::json!([32768]), "be-s16-array").is_err());
    assert!(integer_array(&serde_json::json!([-32769]), "be-s16-array").is_err());
    assert_eq!(
        integer_array(&serde_json::json!([[0], [255]]), "u8-array").unwrap(),
        [0, 255]
    );
    for value in [
        serde_json::json!([-1]),
        serde_json::json!([256]),
        serde_json::json!([1.5]),
        serde_json::json!([null]),
    ] {
        assert!(integer_array(&value, "u8-array").is_err());
    }
}
fn record_table(document: &Value) -> Result<Vec<u8>, String> {
    let fields = document["fields"]
        .as_array()
        .ok_or("record fields missing")?;
    if fields.is_empty() {
        return Err("empty record layout".into());
    }
    let kind = json_string(&document["element"], "record element")?;
    let bias = number(&document["bias"], "record bias")?;
    let radix = number(&document["radix"], "record radix")?;
    if !(2..=36).contains(&radix) {
        return Err("invalid record radix".into());
    }
    let index_field = json_string(&document["index_field"], "record index field")?;
    let mut values = Vec::new();
    for (i, record) in document["records"]
        .as_array()
        .ok_or("records missing")?
        .iter()
        .enumerate()
    {
        if number(&record[index_field], "record index")? != i {
            return Err("record indices are not sequential".into());
        }
        for field in fields {
            let name = json_string(field, "record field")?;
            let value = u64::from_str_radix(json_string(&record[name], name)?, radix as u32)
                .map_err(|e| e.to_string())?;
            let value = value
                .checked_sub(bias as u64)
                .ok_or("record value is below bias")?;
            values.push(Value::from(value));
        }
    }
    integer_array(&Value::Array(values), kind)
}

fn pointer_table(document: &Value) -> Result<Vec<u8>, String> {
    let base = u32::try_from(number(&document["base_address"], "pointer base")?)
        .map_err(|_| "pointer base exceeds u32")?;
    let address = u32::try_from(number(&document["address"], "pointer table address")?)
        .map_err(|_| "table address exceeds u32")?;
    let slots = document["slots"]
        .as_array()
        .ok_or("pointer slots missing")?;
    if slots.len() != number(&document["slot_count"], "pointer count")? {
        return Err("pointer count differs".into());
    }
    let mut resolved = Vec::<u32>::new();
    for slot in slots {
        let text = json_string(slot, "pointer slot")?;
        let value = match text {
            "base" => base,
            "self" => address,
            "null" => 0,
            _ if text.starts_with("alias:") => {
                let target = number(&Value::String(text[6..].into()), "alias index")?;
                let value = *resolved
                    .get(target)
                    .ok_or("alias must reference an earlier slot")?;
                if value == 0 {
                    return Err("alias references a null slot".into());
                }
                value
            }
            _ => {
                let value =
                    u32::try_from(number(slot, "pointer")?).map_err(|_| "pointer exceeds u32")?;
                if value < base {
                    return Err("pointer lies before base".into());
                }
                value
            }
        };
        resolved.push(value);
    }
    Ok(resolved.into_iter().flat_map(u32::to_le_bytes).collect())
}

#[test]
fn record_and_pointer_tables_preserve_layout_and_reject_bad_references() {
    let records = serde_json::json!({"fields":["a","b"],"element":"le-u16-array","bias":16,"radix":16,"index_field":"id","records":[{"id":0,"a":"11","b":"1234"}]});
    assert_eq!(record_table(&records).unwrap(), [1, 0, 36, 18]);
    for (pointer, value) in [
        ("/records/0/id", serde_json::json!(1)),
        ("/records/0/a", serde_json::json!("f")),
        ("/records/0/a", serde_json::json!("10010")),
        ("/radix", serde_json::json!(1)),
    ] {
        let mut bad = records.clone();
        *bad.pointer_mut(pointer).unwrap() = value;
        assert!(record_table(&bad).is_err());
    }
    let pointers = serde_json::json!({"base_address":4096,"address":8192,"slot_count":5,"slots":["base","self","0x1234","alias:0x002","null"]});
    assert_eq!(
        pointer_table(&pointers).unwrap(),
        [0, 16, 0, 0, 0, 32, 0, 0, 52, 18, 0, 0, 52, 18, 0, 0, 0, 0, 0, 0]
    );
    for invalid in ["alias:4", "0x100000000", "0xfff"] {
        let mut bad = pointers.clone();
        bad["slots"][3] = Value::from(invalid);
        assert!(pointer_table(&bad).is_err());
    }
    let mut bad = pointers;
    bad["slots"][2] = Value::from("null");
    assert!(pointer_table(&bad).is_err());
}

fn resolve_table_symbols(document: &mut Value, symbols: &SourcePaths) -> Result<(), String> {
    for segment in document["segments"]
        .as_array_mut()
        .ok_or("table segments missing")?
    {
        if segment["element"] != "thumb-pointer" {
            continue;
        }
        let values = segment["values"]
            .as_array_mut()
            .ok_or("pointer array missing")?;
        for value in values {
            let address = if value.is_null() {
                0
            } else if value.is_number() || value.as_str().is_some_and(|text| text.starts_with("0x"))
            {
                let address = u32::try_from(number(value, "pointer address")?)
                    .map_err(|_| "pointer exceeds u32")?;
                if address % 2 != 0 {
                    return Err("Thumb function address must be halfword aligned".into());
                }
                address | 1
            } else {
                let name = json_string(value, "pointer symbol")?;
                let address = symbols
                    .main_symbol(name)?
                    .ok_or_else(|| format!("unknown pointer symbol: {name}"))?;
                if address % 2 != 0 {
                    return Err("Thumb function address must be halfword aligned".into());
                }
                address | 1
            };
            *value = Value::from(address);
        }
        segment["element"] = Value::from("le-u32");
    }
    Ok(())
}

#[test]
fn typed_pointer_tables_use_the_owner_register() {
    let symbols = SourcePaths::parse(
        Path::new("."),
        r#"{"format":3,"owners":{"main:08001000":{"name":"Callback_Run"}}}"#,
    )
    .unwrap();
    let source = serde_json::json!({"format":1,"kind":"typed-table","address":0,"size":12,"segments":[
        {"address":0,"end":8,"stride":4,"element":"thumb-pointer","values":["Callback_Run",null]},
        {"address":8,"end":12,"stride":4,"element":"le-s32","values":[-2]}
    ]});
    let mut resolved = source.clone();
    resolve_table_symbols(&mut resolved, &symbols).unwrap();
    assert_eq!(
        typed_table(&resolved).unwrap(),
        [1, 16, 0, 8, 0, 0, 0, 0, 254, 255, 255, 255]
    );
    let mut bad = source;
    bad["segments"][0]["values"][0] = Value::from("Missing");
    assert!(resolve_table_symbols(&mut bad, &symbols).is_err());
    let mut hex = bad;
    hex["segments"][0]["values"][0] = Value::from("0x08001002");
    resolve_table_symbols(&mut hex, &symbols).unwrap();
    assert_eq!(hex["segments"][0]["values"][0], 0x0800_1003);
    resolved["segments"][1]["values"][0] = Value::from(2147483648_i64);
    assert!(typed_table(&resolved).is_err());
}

#[test]
fn typed_pointer_tables_use_their_own_games_register() {
    let directory = tempfile::tempdir().unwrap();
    let root = directory.path();
    for (game, address) in [("tbs", "08001000"), ("tla", "08002000")] {
        let recon = root.join("recon").join(game);
        fs::create_dir_all(&recon).unwrap();
        fs::write(
            recon.join("source-paths.json"),
            format!(r#"{{"format":3,"owners":{{"main:{address}":{{"name":"Callback_Run"}}}}}}"#),
        )
        .unwrap();
    }
    fs::write(
        root.join("TABLE.JSON"),
        r#"{"format":1,"kind":"typed-table","address":0,"size":4,"segments":[
            {"address":0,"end":4,"stride":4,"element":"thumb-pointer","values":["Callback_Run"]}]}"#,
    )
    .unwrap();
    let entry =
        serde_json::json!({"kind":"typed-table","source":"TABLE.JSON","address":0,"size":4});
    for (id, expected) in [
        (crate::targets::DecompTargetId::TbsEn, [1, 16, 0, 8]),
        (crate::targets::DecompTargetId::TlaEn, [1, 32, 0, 8]),
    ] {
        let mut ctx = Context::for_game(root, crate::targets::target_for(id));
        assert_eq!(build_entry(&mut ctx, &entry).unwrap().0, expected, "{id}");
    }
}

/// Derive a stream directory: one `le-u32` word per index giving where that
/// index's stream starts relative to the directory, or zero when it has none.
/// The streams named by `stream_offsets.streams` (a pointer into the table's
/// source document, keyed by decimal index) follow one another from
/// `stream_offsets.first` in index order, each `encoded_size` bytes long.
fn resolve_stream_offsets(table: &mut Value, source: &Value) -> Result<(), String> {
    for segment in table["segments"]
        .as_array_mut()
        .ok_or("table segments missing")?
    {
        let Some(spec) = segment.get("stream_offsets").cloned() else {
            continue;
        };
        if segment.get("values").is_some() || segment["element"] != "le-u32" {
            return Err("stream offsets derive the le-u32 words of a segment".into());
        }
        let streams = source
            .pointer(json_string(&spec["streams"], "stream offsets pointer")?)
            .and_then(Value::as_object)
            .ok_or("stream offsets pointer is absent")?;
        let size = number(&segment["end"], "segment end")?
            .checked_sub(number(&segment["address"], "segment address")?)
            .ok_or("segment ends before it starts")?;
        let count = size / 4;
        if streams.keys().any(|key| {
            key.parse::<usize>()
                .map_or(true, |index| index >= count || index.to_string() != *key)
        }) {
            return Err("stream index lies outside its directory".into());
        }
        let mut next = number(&spec["first"], "first stream offset")?;
        let mut values = Vec::with_capacity(count);
        for index in 0..count {
            match streams.get(&index.to_string()) {
                Some(stream) => {
                    values.push(next);
                    next = next
                        .checked_add(number(&stream["encoded_size"], "stream encoded size")?)
                        .ok_or("stream offsets overflow")?;
                }
                None => values.push(0),
            }
        }
        let object = segment.as_object_mut().ok_or("table segment differs")?;
        object.remove("stream_offsets");
        object.insert("values".into(), serde_json::json!(values));
    }
    Ok(())
}

#[test]
fn stream_directories_derive_offsets_from_encoded_sizes() {
    let source = serde_json::json!({"streams":{"1":{"encoded_size":3},"3":{"encoded_size":5}},
        "table":{"format":1,"kind":"typed-table","address":0,"size":16,"segments":[
            {"address":0,"end":16,"stride":4,"element":"le-u32","stream_offsets":{"streams":"/streams","first":16}}]}});
    let mut table = source["table"].clone();
    resolve_stream_offsets(&mut table, &source).unwrap();
    assert_eq!(
        typed_table(&table).unwrap(),
        [0, 0, 0, 0, 16, 0, 0, 0, 0, 0, 0, 0, 19, 0, 0, 0]
    );
    let mut outside = source.clone();
    outside["streams"]["4"] = serde_json::json!({"encoded_size":1});
    assert!(resolve_stream_offsets(&mut source["table"].clone(), &outside).is_err());
    let mut stored = source["table"].clone();
    stored["segments"][0]["values"] = serde_json::json!([0, 0, 0, 0]);
    assert!(resolve_stream_offsets(&mut stored, &source).is_err());
}

/// Replace `1bpp-rows` record fields with the packed rows of the atlas frame
/// each record names in the segment's `image`: pixel x of a row sits at bit
/// `width - 1 - x`; rows of eight pixels become `u8` values and rows of
/// sixteen pixels little-endian `le-u16` values. Rows below the packed ones
/// must be blank.
fn resolve_table_bitmaps(document: &mut Value, root: &Path) -> Result<Vec<String>, String> {
    let mut sources = Vec::new();
    for segment in document["segments"]
        .as_array_mut()
        .ok_or("table segments missing")?
    {
        let Some(image) = segment.get("image").cloned() else {
            continue;
        };
        let name = json_string(&image["source"], "bitmap source")?;
        let decoded = indexed_png(&fs::read(root_path(root, name)?).map_err(|e| e.to_string())?)
            .map_err(|e| e.to_string())?;
        if decoded.pixels.iter().any(|pixel| *pixel > 1) {
            return Err("bitmap image is not 1bpp".into());
        }
        let pixels: Vec<u8> = decoded.pixels.iter().map(|pixel| *pixel as u8).collect();
        let frame_width = number(&image["frame_width"], "frame width")?;
        let frame_height = number(&image["frame_height"], "frame height")?;
        let frames = component_frames(
            &image,
            decoded.width as usize,
            decoded.height as usize,
            &pixels,
            1,
        )?;
        let mut bitmap_fields = Vec::new();
        for field in segment["fields"]
            .as_array_mut()
            .ok_or("record fields missing")?
        {
            if field["element"] != "1bpp-rows" {
                continue;
            }
            let rows = number(&field["rows"], "bitmap rows")?;
            if rows == 0 || rows > frame_height {
                return Err("bitmap rows exceed the frame".into());
            }
            field["element"] = Value::from(match frame_width {
                8 => "u8",
                16 => "le-u16",
                _ => return Err("bitmap rows must be 8 or 16 pixels wide".into()),
            });
            bitmap_fields.push((json_string(&field["name"], "field name")?.to_string(), rows));
        }
        for record in segment["records"].as_array_mut().ok_or("records missing")? {
            for (name, rows) in &bitmap_fields {
                let frame = frames
                    .get(number(&record[name.as_str()], "bitmap frame")?)
                    .ok_or("bitmap frame lies outside its atlas")?;
                if frame[rows * frame_width..].iter().any(|pixel| *pixel != 0) {
                    return Err("bitmap rows below the record are not blank".into());
                }
                let values: Vec<u32> = (0..*rows)
                    .map(|y| {
                        (0..frame_width).fold(0u32, |row, x| {
                            row | u32::from(frame[y * frame_width + x]) << (frame_width - 1 - x)
                        })
                    })
                    .collect();
                record[name.as_str()] = Value::from(values);
            }
        }
        sources.push(name.to_string());
    }
    Ok(sources)
}

fn typed_table(document: &Value) -> Result<Vec<u8>, String> {
    if document["format"] != 1 || document["kind"] != "typed-table" {
        return Err("typed table identity differs".into());
    }
    let mut address = number(&document["address"], "table address")?;
    let labels = table_labels(document)?;
    let mut output = Vec::new();
    let mut pools: HashMap<String, Vec<usize>> = HashMap::new();
    for segment in document["segments"]
        .as_array()
        .ok_or("table segments missing")?
    {
        let start = number(&segment["address"], "segment address")?;
        let end = number(&segment["end"], "segment end")?;
        if start != address || end <= start {
            return Err("table segments are not contiguous".into());
        }
        let size = end - start;
        let stride = number(&segment["stride"], "segment stride")?;
        let (kind, width) = match segment["element"].as_str() {
            Some("u8") => ("u8-array", 1),
            Some("s8") => ("s8-array", 1),
            Some("le-u16") => ("le-u16-array", 2),
            Some("le-s16") => ("le-s16-array", 2),
            Some("le-u32") => ("le-u32-array", 4),
            Some("le-s32") => ("le-s32-array", 4),
            Some("ascii-fixed") => ("ascii-fixed", 1),
            Some("ascii-pool") => ("ascii-pool", 1),
            Some("pool-pointer") => ("pool-pointer", 4),
            Some("record") => ("record", 1),
            _ => return Err("unknown table element".into()),
        };
        if stride < width || stride % width != 0 || size % stride != 0 {
            return Err("table stride differs".into());
        }
        let bytes = if let Some(fill) = segment.get("fill") {
            if segment.get("values").is_some() || kind != "u8-array" {
                return Err("fill requires an unsigned byte segment without values".into());
            }
            vec![u8::try_from(number(fill, "fill")?).map_err(|_| "fill exceeds u8")?; size]
        } else if let Some(generator) = segment.get("generator") {
            if segment.get("values").is_some() {
                return Err("a generated segment lists no values".into());
            }
            generated_values(generator, kind, width, size / width)?
        } else if kind == "record" {
            let fields = segment["fields"]
                .as_array()
                .ok_or("record fields missing")?;
            let names = fields
                .iter()
                .map(|f| json_string(&f["name"], "field name"))
                .collect::<Result<std::collections::HashSet<_>, _>>()?;
            if names.is_empty() || names.len() != fields.len() {
                return Err("record field names must be nonempty and unique".into());
            }
            let label = segment
                .get("label")
                .map(|label| json_string(label, "record label"))
                .transpose()?;
            let mut seen_labels = std::collections::HashSet::new();
            let mut bytes = Vec::new();
            for record in segment["records"].as_array().ok_or("records missing")? {
                let record = record.as_object().ok_or("record must be an object")?;
                for (key, value) in record {
                    if Some(key.as_str()) == label {
                        let text = json_string(value, "record label")?;
                        if text.is_empty() || !seen_labels.insert(text.to_string()) {
                            return Err("record labels must be nonempty and unique".into());
                        }
                    } else if !names.contains(key.as_str()) {
                        return Err("record fields differ".into());
                    }
                }
                let start = bytes.len();
                for field in fields {
                    let name = json_string(&field["name"], "field name")?;
                    let value = record
                        .get(name)
                        .or_else(|| field.get("default"))
                        .ok_or("record field absent")?;
                    bytes.extend(table_values(value, field, stride, &labels)?);
                }
                if bytes.len() - start != stride {
                    return Err("record stride differs".into());
                }
            }
            bytes
        } else if kind == "ascii-fixed" {
            let text = json_string(&segment["text"], "fixed text")?;
            if stride != 1
                || text.len() >= size
                || !text
                    .bytes()
                    .all(|b| (0x20..=0x7e).contains(&b) || b == b'\n')
            {
                return Err("fixed text differs".into());
            }
            let mut bytes = text.as_bytes().to_vec();
            bytes.resize(size, 0);
            bytes
        } else if kind == "ascii-pool" {
            // Zero-terminated printable strings; every string after the first
            // starts on an `alignment` boundary of its address, and the pool's
            // `name` lets a later `pool-pointer` segment address them by index.
            let alignment = number(&segment["alignment"], "pool alignment")?;
            let texts = segment["texts"].as_array().ok_or("pool texts missing")?;
            if stride != 1 || alignment == 0 || texts.is_empty() {
                return Err("text pool layout differs".into());
            }
            let mut bytes = Vec::new();
            let mut addresses = Vec::new();
            for text in texts {
                let text = json_string(text, "pool text")?;
                if !text.bytes().all(|b| (0x20..=0x7e).contains(&b)) {
                    return Err("pool text is not printable ASCII".into());
                }
                if !addresses.is_empty() {
                    let aligned = (start + bytes.len()).div_ceil(alignment) * alignment - start;
                    bytes.resize(aligned, 0);
                }
                addresses.push(start + bytes.len());
                bytes.extend_from_slice(text.as_bytes());
                bytes.push(0);
            }
            let name = json_string(&segment["name"], "pool name")?;
            if pools.insert(name.to_string(), addresses).is_some() {
                return Err("duplicate pool name".into());
            }
            bytes
        } else if kind == "pool-pointer" {
            let pool = pools
                .get(json_string(&segment["pool"], "pointer pool")?)
                .ok_or("pointer pool is not an earlier text pool")?;
            // An entry is a string's pool index or, for a reader that also
            // accepts a marker word in place of a pointer, a constant named
            // by the segment's or the table's `names`.
            let addresses = segment["values"]
                .as_array()
                .ok_or("pointer values missing")?
                .iter()
                .map(|index| match index.as_str() {
                    Some(name) => segment
                        .get("names")
                        .and_then(|names| names.get(name))
                        .or_else(|| labels.names.get(name))
                        .and_then(Value::as_u64)
                        .and_then(|value| usize::try_from(value).ok())
                        .ok_or_else(|| format!("pool pointer {name} names no constant")),
                    None => pool
                        .get(number(index, "pool index")?)
                        .copied()
                        .ok_or_else(|| "pool index is outside its pool".to_string()),
                })
                .collect::<Result<Vec<_>, String>>()?;
            let mut spec = segment.clone();
            spec["element"] = Value::from("le-u32");
            table_values(&serde_json::json!(addresses), &spec, size, &labels)?
        } else {
            table_values(&segment["values"], segment, size, &labels)?
        };
        if bytes.len() != size {
            return Err("table segment size differs".into());
        }
        if let Some(count) = segment.get("index_count") {
            let count = number(count, "index count")?;
            if kind != "u8-array"
                || count == 0
                || count > 256
                || bytes.iter().any(|&byte| usize::from(byte) >= count)
            {
                return Err("byte index is outside its table".into());
            }
            if let Some(permutation) = segment.get("permutation") {
                let permutation = permutation.as_bool().ok_or("permutation must be boolean")?;
                if permutation {
                    let mut ordered = bytes.clone();
                    ordered.sort_unstable();
                    if ordered.len() != count
                        || ordered
                            .iter()
                            .enumerate()
                            .any(|(i, &b)| i != usize::from(b))
                    {
                        return Err("byte table is not a permutation".into());
                    }
                }
            }
        } else if segment.get("permutation").is_some() {
            return Err("permutation requires index_count".into());
        }
        output.extend(bytes);
        address = end;
    }
    if output.len() != number(&document["size"], "table size")? {
        return Err("table size differs".into());
    }
    Ok(output)
}

/// Values a typed-table segment computes from its formula instead of listing.
/// `ceiling-reciprocal` gives entry `i` the ceiling of 2^`numerator_bits` / `i`
/// reduced to the unsigned element width, the multiplier an unsigned divide by
/// `i` reads: a 32-bit numerator leaves entry 1 as 0, and entry 0 has no
/// reciprocal and holds 0.
fn generated_values(
    generator: &Value,
    kind: &str,
    width: usize,
    count: usize,
) -> Result<Vec<u8>, String> {
    if generator["formula"] != "ceiling-reciprocal" {
        return Err("unknown table generator".into());
    }
    if !matches!(kind, "u8-array" | "le-u16-array" | "le-u32-array") {
        return Err("reciprocals need an unsigned element".into());
    }
    let bits = number(&generator["numerator_bits"], "numerator bits")?;
    if bits == 0 || bits > 64 {
        return Err("reciprocal numerator exceeds 64 bits".into());
    }
    let numerator = 1u128 << bits;
    let modulus = 1u128 << (width * 8);
    let values = (0..count)
        .map(|i| match i {
            0 => Value::from(0),
            i => Value::from((numerator.div_ceil(i as u128) % modulus) as u64),
        })
        .collect();
    integer_array(&Value::Array(values), kind)
}

#[test]
fn generated_reciprocals_follow_their_formula_and_width() {
    let source = serde_json::json!({"format":1,"kind":"typed-table","address":0,"size":24,"segments":[
        {"name":"divide_reciprocals","address":0,"end":24,"element":"le-u32","stride":4,
         "generator":{"formula":"ceiling-reciprocal","numerator_bits":32}}
    ]});
    let words: Vec<u32> = typed_table(&source)
        .unwrap()
        .chunks_exact(4)
        .map(|word| u32::from_le_bytes(word.try_into().unwrap()))
        .collect();
    assert_eq!(
        words,
        [0, 0, 0x8000_0000, 0x5555_5556, 0x4000_0000, 0x3333_3334]
    );
    for (key, value) in [
        ("formula", serde_json::json!("floor-reciprocal")),
        ("numerator_bits", serde_json::json!(65)),
        ("numerator_bits", serde_json::json!(0)),
    ] {
        let mut changed = source.clone();
        changed["segments"][0]["generator"][key] = value;
        assert!(typed_table(&changed).is_err(), "{key}");
    }
    for (key, value) in [
        ("element", serde_json::json!("le-s32")),
        ("values", serde_json::json!([0, 0, 0, 0, 0, 0])),
    ] {
        let mut changed = source.clone();
        changed["segments"][0][key] = value;
        assert!(typed_table(&changed).is_err(), "{key}");
    }
}

/// Symbolic integer values of one typed table. A string value is a named
/// constant from the field's or the table's `names`, a `0x` hexadecimal
/// literal, a segment `name` (its start address) or `name[index]` (the
/// address of element `index` of that segment). A name declared by more
/// than one segment is ambiguous and cannot be referenced.
struct TableLabels {
    segments: std::collections::BTreeMap<String, Option<(usize, usize)>>,
    names: Value,
}

fn table_labels(document: &Value) -> Result<TableLabels, String> {
    let mut segments = std::collections::BTreeMap::new();
    for segment in document["segments"]
        .as_array()
        .ok_or("table segments missing")?
    {
        if let Some(name) = segment.get("name") {
            let name = json_string(name, "segment name")?;
            let start = number(&segment["address"], "segment address")?;
            let stride = number(&segment["stride"], "segment stride")?;
            segments
                .entry(name.to_string())
                .and_modify(|label| *label = None)
                .or_insert(Some((start, stride)));
        }
    }
    Ok(TableLabels {
        segments,
        names: document.get("names").cloned().unwrap_or(Value::Null),
    })
}

fn symbolic_value(text: &str, spec: &Value, labels: &TableLabels) -> Result<i64, String> {
    if let Some(named) = spec
        .get("names")
        .and_then(|names| names.get(text))
        .or_else(|| labels.names.get(text))
    {
        return named
            .as_i64()
            .ok_or_else(|| format!("named value {text} is not an integer"));
    }
    if text.starts_with("0x") || text.bytes().all(|byte| byte.is_ascii_digit()) {
        return i64::try_from(number(&Value::from(text), "numeric value")?)
            .map_err(|_| format!("numeric value {text} overflows"));
    }
    let (name, index) = match text.strip_suffix(']').and_then(|text| text.split_once('[')) {
        Some((name, index)) => (name, number(&Value::from(index), "element index")?),
        None => (text, 0),
    };
    match labels.segments.get(name) {
        Some(Some((start, stride))) => index
            .checked_mul(*stride)
            .and_then(|offset| offset.checked_add(*start))
            .and_then(|address| i64::try_from(address).ok())
            .ok_or_else(|| format!("element index overflows in {text}")),
        Some(None) => Err(format!("segment name {name} is ambiguous")),
        None => Err(format!("unknown table value {text}")),
    }
}

fn table_values(
    value: &Value,
    spec: &Value,
    max_bytes: usize,
    labels: &TableLabels,
) -> Result<Vec<u8>, String> {
    let element = json_string(&spec["element"], "field element")?;
    let (kind, width) = match element {
        "u8" => ("u8-array", 1),
        "s8" => ("s8-array", 1),
        "le-u16" => ("le-u16-array", 2),
        "le-s16" => ("le-s16-array", 2),
        "le-u32" => ("le-u32-array", 4),
        "le-s32" => ("le-s32-array", 4),
        _ => return Err("unknown field element".into()),
    };
    fn flatten(
        value: &Value,
        spec: &Value,
        labels: &TableLabels,
        out: &mut Vec<i64>,
    ) -> Result<(), String> {
        if let Some(items) = value.as_array() {
            for item in items {
                flatten(item, spec, labels, out)?;
            }
        } else if let Some(text) = value.as_str() {
            out.push(symbolic_value(text, spec, labels)?);
        } else {
            out.push(array_member(value).map_err(|_| "field value is not an integer")?);
        }
        Ok(())
    }
    let mut values = Vec::new();
    if let Some(bits) = spec.get("bits") {
        let widths = bits
            .as_object()
            .ok_or("bit widths must map part names to widths")?
            .values()
            .map(|w| number(w, "bit width"))
            .collect::<Result<Vec<_>, _>>()?;
        if widths.iter().sum::<usize>() != width * 8 || widths.iter().any(|&w| w == 0) {
            return Err("bit widths do not fill the element".into());
        }
        let groups = match value.as_array() {
            Some(items) if items.iter().all(Value::is_array) => items.iter().collect(),
            _ => vec![value],
        };
        for group in groups {
            let parts = group
                .as_array()
                .filter(|parts| parts.len() == widths.len())
                .ok_or("packed value has the wrong number of parts")?;
            let mut packed = 0i64;
            let mut shift = 0;
            for (part, &bits) in parts.iter().zip(&widths) {
                let part = part.as_i64().ok_or("packed part is not an integer")?;
                if part < 0 || part >= 1i64 << bits {
                    return Err("packed part exceeds its bit width".into());
                }
                packed |= part << shift;
                shift += bits;
            }
            values.push(packed);
        }
    } else {
        flatten(value, spec, labels, &mut values)?;
    }
    for key in ["min", "max"] {
        if let Some(limit) = spec.get(key) {
            let limit = limit.as_i64().ok_or("field bound is not an integer")?;
            if values
                .iter()
                .any(|&v| if key == "min" { v < limit } else { v > limit })
            {
                return Err("field value outside bounds".into());
            }
        }
    }
    if let Some(unique) = spec.get("unique") {
        if unique.as_bool().ok_or("unique must be boolean")? {
            let mut sorted = values.clone();
            sorted.sort_unstable();
            if sorted.windows(2).any(|v| v[0] == v[1]) {
                return Err("duplicate field value".into());
            }
        }
    }
    let mut bytes = integer_array(&serde_json::json!(values), kind)?;
    if let Some(capacity) = spec.get("capacity") {
        let capacity = number(capacity, "capacity")?;
        if spec.get("terminated_capacity").is_some()
            || capacity == 0
            || capacity > max_bytes / width
            || values.len() > capacity
        {
            return Err("field exceeds its capacity".into());
        }
        bytes.resize(capacity * width, 0);
    }
    if let Some(capacity) = spec.get("terminated_capacity") {
        let capacity = number(capacity, "terminated capacity")?;
        if capacity == 0
            || capacity > max_bytes / width
            || values.len() >= capacity
            || values.contains(&0)
        {
            return Err("terminated field has no room for terminator or contains zero".into());
        }
        bytes.resize(capacity.checked_mul(width).ok_or("field size overflow")?, 0);
    } else if let Some(capacity) = spec.get("capacity") {
        let capacity = number(capacity, "capacity")?;
        if capacity == 0 || capacity > max_bytes / width || values.len() > capacity {
            return Err("field exceeds its zero-padded capacity".into());
        }
        bytes.resize(capacity.checked_mul(width).ok_or("field size overflow")?, 0);
    }
    Ok(bytes)
}

#[test]
fn typed_byte_tables_check_index_ranges_and_permutations() {
    let source = serde_json::json!({"format":1,"kind":"typed-table","address":0,"size":4,"segments":[
        {"address":0,"end":4,"element":"u8","stride":1,"values":[2,0,3,1],"index_count":4,"permutation":true}
    ]});
    assert_eq!(typed_table(&source).unwrap(), [2, 0, 3, 1]);
    for (pointer, value) in [
        ("/segments/0/values/0", serde_json::json!(1)),
        ("/segments/0/values/0", serde_json::json!(4)),
        ("/segments/0/index_count", serde_json::json!(5)),
        ("/segments/0/permutation", serde_json::json!("true")),
    ] {
        let mut invalid = source.clone();
        *invalid.pointer_mut(pointer).unwrap() = value;
        assert!(typed_table(&invalid).is_err(), "{pointer}");
    }
    let mut mapping = source;
    mapping["segments"][0]["permutation"] = serde_json::json!(false);
    mapping["segments"][0]["values"] = serde_json::json!([1, 1, 1, 1]);
    assert_eq!(typed_table(&mapping).unwrap(), [1, 1, 1, 1]);
}

#[test]
fn typed_records_preserve_names_termination_bounds_and_signedness() {
    let source = serde_json::json!({"format":1,"kind":"typed-table","address":0,"size":10,"segments":[
        {"address":0,"end":8,"stride":8,"element":"record","fields":[
            {"name":"ids","element":"le-u16","terminated_capacity":3,"min":1,"max":100,"unique":true},
            {"name":"offset","element":"le-s16"}
        ],"records":[{"ids":[2,3],"offset":-2}]},
        {"address":8,"end":10,"stride":1,"element":"u8","fill":255}
    ]});
    assert_eq!(
        typed_table(&source).unwrap(),
        [2, 0, 3, 0, 0, 0, 254, 255, 255, 255]
    );
    let mut hex = source.clone();
    hex["segments"][0]["records"][0]["ids"] = serde_json::json!(["0x2", "3"]);
    assert_eq!(typed_table(&hex).unwrap(), typed_table(&source).unwrap());
    hex["segments"][0]["records"][0]["ids"] = serde_json::json!(["-2"]);
    assert!(typed_table(&hex).is_err());
    for (pointer, value) in [
        ("/segments/0/records/0/ids", serde_json::json!([2, 2])),
        ("/segments/0/records/0/ids", serde_json::json!([0])),
        ("/segments/0/records/0/ids", serde_json::json!([101])),
        ("/segments/0/records/0/ids", serde_json::json!([1, 2, 3])),
        ("/segments/0/records/0/offset", serde_json::json!(32768)),
        (
            "/segments/0/fields/0/terminated_capacity",
            serde_json::json!(1000000000),
        ),
        ("/segments/0/fields/1/name", serde_json::json!("ids")),
        ("/segments/1/fill", serde_json::json!(256)),
        ("/segments/1/address", serde_json::json!(7)),
    ] {
        let mut bad = source.clone();
        *bad.pointer_mut(pointer).unwrap() = value;
        assert!(typed_table(&bad).is_err(), "{pointer}");
    }
}

#[test]
fn typed_tables_resolve_symbolic_values_and_pad_capacities() {
    let source = serde_json::json!({"format":1,"kind":"typed-table","address":4096,"size":28,"names":{"stop":239},"segments":[
        {"name":"script","address":4096,"end":4102,"stride":2,"element":"u8","values":[[1,2],[3,4],["stop",0]]},
        {"name":"directory","address":4102,"end":4110,"stride":4,"element":"le-u32","values":["script[2]","directory"]},
        {"name":"record","address":4110,"end":4116,"stride":6,"element":"record","fields":[
            {"name":"kind","element":"u8","names":{"wide":7}},
            {"name":"slots","element":"u8","capacity":3},
            {"name":"target","element":"le-u16"}
        ],"records":[{"kind":"wide","slots":[9],"target":"0x1002"}]},
        {"address":4116,"end":4124,"stride":1,"element":"ascii-fixed","text":"ab\n"}
    ]});
    assert_eq!(
        typed_table(&source).unwrap(),
        [
            1, 2, 3, 4, 239, 0, 4, 16, 0, 0, 6, 16, 0, 0, 7, 9, 0, 0, 2, 16, 97, 98, 10, 0, 0, 0,
            0, 0
        ]
    );
    for (pointer, value) in [
        ("/segments/1/values/0", serde_json::json!("script[x]")),
        ("/segments/1/values/0", serde_json::json!("missing")),
        (
            "/segments/2/records/0/slots",
            serde_json::json!([1, 2, 3, 4]),
        ),
        ("/segments/2/fields/1/capacity", serde_json::json!(0)),
        ("/segments/0/values/2", serde_json::json!(["halt", 0])),
        ("/segments/0/name", serde_json::json!("directory")),
    ] {
        let mut bad = source.clone();
        *bad.pointer_mut(pointer).unwrap() = value;
        assert!(typed_table(&bad).is_err(), "{pointer}");
    }
}

#[test]
fn typed_records_apply_defaults_capacities_and_labels() {
    let source = serde_json::json!({"format":1,"kind":"typed-table","address":0,"size":12,"segments":[
        {"address":0,"end":12,"stride":6,"element":"record","label":"name","fields":[
            {"name":"id","element":"le-u16"},
            {"name":"slots","element":"u8","capacity":3,"default":[]},
            {"name":"kind","element":"u8","default":4,"max":4}
        ],"records":[{"name":"first","id":1},{"name":"second","id":2,"slots":[7,8],"kind":0}]}
    ]});
    assert_eq!(
        typed_table(&source).unwrap(),
        [1, 0, 0, 0, 0, 4, 2, 0, 7, 8, 0, 0]
    );
    for (pointer, value) in [
        ("/segments/0/records/0/name", serde_json::json!("")),
        ("/segments/0/records/0/name", serde_json::json!("second")),
        ("/segments/0/records/0/id", serde_json::json!(null)),
        (
            "/segments/0/records/1/slots",
            serde_json::json!([1, 2, 3, 4]),
        ),
        ("/segments/0/records/1/kind", serde_json::json!(5)),
        ("/segments/0/fields/1/capacity", serde_json::json!(0)),
    ] {
        let mut bad = source.clone();
        *bad.pointer_mut(pointer).unwrap() = value;
        assert!(typed_table(&bad).is_err(), "{pointer}");
    }
    let mut both = source.clone();
    both["segments"][0]["fields"][1]["terminated_capacity"] = serde_json::json!(3);
    assert!(typed_table(&both).is_err());
    let mut unknown = source.clone();
    unknown["segments"][0]["records"][0]["extra"] = serde_json::json!(1);
    assert!(typed_table(&unknown).is_err());
    let mut missing = source;
    missing["segments"][0]["records"][0]
        .as_object_mut()
        .unwrap()
        .remove("id");
    assert!(typed_table(&missing).is_err());
}

#[test]
fn tiled_atlas_serializes_frames_before_tile_rows() {
    let root = tempfile::tempdir().unwrap();
    let mut image = Vec::new();
    {
        let mut encoder = png::Encoder::new(&mut image, 16, 16);
        encoder.set_color(png::ColorType::Indexed);
        encoder.set_depth(png::BitDepth::Eight);
        encoder.set_palette((0..16).flat_map(|i| [i * 8; 3]).collect::<Vec<u8>>());
        let pixels = (0..256)
            .map(|i| if i % 16 < 8 { 1 } else { 2 })
            .collect::<Vec<u8>>();
        encoder
            .write_header()
            .unwrap()
            .write_image_data(&pixels)
            .unwrap();
    }
    fs::write(root.path().join("atlas.png"), image).unwrap();
    let entry = serde_json::json!({"kind":"gba-4bpp-tiles","source":"atlas.png","size":128,"frames":2,"columns":2,"frame_tiles_wide":1,"frame_tiles_high":2,"symbolic_palette":true});
    let result = build_component(root.path(), &entry).unwrap();
    assert_eq!(result.data, [vec![0x11; 64], vec![0x22; 64]].concat());
    let mut paired = entry.clone();
    paired["frame"] = 0.into();
    paired["frames_per_image"] = 2.into();
    assert_eq!(
        build_component(root.path(), &paired).unwrap().data,
        result.data
    );
    paired["frame"] = 1.into();
    assert!(build_component(root.path(), &paired).is_err());
    paired["frame"] = 0.into();
    for parts in [0, 3] {
        paired["frames_per_image"] = parts.into();
        assert!(build_component(root.path(), &paired).is_err());
    }
    assert_eq!(
        result.details,
        serde_json::json!({
            "width": 16, "height": 16, "bpp": 4, "tiles": 4, "palette_entries": 16,
        })
    );
    for field in ["frames", "columns", "frame_tiles_wide", "frame_tiles_high"] {
        let mut bad = entry.clone();
        bad[field] = serde_json::json!(0);
        assert!(build_component(root.path(), &bad).is_err());
    }
}

#[test]
fn typed_tables_check_layout_width_and_text() {
    let source = serde_json::json!({"format":1,"kind":"typed-table","address":16,"size":12,"segments":[
        {"address":16,"end":18,"element":"s8","stride":1,"values":[-128,127]},
        {"address":18,"end":20,"element":"le-u16","stride":2,"values":[4660]},
        {"address":20,"end":24,"element":"le-u32","stride":4,"values":[305419896]},
        {"address":24,"end":28,"element":"ascii-fixed","stride":1,"text":"AB"}
    ]});
    assert_eq!(
        typed_table(&source).unwrap(),
        [128, 127, 52, 18, 120, 86, 52, 18, 65, 66, 0, 0]
    );
    for (pointer, value) in [
        ("/segments/0/address", serde_json::json!(15)),
        ("/segments/1/stride", serde_json::json!(0)),
        ("/segments/1/values/0", serde_json::json!(65536)),
        ("/segments/2/values/0", serde_json::json!(-1)),
        ("/segments/2/values/0", serde_json::json!(4294967296u64)),
        ("/segments/3/text", serde_json::json!("ABCD")),
        ("/segments/3/text", serde_json::json!("é")),
        ("/size", serde_json::json!(13)),
    ] {
        let mut invalid = source.clone();
        *invalid.pointer_mut(pointer).unwrap() = value;
        assert!(typed_table(&invalid).is_err(), "{pointer}");
    }
}

#[test]
fn typed_text_pools_resolve_aligned_string_pointers() {
    let source = serde_json::json!({"format":1,"kind":"typed-table","address":256,"size":26,"segments":[
        {"address":256,"end":270,"stride":1,"element":"ascii-pool","name":"names","alignment":4,"texts":["AB","","CDEFG"]},
        {"address":270,"end":282,"stride":4,"element":"pool-pointer","pool":"names","values":[2,0],"terminated_capacity":3}
    ]});
    assert_eq!(
        typed_table(&source).unwrap(),
        [65, 66, 0, 0, 0, 0, 0, 0, 67, 68, 69, 70, 71, 0, 8, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0]
    );
    for (pointer, value) in [
        ("/segments/0/alignment", serde_json::json!(0)),
        ("/segments/0/texts/2", serde_json::json!("é")),
        ("/segments/0/texts/2", serde_json::json!("CDEF")),
        ("/segments/1/pool", serde_json::json!("other")),
        ("/segments/1/values/0", serde_json::json!(3)),
        ("/segments/1/values", serde_json::json!([0, 1, 2])),
    ] {
        let mut invalid = source.clone();
        *invalid.pointer_mut(pointer).unwrap() = value;
        assert!(typed_table(&invalid).is_err(), "{pointer}");
    }
    let mut marked = source.clone();
    marked["segments"][1]["names"] = serde_json::json!({"section_break": 255});
    marked["segments"][1]["values"] = serde_json::json!([2, "section_break"]);
    assert_eq!(typed_table(&marked).unwrap()[18..22], [255, 0, 0, 0]);
    marked["segments"][1]["values"] = serde_json::json!([2, "page_break"]);
    assert!(typed_table(&marked).is_err());
    let mut reversed = source.clone();
    reversed["segments"].as_array_mut().unwrap().reverse();
    reversed["segments"][0]["address"] = Value::from(256);
    reversed["segments"][0]["end"] = Value::from(268);
    reversed["segments"][1]["address"] = Value::from(268);
    reversed["segments"][1]["end"] = Value::from(282);
    assert!(typed_table(&reversed).is_err());
}

#[test]
fn typed_fields_pack_named_bit_widths_lsb_first() {
    let source = serde_json::json!({"format":1,"kind":"typed-table","address":0,"size":6,"segments":[
        {"address":0,"end":4,"element":"le-u16","stride":2,"bits":{"id":9,"class":7},"values":[[1,0],[15,1]]},
        {"address":4,"end":6,"element":"le-u16","stride":2,"bits":{"low":12,"high":4},"values":[4095,0]}
    ]});
    assert_eq!(typed_table(&source).unwrap(), [1, 0, 15, 2, 255, 15]);
    for (pointer, value) in [
        ("/segments/0/values/0", serde_json::json!([512, 0])),
        ("/segments/0/values/0", serde_json::json!([1])),
        ("/segments/0/values/0", serde_json::json!([-1, 0])),
        ("/segments/0/bits", serde_json::json!({"id":9,"class":6})),
        ("/segments/0/bits", serde_json::json!([9, 7])),
    ] {
        let mut bad = source.clone();
        *bad.pointer_mut(pointer).unwrap() = value;
        assert!(typed_table(&bad).is_err(), "{pointer}");
    }
}

#[test]
fn tile_components_truncate_zero_canvas_tails() {
    let root = tempfile::tempdir().unwrap();
    let mut image = Vec::new();
    {
        let mut encoder = png::Encoder::new(&mut image, 8, 16);
        encoder.set_color(png::ColorType::Indexed);
        encoder.set_depth(png::BitDepth::Eight);
        encoder.set_palette((0..16).flat_map(|i| [i * 8; 3]).collect::<Vec<u8>>());
        let pixels = (0..128).map(|i| u8::from(i < 72)).collect::<Vec<u8>>();
        encoder
            .write_header()
            .unwrap()
            .write_image_data(&pixels)
            .unwrap();
    }
    fs::write(root.path().join("canvas.png"), image).unwrap();
    let entry = serde_json::json!({"kind":"gba-4bpp-tiles","source":"canvas.png","size":36,"canvas_size":64});
    assert_eq!(
        build_component(root.path(), &entry).unwrap().data,
        vec![0x11; 36]
    );
    for (key, value) in [("size", 35), ("canvas_size", 63)] {
        let mut bad = entry.clone();
        bad[key] = serde_json::json!(value);
        assert!(build_component(root.path(), &bad).is_err(), "{key}");
    }
}

fn parse_general_tokens(value: &Value) -> Result<Vec<psynergy::assets::lz::GeneralToken>, String> {
    value
        .as_array()
        .ok_or("general-LZ tokens are not an array".to_string())?
        .iter()
        .map(|item| {
            let values = item
                .as_array()
                .ok_or("general-LZ token is not an array".to_string())?;
            let tag = values
                .first()
                .and_then(Value::as_str)
                .ok_or("general-LZ token has no tag".to_string())?;
            match tag {
                "l" if values.len() == 2 => Ok(psynergy::assets::lz::GeneralToken::Literal(
                    number(&values[1], "literal")? as u32,
                )),
                "c" if values.len() == 3 => Ok(psynergy::assets::lz::GeneralToken::Copy {
                    length: number(&values[1], "copy length")? as u32,
                    distance: number(&values[2], "copy distance")? as u32,
                }),
                _ => Err("unsupported general-LZ token".to_string()),
            }
        })
        .collect()
}
fn parse_halfword_tokens(
    value: &Value,
) -> Result<Vec<psynergy::assets::lz::HalfwordToken>, String> {
    value
        .as_array()
        .ok_or("halfword-LZ tokens are not an array".to_string())?
        .iter()
        .map(|item| {
            let values = item
                .as_array()
                .ok_or("halfword-LZ token is not an array".to_string())?;
            match (values.first().and_then(Value::as_str), values.len()) {
                (Some("l"), 2) => Ok(psynergy::assets::lz::HalfwordToken::Literal(number(
                    &values[1], "literal",
                )?
                    as u32)),
                (Some("c"), 3) => Ok(psynergy::assets::lz::HalfwordToken::Copy {
                    length: number(&values[1], "copy length")? as u32,
                    distance: number(&values[2], "copy distance")? as u32,
                }),
                (Some("e"), 1) => Ok(psynergy::assets::lz::HalfwordToken::End),
                _ => Err("unsupported halfword-LZ token".to_string()),
            }
        })
        .collect()
}
/// The plan an entry selects inside its plan document: `plan_section` names
/// a top-level key or, starting with `/`, a JSON pointer.
fn select_plan<'a>(document: &'a Value, entry: &Value) -> Result<&'a Value, String> {
    match entry.get("plan_section").and_then(Value::as_str) {
        None => Ok(document),
        Some(pointer) if pointer.starts_with('/') => document
            .pointer(pointer)
            .ok_or_else(|| format!("plan section {pointer} is absent")),
        Some(section) => Ok(document.get(section).unwrap_or(document)),
    }
}
/// Encode one stream from its plan. `arena` holds the bytes that precede the
/// stream in its container for codecs whose copies read from them; `machine`
/// holds the target machine's LZSS compressors, which general and palette LZ
/// require.
/// One compressed stream, answered from the stream cache when the same
/// build implementation already encoded the same input under the same plan,
/// arena and machine: the encoders are pure functions of those, so an edited
/// overlay or asset re-encodes only its own streams. Failures are never
/// cached, and every result still meets the ROM comparison.
fn encode_lz_stream(
    decoded: &[u8],
    plan: &Value,
    arena: &[u8],
    machine: Option<&LzMachine>,
) -> Result<Vec<u8>, String> {
    static CACHE: std::sync::OnceLock<Option<psynergy::cache::SqliteCache>> =
        std::sync::OnceLock::new();
    let cache = CACHE.get_or_init(|| {
        psynergy::cache::SqliteCache::open(
            &crate::compiler::routing::root().join("out/cache/asset-streams.sqlite3"),
        )
        .ok()
    });
    let mut identity = Vec::new();
    for part in [
        b"lz-stream-v1".as_slice(),
        executable_signature()?.as_bytes(),
        plan.to_string().as_bytes(),
        format!("{machine:?}").as_bytes(),
        arena,
        decoded,
    ] {
        identity.extend_from_slice(&(part.len() as u64).to_le_bytes());
        identity.extend_from_slice(part);
    }
    let key = sha256::hex(&identity);
    if let Some(hit) = cache
        .as_ref()
        .and_then(|cache| cache.get(&key).ok().flatten())
        .and_then(|entries| entries.into_iter().find(|(kind, _)| kind == "stream"))
    {
        return Ok(hit.1);
    }
    let built = encode_lz_stream_uncached(decoded, plan, arena, machine)?;
    if let Some(cache) = cache {
        let _ = cache.put(&key, &[("stream", &built)]);
    }
    Ok(built)
}
fn encode_lz_stream_uncached(
    decoded: &[u8],
    plan: &Value,
    arena: &[u8],
    machine: Option<&LzMachine>,
) -> Result<Vec<u8>, String> {
    let codec = json_string(&plan["codec"], "codec")?;
    if codec == "golden-sun-overlay-lz" {
        return encode_overlay_stream(
            decoded,
            machine.ok_or("overlay compression needs the target's reference machine definition")?,
        );
    }
    if decoded.len() != number(&plan["decoded_size"], "decoded_size")? {
        return Err("decoded components do not match plan size".to_string());
    }
    if codec == "golden-sun-tagged-lz" {
        // A tagged stream's leading byte names its codec: the smaller of the
        // general and palette encodings, palette on ties, as for overlays.
        if plan.get("tokens").is_some() || plan.get("tag").is_some() {
            return Err("tagged streams derive their codec; stored choices are refused".into());
        }
        let built = encode_overlay_stream(
            decoded,
            machine.ok_or("tagged compression needs the target's reference machine definition")?,
        )?;
        check_stored_extent(plan, built.len())?;
        return Ok(built);
    }
    let arena_codec = codec == "golden-sun-arena-lz";
    if arena_codec && (plan.get("tokens").is_some() || plan.get("final_flags").is_some()) {
        return Err(
            "arena streams are compressed from their input; stored controls are refused".into(),
        );
    }
    let mut expanded = plan.clone();
    if !arena_codec {
        expanded["tokens"] = compression_plan::materialize(decoded, plan, machine)?;
    }
    let plan = &expanded;
    let mut built = match codec {
        "golden-sun-general-lz-prefill" => psynergy::assets::lz::encode_general_prefill(
            decoded,
            &parse_general_tokens(plan.get("tokens").ok_or("general-LZ tokens are missing")?)?,
            number(&plan["prefill"], "prefill")?,
            number(plan.get("header").unwrap_or(&Value::from(1)), "header")?,
        )
        .map_err(|e| e.to_string())?,
        "golden-sun-general-lz" => psynergy::assets::lz::encode_general(
            decoded,
            &parse_general_tokens(plan.get("tokens").ok_or("general-LZ tokens are missing")?)?,
        )
        .map_err(|e| e.to_string())?,
        "golden-sun-halfword-lz" => psynergy::assets::lz::encode_halfword(
            &decoded,
            &parse_halfword_tokens(plan.get("tokens").ok_or("halfword-LZ tokens are missing")?)?,
        )
        .map_err(|e| e.to_string())?,
        "golden-sun-palette-lz" | "golden-sun-tagged-palette-lz" => {
            let groups = plan
                .get("tokens")
                .and_then(Value::as_array)
                .ok_or("palette-LZ tokens are not an array".to_string())?
                .iter()
                .map(parse_group)
                .collect::<Result<Vec<_>, _>>()?;
            psynergy::assets::lz::encode_palette(decoded, &groups).map_err(|e| e.to_string())?
        }
        "golden-sun-arena-lz" => {
            psynergy::assets::lz::compress_arena(decoded, arena).map_err(|e| e.to_string())?
        }
        _ => return Err("unsupported custom-LZ plan".to_string()),
    };
    if codec == "golden-sun-tagged-palette-lz" {
        if number(&plan["tag"], "tag")? != 1 {
            return Err("tagged palette-LZ plan is missing tag 1".to_string());
        }
        built.insert(0, 1);
    }
    check_stored_extent(plan, built.len())?;
    Ok(built)
}
/// A plan's `encoded_size` is its stream's stored extent: the stream and the
/// up to three alignment bytes that the file's writer, never the encoder,
/// leaves after it (see `packer`). Recorded alignment bytes are refused.
fn check_stored_extent(plan: &Value, stream: usize) -> Result<(), String> {
    if plan.get("lookahead").is_some() {
        return Err(
            "stored lookahead is refused; the packer replay supplies stream alignment".into(),
        );
    }
    if let Some(extent) = plan.get("encoded_size") {
        let extent = number(extent, "encoded_size")?;
        if stream > extent || extent - stream > 3 {
            return Err(format!(
                "encoded stream is 0x{stream:x} bytes, plan expects a stored extent of 0x{extent:x}"
            ));
        }
    }
    Ok(())
}
#[test]
fn stored_extents_leave_alignment_to_the_writer_and_refuse_recorded_bytes() {
    assert!(check_stored_extent(&serde_json::json!({"encoded_size": 8}), 5).is_ok());
    assert!(check_stored_extent(&serde_json::json!({"encoded_size": 8}), 8).is_ok());
    assert!(check_stored_extent(&serde_json::json!({"encoded_size": 8}), 4).is_err());
    assert!(check_stored_extent(&serde_json::json!({"encoded_size": 8}), 9).is_err());
    for recorded in ["", "00", "4e"] {
        let plan = serde_json::json!({"encoded_size": 8, "lookahead": recorded});
        assert!(check_stored_extent(&plan, 7)
            .unwrap_err()
            .contains("stored lookahead is refused"));
    }
}

/// Overlay streams select the smaller of the two encodings, palette on ties.
/// Legacy sidecars are explicit exceptions; this path consumes no saved choices.
pub(crate) fn encode_overlay_stream(
    decoded: &[u8],
    machine: &LzMachine,
) -> Result<Vec<u8>, String> {
    let general = encode_lz_stream(
        decoded,
        &serde_json::json!({
            "codec":"golden-sun-general-lz", "decoded_size":decoded.len()
        }),
        &[],
        Some(machine),
    )?;
    let palette = encode_lz_stream(
        decoded,
        &serde_json::json!({
            "codec":"golden-sun-tagged-palette-lz", "decoded_size":decoded.len(), "tag":1
        }),
        &[],
        Some(machine),
    )?;
    Ok(if palette.len() <= general.len() {
        palette
    } else {
        general
    })
}
/// Every ROM range `manifest` declares, expanded as the asset build expands
/// it: its regions, closure packages and series, as `(start, end)` addresses.
/// Nothing is built or read from a ROM.
pub(crate) fn declared_ranges(root: &Path, manifest: &str) -> Result<Vec<(usize, usize)>, String> {
    let mut ctx = Context::new(root);
    let manifest = json(&ctx.source(manifest)?)?;
    let mut entries = manifest
        .get("regions")
        .and_then(Value::as_array)
        .cloned()
        .unwrap_or_default();
    expand_closure_packages(&mut ctx, &manifest, &mut entries)?;
    expand_series(&mut ctx, &manifest, &mut entries)?;
    entries
        .iter()
        .map(|entry| {
            let address = number(&entry["address"], "asset address")?;
            let size = number(&entry["size"], "asset size")?;
            Ok((address, address + size))
        })
        .collect()
}
/// The LZSS compressors of the reference machine that `target`'s asset
/// manifest names.
pub(crate) fn target_lz_machine(
    root: &Path,
    target: &crate::targets::DecompTarget,
) -> Result<LzMachine, String> {
    let manifest = json(&root.join(target.asset_manifest))?;
    let name = json_string(&manifest["machine"], "reference machine definition")?;
    LzMachine::of(&json(&root_path(root, name)?)?).map_err(|error| format!("{name}: {error}"))
}
/// Build an LZ entry: its components are concatenated and encoded with the
/// selected plan. A plan array describes a sequence of streams: stream `i`
/// encodes the components with atlas frame `i` selected, is padded to
/// `stream_alignment`, and may read the streams before it as its arena.
#[cfg(test)]
fn build_general_lz(root: &Path, entry: &Value) -> Result<(Vec<u8>, Vec<String>, Value), String> {
    build_general_lz_cached(&Context::new(root), entry)
}
fn build_general_lz_cached(
    ctx: &Context,
    entry: &Value,
) -> Result<(Vec<u8>, Vec<String>, Value), String> {
    let root = &ctx.root;
    let plan_name = entry
        .get("plan")
        .map(|value| json_string(value, "general-LZ plan"))
        .transpose()?;
    let plan_document = match plan_name {
        Some(name) => ctx.document(&root_path(root, name)?)?,
        None if entry["components"].as_array().is_some_and(|components| {
            components.len() == 1 && components[0]["kind"] == "golden-sun-thumb-overlay"
        }) =>
        {
            std::rc::Rc::new(serde_json::json!({"codec":"golden-sun-overlay-lz"}))
        }
        None => return Err("only overlay streams may omit their compression plan".into()),
    };
    let plan = select_plan(&plan_document, entry)?;
    let component_document = entry
        .get("components_source")
        .map(|source| {
            let source = json_string(source, "general-LZ components source")?;
            ctx.document(&root_path(root, source)?)
        })
        .transpose()?;
    let referenced_components = if let Some(document) = component_document.as_ref() {
        let pointer = json_string(
            &entry["components_pointer"],
            "general-LZ components pointer",
        )?;
        Some(
            document
                .pointer(pointer)
                .ok_or("general-LZ components pointer is absent")?,
        )
    } else {
        None
    };
    let components = entry
        .get("components")
        .or(referenced_components)
        .or_else(|| plan_document.get("components"))
        .and_then(Value::as_array)
        .ok_or("general-LZ components are not an array".to_string())?;
    let sequence = plan.is_array();
    let plans: Vec<&Value> = match plan.as_array() {
        Some(streams) => streams.iter().collect(),
        None => vec![plan],
    };
    let alignment = entry
        .get("stream_alignment")
        .map(|value| number(value, "stream alignment"))
        .transpose()?
        .unwrap_or(1)
        .max(1);
    let mut built = Vec::new();
    let mut sources = Vec::new();
    let mut reports = Vec::new();
    let mut decoded_total = 0;
    let mut tokens = 0;
    for (index, plan) in plans.iter().enumerate() {
        let mut decoded = Vec::new();
        for component in components {
            let mut component = component.clone();
            if sequence {
                component["frame"] = Value::from(index);
            }
            let result = build_component_cached(ctx, &component)?;
            if index == 0 {
                sources.push(json_string(&component["source"], "component source")?.to_string());
                sources.extend(result.sources.iter().skip(1).cloned());
                reports.push(serde_json::json!({"kind": component.get("kind"), "source": component.get("source"), "details": result.details}));
            }
            decoded.extend(result.data);
        }
        decoded_total += decoded.len();
        tokens += plan
            .get("tokens")
            .and_then(Value::as_array)
            .map_or(0, Vec::len);
        let mut stream = encode_lz_stream(&decoded, plan, &built, ctx.lz_machine.as_ref())?;
        stream.resize(stream.len().div_ceil(alignment) * alignment, 0);
        built.extend(stream);
    }
    if let Some(name) = plan_name {
        sources.push(name.to_string());
    }
    if let Some(source) = compression_plan::table_source(&plan_document) {
        sources.push(source.to_string());
    }
    Ok((
        built,
        dedup_sources(sources),
        serde_json::json!({"decoded_size": decoded_total, "tokens": tokens, "streams": plans.len(), "components": reports}),
    ))
}
fn closure_self_test() -> Result<String, String> {
    let root = repository_root();
    let missing = root.join("out/__self_test_missing__/index.json");
    if missing.exists() {
        return Err("closure package self-test path exists".to_string());
    }
    let mut present_regions = 0;
    for game in ["THE BROKEN SEAL", "THE LOST AGE"] {
        let index = root
            .join("games")
            .join(game)
            .join("SOUND/SAMPLE/SAMPLES.TSV");
        let text = fs::read_to_string(index)
            .map_err(|error| format!("{game} PCM self-test index: {error}"))?;
        let mut rows = text.lines().filter(|line| !line.starts_with('#'));
        if rows.next() != Some("sample\taddress\tfrequency\tloop_start\tsample_count\tsource") {
            return Err(format!("{game} PCM self-test index differs"));
        }
        let count = rows.count();
        if count == 0 {
            return Err(format!("{game} PCM self-test index is empty"));
        }
        present_regions += count;
    }
    let overlapping = serde_json::json!([
        {"address": "0x08001000", "size": 16},
        {"address": "0x08001008", "size": 16}
    ]);
    if closure_coverage(overlapping.as_array().unwrap(), "self-test").is_ok() {
        return Err("overlapping closure coverage was accepted".to_string());
    }
    Ok(format!(
        "self-test=ok optional=skipped present_regions={present_regions} provenance=verified"
    ))
}
struct Context {
    root: PathBuf,
    /// The game being built: its owner register names the Thumb functions
    /// typed tables point at, and its retained listings hold the overlays
    /// an overlay series names.
    game: crate::targets::DecompTarget,
    /// The LZSS compressors of the reference machine the build's manifest
    /// names; general- and palette-LZ streams cannot be encoded without them.
    lz_machine: Option<LzMachine>,
    documents: std::cell::RefCell<HashMap<PathBuf, std::rc::Rc<Value>>>,
    images:
        std::cell::RefCell<HashMap<PathBuf, std::rc::Rc<psynergy::assets::image::IndexedImage>>>,
    /// Every input path the build resolved or read, for the consumer audit.
    opened: std::cell::RefCell<BTreeSet<PathBuf>>,
    /// Shared-palette checks already passed, by image and declared palette.
    palettes: std::cell::RefCell<HashMap<(PathBuf, String), Option<String>>>,
}
impl Context {
    fn new(root: &Path) -> Self {
        Self::for_game(
            root,
            crate::targets::target_for(crate::targets::DEFAULT_TARGET),
        )
    }
    fn for_game(root: &Path, game: crate::targets::DecompTarget) -> Self {
        Self {
            root: root.to_path_buf(),
            game,
            lz_machine: None,
            documents: Default::default(),
            images: Default::default(),
            opened: Default::default(),
            palettes: Default::default(),
        }
    }
    /// [`check_shared_palette`] once per image and declared palette: an atlas
    /// holds many components, and the check reads every pixel of the image.
    fn shared_palette(
        &self,
        path: &Path,
        entry: &Value,
        image: &psynergy::assets::image::IndexedImage,
    ) -> Result<Option<String>, String> {
        let key = (
            self.resolved(path)?,
            format!("{}{}", entry["palette"], entry["pixel_format"]),
        );
        if let Some(checked) = self.palettes.borrow().get(&key) {
            return Ok(checked.clone());
        }
        let checked = check_shared_palette(&self.root, entry, image)?;
        self.palettes.borrow_mut().insert(key, checked.clone());
        Ok(checked)
    }
    fn source(&self, name: &str) -> Result<PathBuf, String> {
        let path = root_path(&self.root, name)?;
        self.opened.borrow_mut().insert(path.clone());
        Ok(path)
    }
    fn resolved(&self, path: &Path) -> Result<PathBuf, String> {
        if path.exists() {
            return Ok(path.to_path_buf());
        }
        let relative = path.strip_prefix(&self.root).map_err(|e| e.to_string())?;
        root_path(
            &self.root,
            relative.to_str().ok_or("asset path is not UTF-8")?,
        )
    }
    /// Repository-relative names of every input this build resolved or read.
    fn opened_names(&self) -> impl Iterator<Item = String> + '_ {
        let opened = self.opened.borrow().clone();
        opened
            .into_iter()
            .filter_map(|path| root_relative(&self.root, &path).ok())
    }
    fn indexed(
        &self,
        path: &Path,
    ) -> Result<std::rc::Rc<psynergy::assets::image::IndexedImage>, String> {
        let path = self.resolved(path)?;
        self.opened.borrow_mut().insert(path.clone());
        if let Some(image) = self.images.borrow().get(&path) {
            return Ok(image.clone());
        }
        let image = std::rc::Rc::new(
            indexed_png(&fs::read(&path).map_err(|e| e.to_string())?).map_err(|e| e.to_string())?,
        );
        self.images.borrow_mut().insert(path, image.clone());
        Ok(image)
    }
    fn document(&self, path: &Path) -> Result<std::rc::Rc<Value>, String> {
        let path = self.resolved(path)?;
        self.opened.borrow_mut().insert(path.clone());
        if let Some(value) = self.documents.borrow().get(&path) {
            return Ok(value.clone());
        }
        let mut document = json(&path)?;
        compression_plan::expand(&self.root, &mut document)?;
        let value = std::rc::Rc::new(document);
        self.documents.borrow_mut().insert(path, value.clone());
        Ok(value)
    }
}
#[test]
fn library_asset_builds_reject_invalid_plans_without_populating_caches() {
    let directory = tempfile::tempdir().unwrap();
    let root = directory.path();
    fs::write(root.join("index.json"), b"{}").unwrap();
    let mut ctx = Context::new(root);
    for kind in ["golden-sun-general-lz"] {
        let entry =
            serde_json::json!({"kind":kind,"source":"index.json","address":0,"resource_id":0});
        assert!(build_entry(&mut ctx, &entry).is_err(), "{kind}");
    }
}

#[test]
fn manifest_fill_is_a_byte_value_not_a_rom_lookup() {
    let directory = tempfile::tempdir().unwrap();
    let mut context = Context::new(directory.path());
    let mut entry = serde_json::json!({"kind":"byte-fill", "address":0, "size":9, "value":165});
    let (bytes, sources, _) = build_entry(&mut context, &entry).unwrap();
    assert_eq!(bytes, vec![165; 9]);
    assert!(sources.is_empty());
    entry["value"] = Value::from(256);
    assert!(build_entry(&mut context, &entry).is_err());
    entry["value"] = Value::from(-1);
    assert!(build_entry(&mut context, &entry).is_err());
}
/// A fill the manifest lists by itself is the linker's zero padding before a
/// fixed placement: it must end at a module base the resource directory
/// names (a slot below the directory table), at the directory table, or at
/// the end of the cartridge image. Where it starts is the end of the last
/// linked owner before it; a fill cannot stand for bytes after a placement.
fn check_fill_placements(
    regions: &[Value],
    directory: Option<&Value>,
    image_end: usize,
) -> Result<(), String> {
    let mut bases = BTreeSet::from([image_end]);
    if let Some(directory) = directory {
        let table = number(&directory["address"], "resource directory address")?;
        bases.insert(table);
        for slot in directory["slots"]
            .as_array()
            .ok_or("resource directory slots are not an array")?
        {
            if let Some(text) = slot.as_str().filter(|text| text.starts_with("0x")) {
                let address = number(&Value::from(text), "module base")?;
                if address < table {
                    bases.insert(address);
                }
            }
        }
    }
    for region in regions
        .iter()
        .filter(|region| region["kind"] == "byte-fill")
    {
        let address = number(&region["address"], "fill address")?;
        let end = address
            .checked_add(number(&region["size"], "fill size")?)
            .ok_or("fill region overflows")?;
        if !bases.contains(&end) {
            return Err(format!(
                "fill at 0x{address:08x} ends at 0x{end:08x}, which is no module base, resource directory or image end"
            ));
        }
    }
    Ok(())
}
#[test]
fn manifest_fills_end_at_fixed_placements() {
    let directory = serde_json::json!({
        "address":"0x08100000","slots":["base","self","0x08100100","0x08010000","alias:0x003","null"]
    });
    let fill = |address: &str, size: &str| serde_json::json!({"address":address,"size":size,"kind":"byte-fill","value":0});
    for fill in [
        fill("0x0800f000", "0x1000"),
        fill("0x080ff000", "0x1000"),
        fill("0x081f0000", "0x10000"),
    ] {
        check_fill_placements(&[fill], Some(&directory), 0x0820_0000).unwrap();
    }
    // A fill cannot end inside a module or after a resource of the archive.
    for fill in [fill("0x0800f000", "0xfff"), fill("0x08100100", "0x10")] {
        assert!(check_fill_placements(&[fill], Some(&directory), 0x0820_0000).is_err());
    }
    // Without a directory only the image end is a fixed placement.
    assert!(check_fill_placements(&[fill("0x0800f000", "0x1000")], None, 0x0820_0000).is_err());
    let typed = serde_json::json!({"address":"0x08000000","size":"0x10","kind":"typed-table"});
    check_fill_placements(&[typed], None, 0x0820_0000).unwrap();
}
fn expand_series(
    ctx: &mut Context,
    manifest: &Value,
    entries: &mut Vec<Value>,
) -> Result<(), String> {
    let series_list = manifest
        .get("series")
        .and_then(Value::as_array)
        .cloned()
        .unwrap_or_default();
    for series in &series_list {
        let kind = json_string(&series["kind"], "series kind")?;
        match kind {
            "golden-sun-native-source-series" => {
                let name = json_string(&series["index"], "native source index")?;
                let index = json(&ctx.source(name)?)?;
                native::validate(&index)?;
                entries.extend(
                    index["regions"]
                        .as_array()
                        .ok_or("native regions missing")?
                        .iter()
                        .cloned(),
                );
            }
            "golden-sun-delta7-still-series" => {
                let index_name = json_string(&series["index"], "delta7 index")?;
                let index = json(&ctx.source(index_name)?)?;
                let resources = index
                    .get("resources")
                    .and_then(Value::as_array)
                    .ok_or("delta7 index resources missing")?;
                let mut lookup = HashMap::new();
                for item in resources {
                    lookup.insert(
                        json_string(&item["id"], "delta7 id")?.to_ascii_lowercase(),
                        item.clone(),
                    );
                }
                for resource in series_values(series, "resources")? {
                    let id =
                        json_string(&resource["id"], "delta7 resource id")?.to_ascii_lowercase();
                    let indexed = lookup.get(&id).ok_or_else(|| {
                        format!("pre-rendered background index has no resource {id}")
                    })?;
                    let source = Path::new(index_name)
                        .parent()
                        .unwrap_or(Path::new("."))
                        .join(json_string(&indexed["file"], "delta7 file")?);
                    entries.push(serde_json::json!({
                        "address": resource.get("address"),
                        "size": resource.get("size"),
                        "kind": "golden-sun-delta7-still",
                        "width": series["width"],
                        "height": series["height"],
                        "palette_entries": series["palette_entries"],
                        "source": root_relative(&ctx.root, &ctx.source(&source.to_string_lossy())?)?
                    }));
                }
            }
            "golden-sun-general-lz-series" => {
                for resource in series_values(series, "resources")? {
                    let name = json_string(&resource["id"], "resource id")?.to_ascii_lowercase();
                    let directory = format!(
                        "{}{name}",
                        json_string(&series["source_prefix"], "series source prefix")?
                    );
                    entries.push(serde_json::json!({
                        "address": resource.get("address"),
                        "size": resource.get("size"),
                        "kind": "golden-sun-general-lz",
                        "plan": format!("{directory}.json"),
                        "plan_section": "compression"
                    }));
                }
            }
            "golden-sun-map-charblock-series" => {
                let char_layout = series
                    .get("charblock_layout")
                    .cloned()
                    .unwrap_or(Value::Null);
                let animation_layout = series
                    .get("animation_layout")
                    .cloned()
                    .unwrap_or(Value::Null);
                for family in series_values(series, "families")? {
                    let tuple = family.as_array().ok_or("charblock family is malformed")?;
                    let name = json_string(&tuple[0], "charblock family id")?.to_ascii_lowercase();
                    let directory = format!(
                        "{}{name}",
                        json_string(&series["source_prefix"], "series source prefix")?
                    );
                    entries.push(serde_json::json!({
                        "address": tuple[1],
                        "size": tuple[2],
                        "kind": "golden-sun-general-lz",
                        "plan": format!("{directory}.json"),
                        "plan_section": "palette",
                        "components": [{"kind":"gba-palette","size":"0x1c0","source":format!("{directory}_palette.224.png")}]
                    }));
                    for (index, raw) in tuple[5..].iter().enumerate() {
                        let item = raw.as_array().ok_or("charblock tuple malformed")?;
                        let bank = index + 1;
                        let object_source = item.get(2).and_then(Value::as_str);
                        let component_kind = if object_source.is_some() {
                            "gba-4bpp-object-bank"
                        } else {
                            "gba-4bpp-tiles"
                        };
                        let source = object_source
                            .map(str::to_string)
                            .unwrap_or_else(|| format!("{directory}_charblock{bank}.4bpp.png"));
                        entries.push(serde_json::json!({
                            "address": item[0],
                            "size": item[1],
                            "kind": "golden-sun-kind2-lz",
                            "plan": format!("{directory}.json"),
                            "plan_section": format!("charblock{bank}"),
                            "layout": char_layout,
                            "components": [{"kind":component_kind,"size":"0x4000","source":source}]
                        }));
                    }
                    if !tuple[3].is_null() {
                        entries.push(serde_json::json!({
                            "address": tuple[3],
                            "size": tuple[4],
                            "kind": "golden-sun-kind2-lz",
                            "plan": format!("{directory}.json"),
                            "plan_section": "animation_source",
                            "layout": animation_layout,
                            "components": [{"kind":"gba-4bpp-tiles","size":"0x4000","source":format!("{directory}_animation_source.4bpp.png")}]
                        }));
                    }
                }
            }
            "golden-sun-standalone-palette-series" => {
                for palette in series_values(series, "palettes")? {
                    let name = json_string(&palette["id"], "palette id")?.to_ascii_lowercase();
                    let directory = format!(
                        "{}{name}",
                        json_string(&series["source_prefix"], "series source prefix")?
                    );
                    entries.push(serde_json::json!({
                        "address":palette.get("address"),"size":palette.get("size"),
                        "kind":"golden-sun-general-lz","plan":format!("{directory}.json"),"plan_section":"palette",
                        "components":[{"kind":"gba-palette","size":"0x1c0","source":format!("{directory}_palette.224.png")}]
                    }));
                }
            }
            "golden-sun-color-table-series" => {
                for resource in series_values(series, "resources")? {
                    let name = json_string(&resource["id"], "color table id")?.to_ascii_lowercase();
                    let directory = format!(
                        "{}{name}",
                        json_string(&series["source_prefix"], "series source prefix")?
                    );
                    entries.push(serde_json::json!({"address":resource.get("address"),"size":resource.get("size"),"kind":"gba-palette-rgba","source":format!("{directory}_color_table.rgba.png")}));
                }
            }
            "golden-sun-standalone-tile-series" => {
                for resource in series_values(series, "resources")? {
                    let name = json_string(&resource["id"], "tile id")?.to_ascii_lowercase();
                    let directory = format!(
                        "{}{name}",
                        json_string(&series["source_prefix"], "series source prefix")?
                    );
                    entries.push(serde_json::json!({"address":resource.get("address"),"size":resource.get("size"),"kind":"golden-sun-kind2-lz","plan":format!("{directory}_tiles.kind2.json"),"components":[{"kind":"gba-4bpp-tiles","size":"0x4000","source":format!("{directory}_tiles.4bpp.png")}] }));
                }
            }
            "golden-sun-thumb-overlay-series" => {
                for resource in series_values(series, "resources")? {
                    let tuple = resource
                        .as_array()
                        .ok_or("overlay resource tuple malformed")?;
                    let name = json_string(&tuple[0], "overlay id")?.to_ascii_lowercase();
                    // Overlays are the game's retained overlay listings
                    // unless the series names another directory.
                    let prefix = match series.get("source_prefix") {
                        Some(prefix) => json_string(prefix, "overlay series source prefix")?.into(),
                        None => format!("{}/resource_", ctx.game.overlay_dir()),
                    };
                    let directory = format!("{prefix}{name}");
                    // Each overlay is a stream file: the packer, replayed
                    // over the manifest's resource directory, aligns it.
                    entries.push(serde_json::json!({"address":tuple[1],"size":tuple[2],"kind":"golden-sun-general-lz","components":[{"kind":"golden-sun-thumb-overlay","size":tuple[3],"source":format!("{directory}_overlay.s"),"base":series.get("base")}] }));
                }
            }
            "golden-sun-map-component-series" => {
                // A family is [id, container address, header size, [slot,
                // address, size]...]: the container header is a typed table
                // whose offset words must agree with the listed components,
                // and each slot is an LZ stream or typed table whose plan and
                // values live in the family's JSON document. The Broken Seal
                // headers carry six component offsets; The Lost Age's carry
                // seven, so the header is 0x24 bytes plus four per slot.
                for family in series_values(series, "families")? {
                    let tuple = family.as_array().ok_or("map family malformed")?;
                    let name = json_string(&tuple[0], "map family id")?.to_ascii_lowercase();
                    let directory = format!(
                        "{}{name}",
                        json_string(&series["source_prefix"], "map series source prefix")?
                    );
                    let source = format!("{directory}.json");
                    let document = json(&ctx.source(&source)?)?;
                    let container = number(&tuple[1], "map container")?;
                    let slots = map_component_slots(number(&tuple[2], "map header size")?)?;
                    let mut offsets = vec![0usize; slots];
                    entries.push(serde_json::json!({"address":tuple[1],"size":tuple[2],"kind":"typed-table","source":source,"pointer":"/header"}));
                    for raw in &tuple[3..] {
                        let item = raw.as_array().ok_or("map component malformed")?;
                        let slot = number(&item[0], "map component slot")?;
                        let address = number(&item[1], "map component address")?;
                        if slot >= slots || offsets[slot] != 0 || address <= container {
                            return Err(
                                "map component slots must be unique and follow the header".into()
                            );
                        }
                        offsets[slot] = address - container;
                        let section = map_component_section(slots, slot);
                        let component = match slot {
                            0 => {
                                serde_json::json!({"kind":"gba-tilemap16","pointer":"/metatiles/tilemap","delta_mode":document["metatiles"]["transform_mode"]})
                            }
                            1 => {
                                serde_json::json!({"kind":"u8-array","pointer":"/descriptors/records"})
                            }
                            2 => {
                                let plan = format!("{directory}_grid.lz.json");
                                let decoded_size =
                                    json(&ctx.source(&plan)?)?["decoded_size"].clone();
                                entries.push(serde_json::json!({"address":item[1],"size":item[2],"kind":"golden-sun-general-lz","plan":plan,"components":[{"kind":"indexed-bytes","size":decoded_size,"source":format!("{directory}_grid_content.png")}]}));
                                continue;
                            }
                            _ if document[section].get("codec").is_none() => {
                                // Raw typed segments stay tables until their meaning is known.
                                entries.push(serde_json::json!({"address":item[1],"size":item[2],"kind":"typed-table","source":source,"pointer":format!("/{section}")}));
                                continue;
                            }
                            _ if document[section].get("words").is_some() => {
                                serde_json::json!({"kind":"le-u16-array","pointer":format!("/{section}/words")})
                            }
                            _ => {
                                serde_json::json!({"kind":"u8-array","pointer":format!("/{section}/values")})
                            }
                        };
                        let mut component = component;
                        component["source"] = Value::from(source.as_str());
                        component["size"] = document[section]["decoded_size"].clone();
                        entries.push(serde_json::json!({"address":item[1],"size":item[2],"kind":"golden-sun-general-lz","plan":source,"plan_section":section,"components":[component]}));
                    }
                    let declared = document["header"]["segments"]
                        .as_array()
                        .into_iter()
                        .flatten()
                        .find(|segment| segment["name"] == "component_offsets")
                        .and_then(|segment| segment["values"].as_array())
                        .ok_or_else(|| format!("map {name} header lacks component offsets"))?
                        .iter()
                        .map(|value| number(value, "component offset"))
                        .collect::<Result<Vec<_>, _>>()?;
                    if declared != offsets {
                        return Err(format!(
                            "map {name} header offsets differ from its listed components"
                        ));
                    }
                }
            }
            "golden-sun-sound-sequence-series" => {
                let index_name = json_string(&series["index"], "sequence index")?;
                if !index_name.to_ascii_lowercase().ends_with(".tsv") {
                    return Err("sequence series requires its canonical TSV table".into());
                }
                let index_path = ctx.source(index_name)?;
                let text = fs::read_to_string(&index_path).map_err(|error| error.to_string())?;
                let mut rows = text.lines().filter(|line| !line.starts_with('#'));
                if rows.next() != Some("sound_id\tclass\taddress\tsize\tsource") {
                    return Err("sequence table header differs".to_string());
                }
                let directory = Path::new(index_name).parent().unwrap_or(Path::new("."));
                let mut previous = None;
                for row in rows {
                    let fields = row.split('\t').collect::<Vec<_>>();
                    if fields.len() != 5 {
                        return Err("sequence table row width differs".to_string());
                    }
                    let id = fields[0]
                        .parse::<usize>()
                        .map_err(|_| "sequence table sound ID differs".to_string())?;
                    if previous.is_some_and(|value| id <= value)
                        || !matches!(fields[1], "music" | "sfx")
                    {
                        return Err(format!("sequence table row {id} identity differs"));
                    }
                    previous = Some(id);
                    let source = directory
                        .join(fields[4])
                        .to_string_lossy()
                        .replace('\\', "/");
                    ctx.source(&source)?;
                    entries.push(serde_json::json!({
                        "address":fields[2],
                        "size":fields[3],
                        "kind":"golden-sun-sound-sequence",
                        "source":source,
                    }));
                }
                if previous.is_none() {
                    return Err("sequence table is empty".to_string());
                }
            }
            "golden-sun-pcm-wave-series" => {
                let index_name = json_string(&series["index"], "PCM index")?;
                if !index_name.to_ascii_lowercase().ends_with(".tsv") {
                    return Err("PCM series requires its canonical TSV table".into());
                }
                let index_path = ctx.source(index_name)?;
                let text = fs::read_to_string(&index_path).map_err(|error| error.to_string())?;
                let mut rows = text.lines().filter(|line| !line.starts_with('#'));
                if rows.next()
                    != Some("sample\taddress\tfrequency\tloop_start\tsample_count\tsource")
                {
                    return Err("PCM table header differs".to_string());
                }
                let directory = Path::new(index_name).parent().unwrap_or(Path::new("."));
                for (sample, row) in rows.enumerate() {
                    let fields = row.split('\t').collect::<Vec<_>>();
                    if fields.len() != 6 || fields[0] != sample.to_string() {
                        return Err(format!("PCM table row {sample} identity differs"));
                    }
                    let frequency = fields[2]
                        .parse::<u32>()
                        .map_err(|_| format!("PCM table row {sample} frequency differs"))?;
                    let loop_start =
                        if fields[3].is_empty() {
                            Value::Null
                        } else {
                            Value::from(fields[3].parse::<u32>().map_err(|_| {
                                format!("PCM table row {sample} loop point differs")
                            })?)
                        };
                    let sample_count = fields[4]
                        .parse::<usize>()
                        .map_err(|_| format!("PCM table row {sample} extent differs"))?;
                    let size = (16usize + sample_count + 3) & !3;
                    let source = directory
                        .join(fields[5])
                        .to_string_lossy()
                        .replace('\\', "/");
                    ctx.source(&source)?;
                    entries.push(serde_json::json!({
                        "address":fields[1],
                        "size":size,
                        "frequency":frequency,
                        "loop_start":loop_start,
                        "kind":"golden-sun-pcm-wave",
                        "source":source,
                        "index":index_name,
                    }));
                }
            }
            _ => return Err(format!("unsupported asset series: {kind}")),
        }
    }
    Ok(())
}
/// Component slots a map container header declares: 0x24 bytes of parameters
/// and records, then one u32 offset per slot (six in TBS, seven in TLA).
fn map_component_slots(header_size: usize) -> Result<usize, String> {
    match header_size
        .checked_sub(0x24)
        .map(|words| (words % 4, words / 4))
    {
        Some((0, slots @ (6 | 7))) => Ok(slots),
        _ => Err(format!(
            "map container header 0x{header_size:x} does not hold six or seven component offsets"
        )),
    }
}
/// The document section of one slot, named after the engine's reader. The Lost
/// Age loader (0x0802a6b8) decompresses a 128x128 grid of descriptor indices
/// after the cell grid, which the cell readers from 0x0802d45c onward use to
/// index the descriptors at 0x0202c000; The Broken Seal keeps that index in
/// each grid cell. Its animation queues and blend animation follow in The
/// Broken Seal's formats, and its last slot replaces the sparse cells with the
/// lists of points, entrances, flagged cell patches and camera bounds that
/// 0x080ca9cc, 0x080cc7c4 and 0x0802b63c read at 0x0202e000.
fn map_component_section(slots: usize, slot: usize) -> &'static str {
    const BROKEN_SEAL: [&str; 6] = [
        "metatiles",
        "descriptors",
        "grid",
        "animation_queues",
        "blend_animation",
        "sparse_cells",
    ];
    const LOST_AGE: [&str; 7] = [
        "metatiles",
        "descriptors",
        "grid",
        "descriptor_grid",
        "animation_queues",
        "blend_animation",
        "positions",
    ];
    if slots == 7 {
        LOST_AGE[slot]
    } else {
        BROKEN_SEAL[slot]
    }
}
#[test]
fn map_headers_hold_six_or_seven_component_offsets() {
    assert_eq!(map_component_slots(0x3c).unwrap(), 6);
    assert_eq!(map_component_slots(0x40).unwrap(), 7);
    for invalid in [0, 0x24, 0x38, 0x3e, 0x44] {
        assert!(map_component_slots(invalid).is_err());
    }
    assert_eq!(map_component_section(6, 5), "sparse_cells");
    assert_eq!(map_component_section(7, 3), "descriptor_grid");
    assert_eq!(map_component_section(7, 4), "animation_queues");
    assert_eq!(map_component_section(7, 6), "positions");
}
fn series_values<'a>(value: &'a Value, key: &str) -> Result<&'a Vec<Value>, String> {
    value
        .get(key)
        .and_then(Value::as_array)
        .ok_or_else(|| format!("{key} is missing or is not an array"))
}
#[test]
fn overlay_series_leave_alignment_to_the_packer_and_compress_automatically() {
    let directory = tempfile::tempdir().unwrap();
    let mut ctx = Context::new(directory.path());
    let manifest = serde_json::json!({"series":[{
        "kind":"golden-sun-thumb-overlay-series", "source_prefix":"overlay/resource_",
        "resources":[["001", "0x08000000", 8, 16]]
    }]});
    let mut entries = Vec::new();
    expand_series(&mut ctx, &manifest, &mut entries).unwrap();
    assert!(entries[0].get("stream_alignment").is_none());
    assert!(entries[0].get("plan").is_none());
    fs::create_dir(directory.path().join("overlay")).unwrap();
    fs::write(
        directory.path().join("overlay/resource_001_stream.lz.json"),
        "{}",
    )
    .unwrap();
    entries.clear();
    expand_series(&mut ctx, &manifest, &mut entries).unwrap();
    assert!(entries[0].get("stream_alignment").is_none());
    assert!(entries[0].get("plan").is_none());
    assert!(build_general_lz_cached(&ctx, &serde_json::json!({"components":[]})).is_err());
}

#[test]
fn overlay_series_default_to_the_games_own_listings() {
    let directory = tempfile::tempdir().unwrap();
    let manifest = serde_json::json!({"series":[{
        "kind":"golden-sun-thumb-overlay-series", "base":"0x02000000",
        "resources":[["64a", "0x08ec3d08", "0x1d10", "0x274a"]]
    }]});
    for (id, directory_name) in [
        (
            crate::targets::DecompTargetId::TbsEn,
            "recon/tbs/raw/overlays",
        ),
        (
            crate::targets::DecompTargetId::TlaEn,
            "recon/tla/raw/overlays",
        ),
    ] {
        let mut ctx = Context::for_game(directory.path(), crate::targets::target_for(id));
        let mut entries = Vec::new();
        expand_series(&mut ctx, &manifest, &mut entries).unwrap();
        assert_eq!(
            entries[0]["components"][0]["source"],
            format!("{directory_name}/resource_64a_overlay.s")
        );
    }
}
#[test]
fn sound_series_use_only_the_canonical_tables() {
    let directory = tempfile::tempdir().unwrap();
    let mut ctx = Context::new(directory.path());
    let sequence =
        "sound_id\tclass\taddress\tsize\tsource\n0\tmusic\t0x08000000\t16\tsongs/opening.mid\n";
    let samples = "sample\taddress\tfrequency\tloop_start\tsample_count\tsource\n0\t0x08000010\t8000\t\t5\tsamples/flute.wav\n";
    for (kind, table, source, expected_size) in [
        (
            "sound-sequence",
            sequence,
            "songs/opening.mid",
            serde_json::json!("16"),
        ),
        (
            "pcm-wave",
            samples,
            "samples/flute.wav",
            serde_json::json!(24),
        ),
    ] {
        let manifest = serde_json::json!({"series":[{
            "kind":format!("golden-sun-{kind}-series"), "index":"index.tsv"
        }]});
        fs::write(directory.path().join("index.tsv"), table).unwrap();
        let mut entries = Vec::new();
        expand_series(&mut ctx, &manifest, &mut entries).unwrap();
        assert_eq!(entries.len(), 1);
        assert_eq!(entries[0]["source"], source);
        assert_eq!(entries[0]["size"], expected_size);
        for invalid in [
            table.replacen('\t', ",", 1),
            table.replace("\n0\t", "\nno-id\t"),
        ] {
            fs::write(directory.path().join("index.tsv"), invalid).unwrap();
            assert!(expand_series(&mut ctx, &manifest, &mut Vec::new()).is_err());
        }
        let mut legacy = manifest;
        legacy["series"][0]["index"] = serde_json::json!("index.json");
        assert!(expand_series(&mut ctx, &legacy, &mut Vec::new())
            .unwrap_err()
            .contains("canonical TSV"));
    }
}
fn closure_coverage(items: &[Value], label: &str) -> Result<Vec<(usize, usize)>, String> {
    let mut regions = Vec::new();
    for (index, item) in items.iter().enumerate() {
        if !item.is_object() || item.is_array() {
            return Err(format!("{label} region {index} differs"));
        }
        let address = number(&item["address"], "closure address")?;
        let size = number(&item["size"], "closure size")?;
        if size == 0 || address < 0x0800_0000 || address + size > 0x0880_0000 {
            return Err(format!("{label} region {index} extent differs"));
        }
        regions.push((address, address + size));
    }
    regions.sort_unstable_by_key(|(address, _)| *address);
    let mut merged: Vec<(usize, usize)> = Vec::new();
    for (address, end) in regions {
        if let Some(previous) = merged.last_mut() {
            if address < previous.1 {
                return Err(format!("{label} regions overlap"));
            }
            if address == previous.1 {
                previous.1 = end;
                continue;
            }
        }
        merged.push((address, end));
    }
    Ok(merged)
}
fn expand_closure_packages(
    ctx: &mut Context,
    manifest: &Value,
    entries: &mut Vec<Value>,
) -> Result<(), String> {
    let supported = ["golden-sun-asset-fragment", "golden-sun-pcm-wave-series"];
    for package in manifest
        .get("closure_packages")
        .and_then(Value::as_array)
        .unwrap_or(&Vec::new())
    {
        let object = package.as_object().ok_or("closure package differs")?;
        let id = json_string(object.get("id").ok_or("closure id missing")?, "closure id")?;
        let kind = json_string(
            object.get("kind").ok_or("closure kind missing")?,
            "closure kind",
        )?;
        let availability = json_string(
            object
                .get("availability")
                .ok_or("closure availability missing")?,
            "closure availability",
        )?;
        let index_name = json_string(
            object.get("index").ok_or("closure index missing")?,
            "closure index",
        )?;
        let expected = object
            .get("expected_ranges")
            .and_then(Value::as_array)
            .ok_or("closure expected_ranges missing")?;
        if !supported.contains(&kind)
            || !matches!(availability, "pending" | "required")
            || expected.is_empty()
        {
            return Err(format!("closure package {id} differs"));
        }
        let index_path = ctx.source(index_name)?;
        if !index_path.exists() {
            if availability == "pending" {
                continue;
            }
            return Err(format!("{id}: required closure package is missing"));
        }
        let document = json(&index_path)?;
        let mut generated = Vec::new();
        if kind == "golden-sun-asset-fragment" {
            if document.get("format") != Some(&Value::from(1))
                || document.get("kind").and_then(Value::as_str) != Some(kind)
                || document.get("id").and_then(Value::as_str) != Some(id)
            {
                return Err(format!("{id}: closure package identity differs"));
            }
            generated.extend(series_values(&document, "regions")?.iter().cloned());
            let local = serde_json::json!({"series": document.get("series").cloned().unwrap_or(Value::Array(Vec::new()))});
            expand_series(ctx, &local, &mut generated)?;
        } else {
            let local = serde_json::json!({"series":[{"kind":kind,"index":index_name}]});
            expand_series(ctx, &local, &mut generated)?;
        }
        let expected_coverage = closure_coverage(expected, &format!("{id} expected coverage"))?;
        let actual_coverage = closure_coverage(&generated, &format!("{id} generated coverage"))?;
        if expected_coverage != actual_coverage {
            return Err(format!(
                "{id}: generated coverage differs from its declared extent"
            ));
        }
        for mut entry in generated {
            if entry.get("_closure_package").is_some() {
                return Err(format!("{id}: nested closure provenance differs"));
            }
            entry["_closure_package"] = Value::String(index_name.to_string());
            entries.push(entry);
        }
    }
    Ok(())
}
fn closure_sources(
    ctx: &Context,
    entry: &Value,
    mut sources: Vec<String>,
) -> Result<Vec<String>, String> {
    if let Some(package) = entry.get("_closure_package").and_then(Value::as_str) {
        ctx.source(package)?;
        if !sources.iter().any(|source| source == package) {
            sources.insert(0, package.to_string());
        }
    }
    Ok(dedup_sources(sources))
}
fn build_entry(ctx: &mut Context, entry: &Value) -> Result<(Vec<u8>, Vec<String>, Value), String> {
    let kind = json_string(&entry["kind"], "asset kind")?;
    let address = number(&entry["address"], "asset address")?;
    let entry_source = entry.get("source").and_then(Value::as_str).unwrap_or("");
    let source_path = |name: &str| ctx.source(name);
    match kind {
        "byte-fill" => {
            let size = number(&entry["size"], "fill size")?;
            let value = u8::try_from(number(&entry["value"], "fill byte")?)
                .map_err(|_| "fill byte exceeds 255")?;
            Ok((
                vec![value; size],
                Vec::new(),
                serde_json::json!({"value":value}),
            ))
        }
        "gba-cartridge-header-standard-fields" => {
            let document = read_gba_header_source(&source_path(entry_source)?)?;
            let size = number(&entry["size"], "header size")?;
            let built = build_gba_header_component(&document, address as u32, size)?;
            Ok((
                built.clone(),
                vec![entry_source.to_string()],
                serde_json::json!({"standard_header_bytes":built.len()}),
            ))
        }
        "gba-4bpp-tiles"
        | "bgr555-banks"
        | "gba-8bpp-tiles"
        | "1bpp-tiles"
        | "gba-palette"
        | "gba-palette-rgba"
        | "indexed-bytes"
        | "u8-array"
        | "s8-array"
        | "be-s16-array"
        | "le-u16-array"
        | "le-u32-array"
        | "rgba-bytes"
        | "zero-skip-bytes"
        | "zero-skip-bank"
        | "mtf4-bytes"
        | "golden-sun-thumb-overlay" => {
            let result = build_component_cached(ctx, entry)?;
            Ok((result.data, result.sources, result.details))
        }
        "components" => {
            let (parts, mut sources) = if let Some(parts) = entry.get("components") {
                (parts.clone(), Vec::new())
            } else {
                let pointer = entry
                    .get("pointer")
                    .map(|pointer| json_string(pointer, "components pointer"))
                    .transpose()?
                    .unwrap_or("/components");
                let document = ctx.document(&source_path(entry_source)?)?;
                let parts = document
                    .pointer(pointer)
                    .ok_or("components pointer is absent")?
                    .clone();
                (parts, vec![entry_source.to_string()])
            };
            let mut built = Vec::new();
            let mut reports = Vec::new();
            for part in parts.as_array().ok_or("components are not an array")? {
                let mut part = part.clone();
                let offset = address + built.len();
                if let Some(declared) = part.get("address") {
                    if number(declared, "component address")? != offset {
                        return Err(format!(
                            "component at 0x{offset:08x} declares address {declared}"
                        ));
                    }
                }
                part["address"] = Value::from(offset);
                let size = number(&part["size"], "component size")?;
                let (data, part_sources, _) = build_entry(ctx, &part).map_err(|error| {
                    format!(
                        "component at 0x{offset:08x} ({}): {error}",
                        part["kind"].as_str().unwrap_or("unknown")
                    )
                })?;
                if data.len() != size {
                    return Err(format!(
                        "component at 0x{offset:08x}: built 0x{:x}, expected 0x{size:x}",
                        data.len()
                    ));
                }
                built.extend(data);
                reports.push(serde_json::json!({"kind": part.get("kind"), "address": hex_address(offset), "size": size, "sources": part_sources}));
                sources.extend(part_sources);
            }
            Ok((
                built,
                dedup_sources(sources),
                serde_json::json!({"components": reports}),
            ))
        }
        "golden-sun-general-lz" => {
            let (built, sources, report) = build_general_lz_cached(ctx, entry)?;
            Ok((built, sources, report))
        }
        "golden-sun-kind2-lz" => {
            let components = entry
                .get("components")
                .and_then(Value::as_array)
                .ok_or("kind-2 components are not an array")?;
            let mut decoded = Vec::new();
            let mut sources = Vec::new();
            let mut reports = Vec::new();
            for component in components {
                let result = build_component_cached(ctx, component)?;
                decoded.extend(result.data);
                sources.extend(result.sources);
                reports.push(serde_json::json!({"kind":component.get("kind"),"source":component.get("source"),"details":result.details}));
            }
            let plan_name = json_string(&entry["plan"], "kind-2 plan")?;
            let plan_path = source_path(plan_name)?;
            let plan_document = ctx.document(&plan_path)?;
            let plan = select_plan(&plan_document, entry)?;
            if let Some(layout) = entry.get("layout") {
                if layout != &Value::Null && plan.get("layout") != Some(layout) {
                    return Err("tag-2 plan layout differs from manifest".to_string());
                }
            }
            if plan["format"] != 1 || plan["codec"] != "golden-sun-kind2-lz" {
                return Err("unsupported tag-2 plan".to_string());
            }
            if decoded.len() != number(&plan["decoded_size"], "decoded_size")? {
                return Err("decoded tag-2 components do not match plan".to_string());
            }
            // Tag-2 keeps its own greedy window; no machine setting applies.
            let materialized = compression_plan::materialize(&decoded, plan, None)?;
            let tokens = materialized
                .as_array()
                .ok_or("tag-2 tokens must be an array")?
                .iter()
                .map(|token| {
                    // A bare width is a literal; `[distance, length]` a copy.
                    if token.is_u64() {
                        return Ok(psynergy::assets::lz::Mtf4LzToken::Literal {
                            width: number(token, "tag-2 literal width")? as u32,
                        });
                    }
                    match token.as_array().map(Vec::as_slice) {
                        Some([distance, length]) => Ok(psynergy::assets::lz::Mtf4LzToken::Copy {
                            length: number(length, "tag-2 copy length")? as u32,
                            distance: number(distance, "tag-2 copy distance")? as u32,
                        }),
                        _ => Err("invalid tag-2 token".to_string()),
                    }
                })
                .collect::<Result<Vec<_>, _>>()?;
            let built = psynergy::assets::lz::encode_mtf4_lz(&decoded, &tokens)
                .map_err(|error| error.to_string())?;
            check_stored_extent(plan, built.len())?;
            sources.push(plan_name.to_string());
            if let Some(source) = compression_plan::table_source(&plan_document) {
                sources.push(source.to_string());
            }
            Ok((
                built,
                dedup_sources(sources),
                serde_json::json!({"decoded_size":decoded.len(),"tokens":tokens.len(),"layout":plan.get("layout").cloned().unwrap_or(Value::Null),"components":reports}),
            ))
        }
        "record-table" | "pointer-table" => {
            let document = ctx.document(&source_path(entry_source)?)?;
            let document = match entry.get("pointer") {
                Some(pointer) => document
                    .pointer(json_string(pointer, "table pointer")?)
                    .ok_or("table pointer is absent")?,
                None => &document,
            };
            if document["format"] != 1 || document["kind"] != kind {
                return Err("table identity differs".into());
            }
            if kind == "pointer-table" && number(&document["address"], "table address")? != address
            {
                return Err("table address differs from manifest".into());
            }
            let built = if kind == "record-table" {
                record_table(&document)?
            } else {
                pointer_table(&document)?
            };
            Ok((
                built,
                vec![entry_source.to_string()],
                serde_json::json!({"representation":kind}),
            ))
        }
        _ => build_entry_native_tail(ctx, entry, kind, address, entry_source),
    }
}
const SEQUENCE_DURATIONS: [usize; 49] = [
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 28,
    30, 32, 36, 40, 42, 44, 48, 52, 54, 56, 60, 64, 66, 68, 72, 76, 78, 80, 84, 88, 90, 92, 96,
];
fn sequence_control_opcode(name: &str) -> Option<u8> {
    Some(match name {
        "priority" => 0xba,
        "tempo" => 0xbb,
        "key_shift" => 0xbc,
        "voice" => 0xbd,
        "volume" => 0xbe,
        "pan" => 0xbf,
        "pitch_bend" => 0xc0,
        "pitch_bend_range" => 0xc1,
        "lfo_speed" => 0xc2,
        "lfo_delay" => 0xc3,
        "modulation_depth" => 0xc4,
        "modulation_type" => 0xc5,
        "tuning" => 0xc8,
        _ => return None,
    })
}
fn sequence_sets_running_status(opcode: u8) -> bool {
    opcode != 0xbb
}
fn sequence_duration_index(value: &Value, label: &str) -> Result<u8, String> {
    let ticks = number(value, label)?;
    SEQUENCE_DURATIONS
        .iter()
        .position(|candidate| *candidate == ticks)
        .map(|index| index as u8)
        .ok_or_else(|| format!("{label} is not representable by the engine duration table"))
}
fn sequence_address(value: &Value, label: &str) -> Result<u32, String> {
    let address = number(value, label)?;
    if !(0x0800_0000..0x0a00_0000).contains(&address) {
        return Err(format!("{label} is not a ROM address"));
    }
    Ok(address as u32)
}
fn sequence_symbol(value: &Value, label: &str) -> Result<String, String> {
    let symbol = json_string(value, label)?;
    if symbol.is_empty()
        || !symbol.chars().enumerate().all(|(index, character)| {
            if index == 0 {
                character.is_ascii_lowercase()
            } else {
                character.is_ascii_lowercase() || character.is_ascii_digit() || character == '_'
            }
        })
    {
        return Err(format!("{label} is invalid"));
    }
    Ok(symbol.to_string())
}
fn sequence_parameter(value: &Value, name: &str) -> Result<u8, String> {
    let number = number(value, name)? as i64;
    let signed = matches!(name, "key_shift" | "pan" | "pitch_bend" | "tuning");
    if signed {
        if !(-128..=127).contains(&number) {
            return Err(format!("{name} does not fit s8"));
        }
        Ok((number as i8) as u8)
    } else if number <= 0xff {
        Ok(number as u8)
    } else {
        Err(format!("{name} does not fit u8"))
    }
}
fn sequence_note_parameters(event: &[Value], start: usize, label: &str) -> Result<Vec<u8>, String> {
    if event.len() < start || event.len() - start > 3 {
        return Err(format!("{label} has more than three parameters"));
    }
    event[start..]
        .iter()
        .enumerate()
        .map(|(index, value)| {
            let number = number(value, &format!("{label} parameter {index}"))?;
            if number >= 0x80 {
                return Err(format!("{label} parameter {index} must be below 0x80"));
            }
            Ok(number as u8)
        })
        .collect()
}
struct EncodedSequenceStream {
    data: Vec<u8>,
    labels: Vec<(String, usize)>,
    pointers: Vec<(usize, String)>,
    events: usize,
}
fn encode_sequence_stream(events: &[Value]) -> Result<EncodedSequenceStream, String> {
    let mut data = Vec::new();
    let mut local_labels = Vec::new();
    let mut pointers = Vec::new();
    let mut running: Option<u8> = None;
    let mut event_count = 0;
    for raw in events {
        let event = raw.as_array().ok_or("sequence event is malformed")?;
        let kind = event
            .first()
            .and_then(Value::as_str)
            .ok_or("sequence event has no kind")?;
        if kind == "label" {
            let name =
                sequence_symbol(event.get(1).ok_or("event label is missing")?, "event label")?;
            local_labels.push((name, data.len()));
            continue;
        }
        event_count += 1;
        let mut encoded = Vec::new();
        match kind {
            "wait" => encoded.push(
                0x80 + sequence_duration_index(
                    event.get(1).ok_or("wait duration is missing")?,
                    "wait duration",
                )?,
            ),
            "fine" => {
                if event.len() != 1 {
                    return Err("fine takes no parameters".to_string());
                }
                encoded.push(0xb1);
            }
            "goto" | "pattern" => {
                if event.len() != 2 {
                    return Err(format!("{kind} takes one target"));
                }
                let target = sequence_symbol(
                    event.get(1).ok_or("sequence target is missing")?,
                    "sequence target",
                )?;
                encoded.push(if kind == "goto" { 0xb2 } else { 0xb3 });
                pointers.push((data.len() + 1, target));
                encoded.extend_from_slice(&[0; 4]);
            }
            "pattern_end" => {
                if event.len() != 1 {
                    return Err("pattern_end takes no parameters".to_string());
                }
                encoded.push(0xb4);
            }
            "repeat" => {
                if event.len() != 3 {
                    return Err("repeat requires a count and target".to_string());
                }
                let count = number(
                    event.get(1).ok_or("repeat count is missing")?,
                    "repeat count",
                )?;
                if count > 0xff {
                    return Err("repeat count does not fit u8".to_string());
                }
                let target = sequence_symbol(
                    event.get(2).ok_or("repeat target is missing")?,
                    "repeat target",
                )?;
                encoded.push(0xb5);
                encoded.push(count as u8);
                pointers.push((data.len() + 2, target));
                encoded.extend_from_slice(&[0; 4]);
            }
            "note" => {
                let opcode = 0xcf
                    + sequence_duration_index(
                        event.get(1).ok_or("note duration is missing")?,
                        "note duration",
                    )?;
                encoded.push(opcode);
                encoded.extend(sequence_note_parameters(event, 2, "note")?);
                running = Some(opcode);
            }
            "note_running" => {
                let opcode = 0xcf
                    + sequence_duration_index(
                        event.get(1).ok_or("running note duration is missing")?,
                        "running note duration",
                    )?;
                let values = sequence_note_parameters(event, 2, "running note")?;
                if values.is_empty() {
                    return Err("running note emits no bytes".to_string());
                }
                if running.is_some_and(|active| active != opcode) {
                    return Err("running note status differs from active status".to_string());
                }
                encoded = values;
                running = Some(opcode);
            }
            "control_running" => {
                if event.len() != 3 {
                    return Err("control_running requires a name and value".to_string());
                }
                let name = json_string(
                    event.get(1).ok_or("running control name is missing")?,
                    "running control name",
                )?;
                let opcode = sequence_control_opcode(name).ok_or("unknown running control")?;
                if !sequence_sets_running_status(opcode) {
                    return Err(format!("{name} cannot use running status"));
                }
                let value = sequence_parameter(
                    event.get(2).ok_or("running control value is missing")?,
                    name,
                )?;
                if value >= 0x80 {
                    return Err(
                        "running control parameter would be parsed as a command".to_string()
                    );
                }
                if running.is_some_and(|active| active != opcode) {
                    return Err("running control status differs from active status".to_string());
                }
                encoded.push(value);
                running = Some(opcode);
            }
            "note_end_running" => {
                if event.len() != 2 {
                    return Err("running note_end requires a value".to_string());
                }
                let value = sequence_note_parameters(event, 1, "running note_end")?[0];
                if running.is_some_and(|active| active != 0xce) {
                    return Err("running note_end status differs from active status".to_string());
                }
                encoded.push(value);
                running = Some(0xce);
            }
            "note_end" => {
                if event.len() > 2 {
                    return Err("note_end has too many parameters".to_string());
                }
                encoded.push(0xce);
                if event.len() == 2 {
                    encoded.extend(sequence_note_parameters(event, 1, "note_end")?);
                }
                running = Some(0xce);
            }
            _ => {
                let opcode = sequence_control_opcode(kind)
                    .ok_or_else(|| format!("unsupported sequence event: {kind}"))?;
                if event.len() != 2 {
                    return Err(format!("{kind} requires one parameter"));
                }
                encoded.push(opcode);
                encoded.push(sequence_parameter(
                    event.get(1).ok_or("control value is missing")?,
                    kind,
                )?);
                if sequence_sets_running_status(opcode) {
                    running = Some(opcode);
                }
            }
        }
        data.extend(encoded);
    }
    Ok(EncodedSequenceStream {
        data,
        labels: local_labels,
        pointers,
        events: event_count,
    })
}
fn sequence_alignment_size(offset: usize, boundary: usize) -> Result<usize, String> {
    if !(2..=0x100).contains(&boundary) || !boundary.is_power_of_two() {
        return Err("alignment boundary must be a power of two from 2 through 256".to_string());
    }
    Ok((0usize.wrapping_sub(offset)) & (boundary - 1))
}
fn build_sequence_source(source: &Value) -> Result<(Vec<u8>, Value), String> {
    if number(&source["format"], "sequence format")? != 1
        || source["engine"].as_str() != Some("smsh-sequence")
    {
        return Err("unsupported sequence source".into());
    }
    let base = sequence_address(&source["base"], "sequence base")?;
    let externals = source["externals"]
        .as_object()
        .ok_or("sequence externals are missing")?;
    let mut external_addresses = HashMap::<String, u32>::new();
    for (name, value) in externals {
        external_addresses.insert(
            sequence_symbol(&Value::String(name.clone()), "external symbol")?,
            sequence_address(value, "external address")?,
        );
    }
    let layout = source["layout"]
        .as_array()
        .ok_or("sequence layout is missing")?;
    let mut labels = HashMap::<String, u32>::new();
    let mut define = |name, offset: usize| -> Result<(), String> {
        if labels.insert(name, base + offset as u32).is_some() {
            return Err("duplicate local label".into());
        }
        Ok(())
    };
    let mut output = Vec::new();
    let mut pointers = Vec::new();
    let mut used_externals = BTreeSet::new();
    let (mut stream_count, mut track_count, mut event_count) = (0usize, 0usize, 0usize);
    for (index, segment) in layout.iter().enumerate() {
        match segment["kind"].as_str() {
            Some("stream") => {
                let offset = output.len();
                define(sequence_symbol(&segment["label"], "stream label")?, offset)?;
                let encoded = encode_sequence_stream(
                    segment["events"]
                        .as_array()
                        .ok_or("stream events are missing")?,
                )?;
                for (name, inner) in encoded.labels {
                    define(name, offset + inner)?;
                }
                pointers.extend(
                    encoded
                        .pointers
                        .into_iter()
                        .map(|(site, name)| (offset + site, name)),
                );
                output.extend(encoded.data);
                stream_count += 1;
                event_count += encoded.events;
            }
            Some("align") => {
                let size = sequence_alignment_size(
                    base as usize + output.len(),
                    number(&segment["boundary"], "alignment boundary")?,
                )?;
                let fill = u8::try_from(number(&segment["fill"], "alignment fill")?)
                    .map_err(|_| "alignment fill does not fit u8")?;
                output.resize(output.len() + size, fill);
            }
            Some("header") => {
                define(
                    sequence_symbol(&segment["label"], "header label")?,
                    output.len(),
                )?;
                let tracks = segment["tracks"]
                    .as_array()
                    .ok_or("header tracks are missing")?;
                if tracks.is_empty() || tracks.len() > 16 {
                    return Err("header track list is invalid".into());
                }
                if number(&segment["block_count"], "header block_count")? != 0 {
                    return Err("nonzero sequence block_count is not supported".into());
                }
                let priority = u8::try_from(number(&segment["priority"], "header priority")?)
                    .map_err(|_| "header value does not fit u8")?;
                let reverb = u8::try_from(number(&segment["reverb"], "header reverb")?)
                    .map_err(|_| "header value does not fit u8")?;
                let tone_bank = sequence_symbol(&segment["tone_bank"], "tone bank")?;
                if external_addresses.contains_key(&tone_bank) {
                    used_externals.insert(tone_bank.clone());
                }
                output.extend_from_slice(&[tracks.len() as u8, 0, priority, reverb]);
                pointers.push((output.len(), tone_bank));
                output.extend_from_slice(&[0; 4]);
                for track in tracks {
                    pointers.push((output.len(), sequence_symbol(track, "track symbol")?));
                    output.extend_from_slice(&[0; 4]);
                }
                track_count += tracks.len();
            }
            _ => return Err(format!("unsupported layout segment {index}")),
        }
    }
    let label_count = labels.len();
    let mut resolved = labels;
    resolved.extend(
        external_addresses
            .iter()
            .map(|(name, address)| (name.clone(), *address)),
    );
    for (offset, name) in pointers {
        let address = resolved
            .get(&name)
            .ok_or_else(|| format!("unknown sequence symbol: {name}"))?;
        output[offset..offset + 4].copy_from_slice(&address.to_le_bytes());
    }
    let unused: Vec<_> = external_addresses
        .keys()
        .filter(|name| !used_externals.contains(*name))
        .cloned()
        .collect();
    if !unused.is_empty() {
        return Err(format!("unused sequence externals: {}", unused.join(", ")));
    }
    let report = serde_json::json!({
        "base": base,
        "end": base as usize + output.len(),
        "bytes": output.len(),
        "streams": stream_count,
        "tracks": track_count,
        "events": event_count,
        "labels": label_count,
    });
    Ok((output, report))
}
#[test]
fn sequence_emission_resolves_forward_and_backward_labels_and_rejects_invalid_layouts() {
    let source = serde_json::json!({
        "format": 1, "engine": "smsh-sequence", "base": "0x08000000",
        "externals": {"voicegroup": "0x08010000"},
        "layout": [
            {"kind": "header", "label": "song", "tracks": ["track"],
             "block_count": 0, "priority": 1, "reverb": 0, "tone_bank": "voicegroup"},
            {"kind": "align", "boundary": 16, "fill": 0},
            {"kind": "stream", "label": "track", "events": [
                ["goto", "end"], ["label", "loop"], ["note", 1, 60, 100],
                ["wait", 1], ["repeat", 2, "loop"], ["label", "end"],
                ["pattern", "loop"], ["fine"]
            ]}
        ]
    });
    let (bytes, report) = build_sequence_source(&source).unwrap();
    assert_eq!(
        bytes,
        [
            1, 0, 1, 0, 0, 0, 1, 8, 16, 0, 0, 8, 0, 0, 0, 0, 0xb2, 31, 0, 0, 8, 0xd0, 60, 100,
            0x81, 0xb5, 2, 21, 0, 0, 8, 0xb3, 21, 0, 0, 8, 0xb1,
        ]
    );
    assert_eq!(
        report,
        serde_json::json!({
            "base": 0x08000000, "end": 0x08000025, "bytes": 37,
            "streams": 1, "tracks": 1, "events": 6, "labels": 4,
        })
    );
    for (path, value, error) in [
        (
            "/layout/2/events/0/1",
            serde_json::json!("missing"),
            "unknown sequence symbol",
        ),
        (
            "/layout/2/label",
            serde_json::json!("song"),
            "duplicate local label",
        ),
        ("/layout/0/block_count", serde_json::json!(1), "nonzero"),
        ("/layout/0/priority", serde_json::json!(256), "header value"),
        ("/layout/0/reverb", serde_json::json!(256), "header value"),
        ("/layout/0/tracks", serde_json::json!([]), "track list"),
        ("/layout/1/boundary", serde_json::json!(3), "power of two"),
        ("/layout/1/fill", serde_json::json!(256), "alignment fill"),
    ] {
        let mut invalid = source.clone();
        *invalid.pointer_mut(path).unwrap() = value;
        assert!(
            build_sequence_source(&invalid).unwrap_err().contains(error),
            "{path}"
        );
    }
    let mut unused = source;
    unused["externals"]["unused"] = serde_json::json!("0x08010004");
    assert!(build_sequence_source(&unused)
        .unwrap_err()
        .contains("unused sequence externals"));
}
/// Prefix of the retired per-event sequence sidecars. The converter derives
/// every encoding choice, so a MIDI still carrying one is refused.
const MIDI_BUILD_DIRECTIVE: &[u8] = b"alchemy-mid2agb\0";
/// Bar lengths, in sequence ticks, tried in order when a playback MIDI is
/// adopted: four, three, two and one 24-tick beats, then one, seven, five and
/// three 12-tick half beats.
const ADOPTION_BARS: [i64; 8] = [96, 72, 48, 24, 12, 84, 60, 36];
/// Bar lines of a sequence MIDI: every time signature in the conductor track
/// starts bars of its length at its tick; before the first one, MIDI's 4/4.
fn sequence_meter(conductor: &[MidiEvent], division: u16) -> Result<Vec<(i64, i64)>, String> {
    let whole = i64::from(division) * 4;
    let mut meter = Vec::<(i64, i64)>::new();
    for event in conductor {
        let EventBody::Meta { meta: 0x58, data } = &event.body else {
            continue;
        };
        let [numerator, power, ..] = data.as_slice() else {
            return Err("MIDI time signature is truncated".to_string());
        };
        let span = i64::from(*numerator) * whole;
        if *numerator == 0 || *power > 7 || span % (1i64 << power) != 0 {
            return Err(format!(
                "MIDI time signature {numerator}/2^{power} does not fit the tick grid"
            ));
        }
        if meter.last().is_some_and(|(tick, _)| *tick >= event.tick) {
            return Err("MIDI time signatures share or reverse a tick".to_string());
        }
        meter.push((event.tick, span >> power));
    }
    if meter.first().is_none_or(|(tick, _)| *tick != 0) {
        meter.insert(0, (0, whole));
    }
    Ok(meter)
}
/// The time signature whose bars last `bar` sequence ticks, counted in 24-tick
/// beats, or in 12-tick half beats when the bar is not whole beats.
fn time_signature(bar: i64, division: u16) -> Result<[u8; 4], String> {
    let whole = i64::from(division) * 4;
    for unit in [24, 12] {
        let note = whole / unit;
        if bar % unit == 0 && note * unit == whole && note.count_ones() == 1 && bar / unit <= 0xff {
            return Ok([(bar / unit) as u8, note.trailing_zeros() as u8, 24, 8]);
        }
    }
    Err(format!(
        "a {bar}-tick bar has no time signature at {division} ticks per quarter"
    ))
}
/// A rest as the converter writes it: cut at every bar line, then into the
/// longest wait commands within each bar.
fn rest_waits(start: i64, length: i64, meter: &[(i64, i64)]) -> Result<Vec<usize>, String> {
    if start < 0 || length < 0 {
        return Err("MIDI rest lies before the stream start".to_string());
    }
    let mut waits = Vec::new();
    let (mut at, end) = (start, start + length);
    while at < end {
        let index = meter.partition_point(|(tick, _)| *tick <= at) - 1;
        let (origin, bar) = meter[index];
        let mut line = origin + ((at - origin) / bar + 1) * bar;
        if let Some((next, _)) = meter.get(index + 1) {
            line = line.min(*next);
        }
        let stop = line.min(end);
        let mut gap = (stop - at) as usize;
        while gap > 0 {
            let wait = SEQUENCE_DURATIONS
                .iter()
                .rev()
                .copied()
                .find(|duration| *duration != 0 && *duration <= gap)
                .ok_or("MIDI wait cannot be tokenized")?;
            waits.push(wait);
            gap -= wait;
        }
        at = stop;
    }
    Ok(waits)
}
#[derive(Clone)]
struct MidiNode {
    compact_tick: i64,
    raw_tick: i64,
    order: usize,
    event: Value,
}
fn reconstruct_midi_stream(
    events: &[MidiEvent],
    meter: &[(i64, i64)],
) -> Result<Vec<Value>, String> {
    let mut nodes = Vec::<MidiNode>::new();
    let mut grid = Vec::<usize>::new();
    let mut pending = HashMap::<u8, Vec<usize>>::new();
    let mut depth = 0i32;
    let mut bracket_start = 0i64;
    let mut removed = 0i64;
    let mut sorted = events.to_vec();
    sorted.sort_by_key(|event| (event.tick, event.order));
    for event in sorted {
        match &event.body {
            EventBody::Meta { meta: 0x2f, .. } | EventBody::Meta { meta: 0x51, .. } => continue,
            EventBody::Meta { meta: 0x07, data } => {
                let text =
                    std::str::from_utf8(data).map_err(|_| "MIDI cue is not UTF-8".to_string())?;
                match text {
                    "pattern+" => {
                        if depth == 0 {
                            bracket_start = event.tick;
                        }
                        depth += 1;
                    }
                    "pattern-" => {
                        depth -= 1;
                        if depth < 0 {
                            return Err("MIDI pattern bracket underflow".to_string());
                        }
                        if depth == 0 {
                            removed += event.tick - bracket_start;
                        }
                    }
                    _ => {}
                }
            }
            EventBody::Meta { meta: 0x06, data } => {
                if depth > 0 {
                    continue;
                }
                let value = serde_json::from_slice::<Value>(data)
                    .map_err(|e| format!("MIDI event marker: {e}"))?;
                let index = nodes.len();
                nodes.push(MidiNode {
                    compact_tick: event.tick - removed,
                    raw_tick: event.tick,
                    order: event.order,
                    event: value,
                });
                grid.push(index);
            }
            EventBody::Channel { status, data }
                if status & 0xf0 == 0x90 || status & 0xf0 == 0x80 =>
            {
                if data.len() < 2 {
                    return Err("MIDI note event is truncated".to_string());
                }
                let key = data[0];
                let note_on = status & 0xf0 == 0x90 && data[1] != 0;
                if note_on {
                    let index = nodes.len();
                    nodes.push(MidiNode {
                        compact_tick: event.tick - removed,
                        raw_tick: event.tick,
                        order: event.order,
                        event: serde_json::json!(["note", 0, key, data[1]]),
                    });
                    if depth == 0 {
                        grid.push(index);
                    }
                    pending.entry(key).or_default().push(index);
                } else {
                    let queue = pending
                        .get_mut(&key)
                        .ok_or("MIDI note-off has no note-on")?;
                    let index = queue
                        .first()
                        .copied()
                        .ok_or("MIDI note-off has no note-on")?;
                    queue.remove(0);
                    nodes[index].event[1] =
                        Value::from((event.tick - nodes[index].raw_tick) as usize);
                }
            }
            _ => {}
        }
    }
    if pending.values().any(|queue| !queue.is_empty()) {
        return Err("MIDI has an unclosed note-on".to_string());
    }
    if depth != 0 {
        return Err("MIDI pattern bracket is not closed".to_string());
    }
    grid.sort_by_key(|index| (nodes[*index].compact_tick, nodes[*index].order));
    let mut output = Vec::new();
    let mut cursor = 0i64;
    for index in grid {
        let node = &nodes[index];
        if node.compact_tick < cursor {
            return Err("MIDI event order moves backwards".to_string());
        }
        // The rest ends at this event in played time; pattern calls before it
        // have already advanced the bar position.
        let gap = node.compact_tick - cursor;
        for wait in rest_waits(node.raw_tick - gap, gap, meter)? {
            output.push(serde_json::json!(["wait", wait]));
        }
        cursor = node.compact_tick;
        output.push(node.event.clone());
    }
    Ok(output)
}
/// The converter's encoding of a reconstructed stream. A note, or a control
/// that sets running status, continues the running command only as the first
/// event after a rest; chords and events after other commands repeat it. An
/// end of tie that names a key continues a running end of tie. Key and
/// velocity are written when they change. A label, pattern call, repeat or
/// jump forgets the running command, key and velocity, because another path
/// reaches the event after it; a pattern end changes nothing.
fn default_sequence(events: &[Value]) -> Result<Vec<Value>, String> {
    let mut output = Vec::new();
    let mut running: Option<u8> = None;
    let (mut key, mut velocity) = (None::<usize>, None::<usize>);
    let mut after_rest = false;
    for event in events {
        let values = event.as_array().ok_or("sequence event is malformed")?;
        let kind = values
            .first()
            .and_then(Value::as_str)
            .ok_or("sequence event has no kind")?;
        match kind {
            "note" | "note_running" => {
                let duration = number(
                    values.get(1).ok_or("note duration is missing")?,
                    "note duration",
                )?;
                let note_key = number(values.get(2).ok_or("note key is missing")?, "note key")?;
                let note_velocity = number(
                    values.get(3).ok_or("note velocity is missing")?,
                    "note velocity",
                )?;
                let opcode = SEQUENCE_DURATIONS
                    .iter()
                    .position(|candidate| *candidate == duration)
                    .map(|index| 0xcf + index as u8);
                let params = if velocity != Some(note_velocity) {
                    vec![note_key, note_velocity]
                } else if key != Some(note_key) {
                    vec![note_key]
                } else {
                    Vec::new()
                };
                let continues =
                    after_rest && opcode.is_some() && running == opcode && !params.is_empty();
                let mut rebuilt = vec![
                    Value::from(if continues { "note_running" } else { "note" }),
                    Value::from(duration),
                ];
                rebuilt.extend(params.into_iter().map(Value::from));
                output.push(Value::Array(rebuilt));
                running = opcode;
                key = Some(note_key);
                velocity = Some(note_velocity);
            }
            "wait" => output.push(event.clone()),
            "label" | "pattern" | "repeat" | "goto" => {
                output.push(event.clone());
                running = None;
                key = None;
                velocity = None;
            }
            "pattern_end" => {
                output.push(event.clone());
                continue;
            }
            "control_running" | "note_end_running" => {
                return Err(format!(
                    "{kind} spells running status the converter derives"
                ));
            }
            "note_end" => {
                let continues = running == Some(0xce) && values.len() == 2;
                output.push(if continues {
                    serde_json::json!(["note_end_running", values[1]])
                } else {
                    event.clone()
                });
                running = Some(0xce);
            }
            _ => match sequence_control_opcode(kind)
                .filter(|opcode| sequence_sets_running_status(*opcode))
            {
                Some(opcode) => {
                    let value = values
                        .get(1)
                        .ok_or_else(|| format!("{kind} value is missing"))?;
                    let continues = after_rest
                        && running == Some(opcode)
                        && values.len() == 2
                        && sequence_parameter(value, kind)? < 0x80;
                    output.push(if continues {
                        serde_json::json!(["control_running", kind, value])
                    } else {
                        event.clone()
                    });
                    running = Some(opcode);
                }
                None => output.push(event.clone()),
            },
        }
        after_rest = kind == "wait";
    }
    Ok(output)
}
fn native_sequence_self_test() -> Result<(), String> {
    let events = serde_json::json!([
        ["priority", 5],
        ["control_running", "priority", 6],
        ["key_shift", 0],
        ["control_running", "key_shift", 1],
        ["tempo", 30]
    ]);
    let encoded = encode_sequence_stream(
        events
            .as_array()
            .ok_or("sequence self-test events are malformed")?,
    )?;
    if encoded.data != [0xba, 5, 6, 0xbc, 0, 1, 0xbb, 30] {
        return Err("sequence running-status self-test differs".to_string());
    }
    let invalid = serde_json::json!([["tempo", 30], ["control_running", "tempo", 31]]);
    if encode_sequence_stream(
        invalid
            .as_array()
            .ok_or("sequence self-test invalid events are malformed")?,
    )
    .is_ok()
    {
        return Err("tempo incorrectly accepts running status".to_string());
    }
    Ok(())
}
fn midi_variable(mut value: usize) -> Vec<u8> {
    let mut bytes = vec![(value & 0x7f) as u8];
    while {
        value >>= 7;
        value != 0
    } {
        bytes.push(((value & 0x7f) as u8) | 0x80);
    }
    bytes.reverse();
    bytes
}
fn encode_midi_track(events: &[MidiEvent]) -> Result<Vec<u8>, String> {
    let mut output = Vec::new();
    let mut tick = 0i64;
    for event in events {
        if matches!(event.body, EventBody::Meta { meta: 0x2f, .. }) {
            continue;
        }
        let delta = event
            .tick
            .checked_sub(tick)
            .filter(|delta| *delta >= 0)
            .ok_or("MIDI event order moves backwards")? as usize;
        output.extend(midi_variable(delta));
        tick = event.tick;
        match &event.body {
            EventBody::Meta { meta, data } => {
                output.extend([0xff, *meta]);
                output.extend(midi_variable(data.len()));
                output.extend(data);
            }
            EventBody::Sysex { status, data } => {
                output.push(*status);
                output.extend(midi_variable(data.len()));
                output.extend(data);
            }
            EventBody::Channel { status, data } => {
                output.push(*status);
                output.extend(data);
            }
        }
    }
    output.extend([0, 0xff, 0x2f, 0]);
    Ok(output)
}
fn repack_midi_tracks(midi: &[u8], native_tracks: usize) -> Result<Vec<u8>, String> {
    let report = midi_events(midi).map_err(|error| error.to_string())?;
    if report.format == 1 && usize::from(report.tracks) == native_tracks + 1 {
        return Ok(midi.to_vec());
    }
    if report.format != 0 || report.tracks != 1 {
        return Err("playback MIDI is neither canonical format 1 nor convertible format 0".into());
    }
    let mut tracks = vec![Vec::<MidiEvent>::new(); native_tracks + 1];
    for event in report.events {
        let destination = match &event.body {
            EventBody::Channel { status, .. } => usize::from(status & 0x0f) + 1,
            _ => 0,
        };
        if destination >= tracks.len() {
            return Err(format!(
                "playback MIDI channel {} exceeds native track count",
                destination
            ));
        }
        tracks[destination].push(event);
    }
    let count = u16::try_from(tracks.len()).map_err(|_| "too many MIDI tracks")?;
    let mut output = Vec::new();
    output.extend_from_slice(b"MThd");
    output.extend_from_slice(&6u32.to_be_bytes());
    output.extend_from_slice(&1u16.to_be_bytes());
    output.extend_from_slice(&count.to_be_bytes());
    output.extend_from_slice(&report.ticks_per_quarter.to_be_bytes());
    for (track_index, mut events) in tracks.into_iter().enumerate() {
        if track_index != 0 {
            let mut active = HashMap::<u8, usize>::new();
            let mut last_tick = 0i64;
            for event in &events {
                last_tick = last_tick.max(event.tick);
                if let EventBody::Channel { status, data } = &event.body {
                    let note = data.first().copied().unwrap_or(0);
                    if status & 0xf0 == 0x90 && data.get(1).copied().unwrap_or(0) != 0 {
                        *active.entry(note).or_default() += 1;
                    } else if status & 0xf0 == 0x80
                        || (status & 0xf0 == 0x90 && data.get(1) == Some(&0))
                    {
                        if let Some(count) = active.get_mut(&note).filter(|count| **count != 0) {
                            *count -= 1;
                        }
                    }
                }
            }
            let channel = u8::try_from(track_index - 1).map_err(|_| "too many MIDI channels")?;
            for (note, count) in active {
                for order in 0..count {
                    events.push(MidiEvent {
                        tick: last_tick,
                        track: track_index,
                        order: usize::MAX - order,
                        body: EventBody::Channel {
                            status: 0x80 | channel,
                            data: vec![note, 0],
                        },
                    });
                }
            }
        }
        events.sort_by_key(|event| (event.tick, event.order));
        let data = encode_midi_track(&events)?;
        output.extend_from_slice(b"MTrk");
        output.extend_from_slice(
            &u32::try_from(data.len())
                .map_err(|_| "MIDI track is too large")?
                .to_be_bytes(),
        );
        output.extend(data);
    }
    Ok(output)
}
/// A sequence MIDI's events by track, its stream skeleton and its bar lines.
struct SequenceMidi {
    tracks: HashMap<usize, Vec<MidiEvent>>,
    skeleton: Option<Value>,
    meter: Vec<(i64, i64)>,
}
/// A MIDI file's events by track and its ticks per quarter note.
fn midi_tracks(midi: &[u8]) -> Result<(HashMap<usize, Vec<MidiEvent>>, u16), String> {
    let report = midi_events(midi).map_err(|error| error.to_string())?;
    let mut tracks = HashMap::<usize, Vec<MidiEvent>>::new();
    for event in report.events {
        tracks.entry(event.track).or_default().push(event);
    }
    Ok((tracks, report.ticks_per_quarter))
}
fn read_sequence_midi(midi: &[u8]) -> Result<SequenceMidi, String> {
    let (tracks, division) = midi_tracks(midi)?;
    let conductor = tracks.get(&0).map(Vec::as_slice).unwrap_or(&[]);
    let mut skeleton = None;
    for event in conductor {
        match &event.body {
            EventBody::Meta { meta: 0x7f, data } if data.starts_with(MIDI_BUILD_DIRECTIVE) => {
                return Err("MIDI carries a retired sequence sidecar directive".to_string())
            }
            EventBody::Meta { meta: 0x01, data } if skeleton.is_none() => {
                skeleton = Some(
                    serde_json::from_slice::<Value>(data)
                        .map_err(|e| format!("MIDI conductor skeleton: {e}"))?,
                );
            }
            _ => {}
        }
    }
    let meter = sequence_meter(conductor, division)?;
    Ok(SequenceMidi {
        tracks,
        skeleton,
        meter,
    })
}
/// The converter's reading of one MIDI stream track under the given bar lines.
fn read_midi_stream(
    midi: &SequenceMidi,
    track: usize,
    meter: &[(i64, i64)],
) -> Result<Vec<Value>, String> {
    default_sequence(&reconstruct_midi_stream(
        midi.tracks.get(&track).map(Vec::as_slice).unwrap_or(&[]),
        meter,
    )?)
}
fn build_midi_sequence(_root: &Path, source: &Path) -> Result<(Vec<u8>, Value), String> {
    let midi =
        read_sequence_midi(&fs::read(source).map_err(|e| format!("{}: {e}", source.display()))?)?;
    let skeleton = midi
        .skeleton
        .as_ref()
        .ok_or("MIDI conductor skeleton is missing")?;
    let skeleton_layout = skeleton
        .get("layout")
        .and_then(Value::as_array)
        .ok_or("MIDI skeleton layout is missing")?;
    let mut stream_index = 0usize;
    let mut layout = Vec::new();
    for segment in skeleton_layout {
        if segment.get("kind").and_then(Value::as_str) != Some("stream") {
            layout.push(segment.clone());
            continue;
        }
        stream_index += 1;
        let label = json_string(&segment["label"], "MIDI stream label")?.to_string();
        layout.push(serde_json::json!({
            "kind": "stream",
            "label": label,
            "events": read_midi_stream(&midi, stream_index, &midi.meter)?
        }));
    }
    let source = serde_json::json!({
        "format": skeleton["format"],
        "engine": skeleton["engine"],
        "base": skeleton["base"],
        "externals": skeleton["externals"],
        "layout": layout
    });
    build_sequence_source(&source)
}
/// Adopt a playback MIDI as the source of a native sequence. Its conductor
/// becomes the sequence skeleton and the first time signature, in
/// `ADOPTION_BARS` order, under which the converter reads every native stream
/// back exactly; without one, adoption refuses and names the first difference.
fn adopt_smsh_midi(source: &Value, midi: &[u8]) -> Result<Vec<u8>, String> {
    let source_layout = source
        .get("layout")
        .and_then(Value::as_array)
        .ok_or("sequence source layout is missing")?;
    let mut streams = Vec::new();
    let mut skeleton_layout = Vec::new();
    for segment in source_layout {
        if segment.get("kind").and_then(Value::as_str) != Some("stream") {
            skeleton_layout.push(segment.clone());
            continue;
        }
        let label = json_string(&segment["label"], "stream label")?;
        let native = segment
            .get("events")
            .and_then(Value::as_array)
            .ok_or("sequence stream events missing")?;
        streams.push((label, native));
        skeleton_layout.push(serde_json::json!({"kind":"stream", "label":label}));
    }
    let midi = repack_midi_tracks(midi, streams.len())?;
    // The conductor is replaced, so nothing it carried is read.
    let (tracks, division) = midi_tracks(&midi)?;
    if tracks.keys().copied().max().unwrap_or(0) != streams.len() {
        return Err("MIDI and native sequence track counts differ".to_string());
    }
    let playback = SequenceMidi {
        tracks,
        skeleton: None,
        meter: Vec::new(),
    };
    let mut first_difference = None;
    let mut adopted = None;
    'bars: for bar in ADOPTION_BARS {
        for (index, (label, native)) in streams.iter().enumerate() {
            let read = read_midi_stream(&playback, index + 1, &[(0, bar)])?;
            if read != **native {
                if first_difference.is_none() {
                    let at = read
                        .iter()
                        .zip(native.iter())
                        .take_while(|(a, b)| a == b)
                        .count();
                    first_difference = Some(format!(
                        "{label} event {at}: native {} but MIDI reads {}",
                        native
                            .get(at)
                            .map_or("nothing".to_string(), Value::to_string),
                        read.get(at).map_or("nothing".to_string(), Value::to_string)
                    ));
                }
                continue 'bars;
            }
        }
        adopted = Some(bar);
        break;
    }
    let bar = adopted.ok_or_else(|| {
        format!(
            "no time signature reproduces the native sequence; {}",
            first_difference.unwrap_or_default()
        )
    })?;
    let mut skeleton = source.clone();
    skeleton["layout"] = Value::Array(skeleton_layout);
    let conductor = encode_midi_track(&[
        MidiEvent {
            tick: 0,
            track: 0,
            order: 0,
            body: EventBody::Meta {
                meta: 0x01,
                data: serde_json::to_vec(&skeleton).map_err(|error| error.to_string())?,
            },
        },
        MidiEvent {
            tick: 0,
            track: 0,
            order: 1,
            body: EventBody::Meta {
                meta: 0x58,
                data: time_signature(bar, division)?.to_vec(),
            },
        },
    ])?;
    let header = 8 + u32::from_be_bytes(midi[4..8].try_into().map_err(|_| "MIDI header")?) as usize;
    let old_conductor = midi
        .get(header + 4..header + 8)
        .map(|size| u32::from_be_bytes(size.try_into().unwrap()) as usize)
        .ok_or("MIDI conductor track is missing")?;
    let mut output = midi[..header].to_vec();
    output.extend_from_slice(b"MTrk");
    output.extend_from_slice(
        &u32::try_from(conductor.len())
            .map_err(|_| "MIDI conductor is too large")?
            .to_be_bytes(),
    );
    output.extend(conductor);
    output.extend_from_slice(
        midi.get(header + 8 + old_conductor..)
            .ok_or("MIDI conductor track is truncated")?,
    );
    Ok(output)
}
#[test]
fn sequence_midi_reading_follows_bar_lines_time_slots_and_jump_targets() {
    let marker = |tick, order, event: Value| MidiEvent {
        tick,
        track: 1,
        order,
        body: EventBody::Meta {
            meta: 0x06,
            data: serde_json::to_vec(&event).unwrap(),
        },
    };
    let note = |tick, order, status: u8, key| MidiEvent {
        tick,
        track: 1,
        order,
        body: EventBody::Channel {
            status,
            data: vec![key, 100],
        },
    };
    let events = [
        note(0, 0, 0x90, 60),
        note(0, 1, 0x90, 64),
        note(5, 2, 0x80, 60),
        note(5, 3, 0x80, 64),
        note(12, 4, 0x90, 67),
        note(17, 5, 0x80, 67),
        marker(24, 6, serde_json::json!(["label", "loop"])),
        note(24, 7, 0x90, 67),
        note(29, 8, 0x80, 67),
        marker(200, 9, serde_json::json!(["goto", "loop"])),
    ];
    let read = |bar| {
        Value::Array(
            default_sequence(&reconstruct_midi_stream(&events, &[(0, bar)]).unwrap()).unwrap(),
        )
    };
    let opening = serde_json::json!([
        ["note", 5, 60, 100],
        ["note", 5, 64],
        ["wait", 12],
        ["note_running", 5, 67],
        ["wait", 12],
        ["label", "loop"],
        ["note", 5, 67, 100]
    ]);
    let with_rest = |waits: &[usize]| {
        let mut expected = opening.as_array().unwrap().clone();
        expected.extend(waits.iter().map(|wait| serde_json::json!(["wait", wait])));
        expected.push(serde_json::json!(["goto", "loop"]));
        Value::Array(expected)
    };
    assert_eq!(read(96), with_rest(&[72, 96, 8]));
    assert_eq!(read(72), with_rest(&[48, 72, 56]));
    let signature = |numerator: u8, power: u8| MidiEvent {
        tick: 0,
        track: 0,
        order: 0,
        body: EventBody::Meta {
            meta: 0x58,
            data: vec![numerator, power, 24, 8],
        },
    };
    assert_eq!(sequence_meter(&[signature(3, 4)], 96).unwrap(), [(0, 72)]);
    assert_eq!(sequence_meter(&[], 96).unwrap(), [(0, 384)]);
    assert!(sequence_meter(&[signature(1, 5)], 100).is_err());
    assert_eq!(time_signature(96, 96).unwrap(), [4, 4, 24, 8]);
    assert_eq!(time_signature(84, 96).unwrap(), [7, 5, 24, 8]);
    assert_eq!(time_signature(12, 96).unwrap(), [1, 5, 24, 8]);
}
#[test]
fn sequence_midi_reading_derives_control_running_status() {
    let marker = |tick, order, event: Value| MidiEvent {
        tick,
        track: 1,
        order,
        body: EventBody::Meta {
            meta: 0x06,
            data: serde_json::to_vec(&event).unwrap(),
        },
    };
    let events = [
        marker(0, 0, serde_json::json!(["volume", 80])),
        marker(12, 1, serde_json::json!(["volume", 90])),
        marker(12, 2, serde_json::json!(["pan", 64])),
        marker(12, 3, serde_json::json!(["pan", 60])),
        marker(24, 4, serde_json::json!(["note_end", 60])),
        marker(24, 5, serde_json::json!(["note_end", 64])),
        marker(36, 6, serde_json::json!(["note_end", 62])),
        marker(36, 7, serde_json::json!(["note_end"])),
        marker(48, 8, serde_json::json!(["fine"])),
    ];
    let read = default_sequence(&reconstruct_midi_stream(&events, &[(0, 96)]).unwrap()).unwrap();
    assert_eq!(
        Value::Array(read),
        serde_json::json!([
            ["volume", 80],
            ["wait", 12],
            ["control_running", "volume", 90],
            ["pan", 64],
            ["pan", 60],
            ["wait", 12],
            ["note_end", 60],
            ["note_end_running", 64],
            ["wait", 12],
            ["note_end_running", 62],
            ["note_end"],
            ["wait", 12],
            ["fine"]
        ])
    );
    let spelled = [marker(
        0,
        0,
        serde_json::json!(["control_running", "volume", 1]),
    )];
    assert!(
        default_sequence(&reconstruct_midi_stream(&spelled, &[(0, 96)]).unwrap())
            .unwrap_err()
            .contains("derives")
    );
}
#[test]
fn midi_adoption_records_the_meter_and_refuses_unreproducible_streams() {
    let track = |events: &[MidiEvent]| {
        let data = encode_midi_track(events).unwrap();
        [
            b"MTrk".as_slice(),
            &(data.len() as u32).to_be_bytes(),
            &data,
        ]
        .concat()
    };
    let at = |tick, order, body| MidiEvent {
        tick,
        track: 1,
        order,
        body,
    };
    let playback = [
        b"MThd\0\0\0\x06\0\x01\0\x02\0\x60".as_slice(),
        &track(&[]),
        &track(&[
            at(
                0,
                0,
                EventBody::Channel {
                    status: 0x90,
                    data: vec![60, 100],
                },
            ),
            at(
                5,
                1,
                EventBody::Channel {
                    status: 0x80,
                    data: vec![60, 64],
                },
            ),
            at(
                96,
                2,
                EventBody::Meta {
                    meta: 0x06,
                    data: br#"["fine"]"#.to_vec(),
                },
            ),
        ]),
    ]
    .concat();
    let source = |waits: Value| {
        let mut events = vec![serde_json::json!(["note", 5, 60, 100])];
        events.extend(
            waits
                .as_array()
                .unwrap()
                .iter()
                .map(|wait| serde_json::json!(["wait", wait])),
        );
        events.push(serde_json::json!(["fine"]));
        serde_json::json!({
            "format": 1, "engine": "smsh-sequence", "base": "0x08000000", "externals": {},
            "layout": [{"kind": "stream", "label": "track_1", "events": events}]
        })
    };
    let adopted = read_sequence_midi(
        &adopt_smsh_midi(&source(serde_json::json!([72, 24])), &playback).unwrap(),
    )
    .unwrap();
    assert_eq!(adopted.meter, [(0, 72)]);
    assert_eq!(
        adopted.skeleton.unwrap()["layout"],
        serde_json::json!([{"kind": "stream", "label": "track_1"}])
    );
    let refusal = adopt_smsh_midi(&source(serde_json::json!([50, 46])), &playback).unwrap_err();
    assert!(refusal.contains("no time signature"), "{refusal}");
    let directive = [MIDI_BUILD_DIRECTIVE, b"{}"].concat();
    let retired = [
        b"MThd\0\0\0\x06\0\x01\0\x01\0\x60".as_slice(),
        &track(&[MidiEvent {
            tick: 0,
            track: 0,
            order: 0,
            body: EventBody::Meta {
                meta: 0x7f,
                data: directive,
            },
        }]),
    ]
    .concat();
    assert!(read_sequence_midi(&retired).is_err());
}

fn build_pcm_record(entry: &Value, wav: &[u8]) -> Result<(Vec<u8>, Value), String> {
    let word = |v: &Value, label: &str| {
        u32::try_from(number(v, label)?).map_err(|_| format!("{label} exceeds u32"))
    };
    let header = entry.get("header");
    let frequency = word(&entry["frequency"], "wave frequency")?;
    let (rate, samples) = psynergy::assets::wav::wav_pcm8(wav).map_err(|e| e.to_string())?;
    if samples.is_empty() || u64::from(rate) != (u64::from(frequency) + 512) / 1024 {
        return Err("WAV sample count or rate differs from catalog".into());
    }
    let catalog_loop = entry
        .get("loop_start")
        .filter(|v| !v.is_null())
        .map(|v| word(v, "wave loop start"))
        .transpose()?;
    let (control, loop_start) = if let Some(header) = header {
        if word(&header["frequency"], "header frequency")? != frequency
            || number(&header["sample_count"], "sample count")? != samples.len()
        {
            return Err("wave header differs from catalog or WAV data".into());
        }
        (
            word(&header["control"], "wave control")?,
            word(&header["loop_start"], "wave loop start")?,
        )
    } else {
        (
            if catalog_loop.is_some() {
                0x40000000
            } else {
                0
            },
            catalog_loop.unwrap_or(0),
        )
    };
    let looped = control & 0xc0000000 != 0;
    if (header.is_some() && catalog_loop != looped.then_some(loop_start))
        || (looped && loop_start as usize >= samples.len())
    {
        return Err("wave loop differs or starts beyond samples".into());
    }
    let size = number(&entry["size"], "wave size")?;
    let padding = size
        .checked_sub(16)
        .and_then(|n| n.checked_sub(samples.len()))
        .ok_or("wave record is shorter than samples")?;
    let fill = if header.is_some() {
        if number(&entry["padding"]["size"], "padding size")? != padding {
            return Err("wave padding size differs".into());
        }
        u8::try_from(number(&entry["padding"]["fill"], "padding fill")?)
            .map_err(|_| "padding exceeds u8")?
    } else {
        if padding > 3 {
            return Err("wave alignment exceeds three bytes".into());
        }
        0
    };
    let last_sample =
        u32::try_from(samples.len() - 1).map_err(|_| "wave sample count exceeds u32")?;
    let mut bytes = Vec::with_capacity(size);
    for value in [control, frequency, loop_start, last_sample] {
        bytes.extend(value.to_le_bytes());
    }
    bytes.extend(&samples);
    bytes.resize(size, fill);
    Ok((
        bytes,
        serde_json::json!({"samples":samples.len(),"rate":rate,"frequency":frequency,"control":control,
        "looped":looped,"loop_start":looped.then_some(loop_start),"padding_bytes":padding,"padding_fill":fill}),
    ))
}

#[test]
fn pcm_records_preserve_exact_headers_loops_and_padding() {
    let mut wav=Vec::from(&b"RIFF\x27\0\0\0WAVEfmt \x10\0\0\0\x01\0\x01\0\x40\x1f\0\0\x40\x1f\0\0\x01\0\x08\0data\x03\0\0\0"[..]);
    wav.extend([0, 128, 255]);
    let source = serde_json::json!({"frequency":8192511,"loop_start":1,"size":20});
    let (built, report) = build_pcm_record(&source, &wav).unwrap();
    assert_eq!(&built[16..], &[128, 0, 127, 0]);
    assert_eq!(u32::from_le_bytes(built[12..16].try_into().unwrap()), 2);
    assert_eq!(report["control"], 0x40000000);
    let mut exact = source.clone();
    exact["header"] = serde_json::json!({"frequency":8192511,"control":0x80000000_u32,"loop_start":1,"sample_count":3});
    exact["padding"] = serde_json::json!({"size":1,"fill":165});
    let (built, report) = build_pcm_record(&exact, &wav).unwrap();
    assert_eq!(built[19], 165);
    assert_eq!(report["control"], 0x80000000_u32);
    for (pointer, value) in [
        ("/header/frequency", serde_json::json!(8192512)),
        ("/header/sample_count", serde_json::json!(2)),
        ("/header/loop_start", serde_json::json!(3)),
        ("/header/control", serde_json::json!(0)),
        ("/padding/size", serde_json::json!(2)),
        ("/padding/fill", serde_json::json!(256)),
        ("/size", serde_json::json!(18)),
    ] {
        let mut bad = exact.clone();
        *bad.pointer_mut(pointer).unwrap() = value;
        assert!(build_pcm_record(&bad, &wav).is_err(), "{pointer}");
    }
    let mut bad = source;
    bad["size"] = serde_json::json!(23);
    assert!(build_pcm_record(&bad, &wav).is_err());
}

/// Message markup is printable ASCII text and `{"command": name}` atoms from
/// the source's command table, each with an `argument` when the table says so;
/// `null` is an empty message. Text may also use the characters the source's
/// optional glyph table declares, each standing for its symbol sequence.
fn message_symbols(
    message: &Value,
    commands: &serde_json::Map<String, Value>,
    glyphs: Option<&serde_json::Map<String, Value>>,
    symbol_count: usize,
) -> Result<Option<Vec<u16>>, String> {
    let atoms = match message {
        Value::Null => return Ok(None),
        Value::String(_) => std::slice::from_ref(message),
        Value::Array(atoms) => atoms.as_slice(),
        _ => return Err("message must be text, atoms, or null".into()),
    };
    let symbol = |value: usize| {
        u16::try_from(value)
            .ok()
            .filter(|_| value < symbol_count)
            .ok_or("message symbol is outside the alphabet")
    };
    let mut symbols = Vec::new();
    for atom in atoms {
        if let Some(text) = atom.as_str() {
            for character in text.chars() {
                if character.is_ascii_graphic() || character == ' ' {
                    symbols.push(symbol(character as usize)?);
                    continue;
                }
                let glyph = (!character.is_ascii())
                    .then(|| glyphs?.get(character.encode_utf8(&mut [0; 4]) as &str))
                    .flatten()
                    .and_then(Value::as_array)
                    .filter(|sequence| !sequence.is_empty())
                    .ok_or("message text must be printable ASCII or a declared glyph")?;
                for value in glyph {
                    symbols.push(symbol(number(value, "glyph symbol")?)?);
                }
            }
            continue;
        }
        let atom = atom
            .as_object()
            .ok_or("message atom must be text or a command")?;
        let name = json_string(&atom["command"], "message command")?;
        let command = commands
            .get(name)
            .ok_or_else(|| format!("unknown message command {name}"))?;
        let opcode = number(&command["opcode"], "command opcode")?;
        if !(1..32).contains(&opcode) {
            return Err(format!(
                "message command {name} opcode is not a control code"
            ));
        }
        symbols.push(symbol(opcode)?);
        let takes_argument = command["argument"] == true;
        if atom.len() != 1 + usize::from(takes_argument)
            || takes_argument != atom.contains_key("argument")
        {
            return Err(format!("message command {name} argument differs"));
        }
        if takes_argument {
            symbols.push(symbol(number(&atom["argument"], "command argument")?)?);
        }
    }
    Ok(Some(symbols))
}

#[test]
fn message_markup_follows_the_declared_command_table() {
    let commands = serde_json::json!({"end":{"opcode":2},"color":{"opcode":8,"argument":true}});
    let commands = commands.as_object().unwrap();
    let glyphs = serde_json::json!({"\u{2014}":[176],"\u{30af}":[131,78],"A":[1],"\u{e9}":[]});
    let glyphs = Some(glyphs.as_object().unwrap());
    assert_eq!(
        message_symbols(&Value::Null, commands, None, 123).unwrap(),
        None
    );
    assert_eq!(
        message_symbols(&serde_json::json!("Hi"), commands, None, 123).unwrap(),
        Some(vec![72, 105])
    );
    assert_eq!(
        message_symbols(
            &serde_json::json!([{"command":"color","argument":5},"z",{"command":"end"}]),
            commands,
            None,
            123
        )
        .unwrap(),
        Some(vec![8, 5, 122, 2])
    );
    assert_eq!(
        message_symbols(
            &serde_json::json!("A\u{2014}\u{30af}"),
            commands,
            glyphs,
            242
        )
        .unwrap(),
        Some(vec![65, 176, 131, 78])
    );
    assert!(message_symbols(&serde_json::json!("\u{2014}"), commands, glyphs, 123).is_err());
    assert!(message_symbols(&serde_json::json!("\u{e9}"), commands, glyphs, 242).is_err());
    assert!(message_symbols(&serde_json::json!("\u{2015}"), commands, glyphs, 242).is_err());
    for bad in [
        serde_json::json!("{"),
        serde_json::json!("\u{e9}"),
        serde_json::json!([{"command":"missing"}]),
        serde_json::json!([{"command":"end","argument":1}]),
        serde_json::json!([{"command":"color"}]),
        serde_json::json!([{"command":"color","argument":123}]),
        serde_json::json!([7]),
    ] {
        assert!(message_symbols(&bad, commands, None, 123).is_err(), "{bad}");
    }
}

fn build_entry_native_tail(
    ctx: &mut Context,
    entry: &Value,
    kind: &str,
    address: usize,
    entry_source: &str,
) -> Result<(Vec<u8>, Vec<String>, Value), String> {
    let source_path = |name: &str| ctx.source(name);
    match kind {
        "golden-sun-sound-sequence" => {
            let source = source_path(entry_source)?;
            let (built, report) = if entry_source.to_ascii_lowercase().ends_with(".json") {
                let document = json(&source)?;
                let document = if let Some(pointer) = entry.get("pointer") {
                    document
                        .pointer(json_string(pointer, "sequence pointer")?)
                        .ok_or("sequence pointer is absent")?
                } else {
                    &document
                };
                build_sequence_source(document)?
            } else {
                build_midi_sequence(&ctx.root, &source)?
            };
            if report["base"].as_u64() != Some(address as u64) {
                return Err("sound-sequence base differs from manifest".to_string());
            }
            Ok((built, vec![entry_source.to_string()], report))
        }
        "golden-sun-pcm-wave" => {
            let wav = fs::read(source_path(entry_source)?).map_err(|error| error.to_string())?;
            let (built, report) = build_pcm_record(entry, &wav)?;
            let mut sources = vec![entry_source.to_string()];
            if let Some(index) = entry.get("index").and_then(Value::as_str) {
                ctx.source(index)?;
                sources.insert(0, index.to_string());
            }
            Ok((built, sources, report))
        }
        "golden-sun-delta7-still" => {
            if entry.get("source_rect").is_some() {
                let image = ctx.indexed(&source_path(entry_source)?)?;
                let (width, height, pixels) = indexed_rect(&image, entry)?;
                if width != number(&entry["width"], "still width")?
                    || height != number(&entry["height"], "still height")?
                {
                    return Err("still atlas dimensions differ".into());
                }
                let palette = build_component_cached(ctx, &entry["palette"])?;
                if palette.data.len() != number(&entry["palette_entries"], "still colors")? * 2 {
                    return Err("still palette dimensions differ".into());
                }
                let mut built = palette.data;
                built.extend(
                    psynergy::assets::compression::encode_delta7(&pixels)
                        .map_err(|e| e.to_string())?,
                );
                let mut sources = vec![entry_source.to_string()];
                sources.extend(palette.sources);
                return Ok((
                    built,
                    sources,
                    serde_json::json!({"width":width,"height":height}),
                ));
            }
            let built = psynergy::assets::compression::delta7_image(
                &fs::read(source_path(entry_source)?).map_err(|error| error.to_string())?,
                number(&entry["width"], "delta7 width")?,
                number(&entry["height"], "delta7 height")?,
                number(&entry["palette_entries"], "delta7 palette entries")?,
            )
            .map_err(|error| error.to_string())?;
            Ok((
                built,
                vec![entry_source.to_string()],
                serde_json::json!({"width":entry["width"],"height":entry["height"],"palette_entries":entry["palette_entries"]}),
            ))
        }
        "golden-sun-message-archive" => {
            if entry_source.to_ascii_lowercase().ends_with(".po") {
                let source = crate::text_catalog::read_source(&source_path(entry_source)?)?;
                if source.address != address
                    || source.size != number(&entry["size"], "archive size")?
                {
                    return Err("message catalog identity differs".into());
                }
                let base = u32::try_from(address).map_err(|_| "archive address exceeds u32")?;
                let archive = psynergy::assets::huffman_archive::encode_huffman_archive(
                    base,
                    source.symbol_count,
                    &source.banks,
                )
                .map_err(|error| error.to_string())?;
                if archive.context_directory != source.contexts as u32
                    || archive.directory != source.directory as u32
                {
                    return Err("message catalog pointers differ from its archive headers".into());
                }
                return Ok((
                    archive.bytes,
                    vec![entry_source.to_string()],
                    serde_json::json!({"banks":source.banks.len(),"messages":source.banks.iter().map(Vec::len).sum::<usize>(),"contexts":archive.contexts}),
                ));
            }
            let document = json(&source_path(entry_source)?)?;
            if document["format"] != 2
                || document["kind"] != kind
                || number(&document["address"], "archive address")? != address
            {
                return Err("message archive identity differs".into());
            }
            let symbol_count = number(&document["symbol_count"], "message symbol count")?;
            let bank_size = number(&document["bank_size"], "message bank size")?;
            let commands = document["commands"]
                .as_object()
                .ok_or("message commands missing")?;
            let glyphs = match document.get("glyphs") {
                None => None,
                Some(glyphs) => Some(glyphs.as_object().ok_or("message glyphs must be a table")?),
            };
            let source_banks = document["banks"]
                .as_array()
                .ok_or("message banks missing")?;
            let mut banks = Vec::new();
            for (index, bank) in source_banks.iter().enumerate() {
                let messages = bank.as_array().ok_or("message bank is not an array")?;
                if messages.len() > bank_size
                    || (messages.len() < bank_size && index + 1 != source_banks.len())
                {
                    return Err("message bank size differs".into());
                }
                banks.push(
                    messages
                        .iter()
                        .map(|message| message_symbols(message, commands, glyphs, symbol_count))
                        .collect::<Result<Vec<_>, _>>()?,
                );
            }
            let base = u32::try_from(address).map_err(|_| "archive address exceeds u32")?;
            let archive = psynergy::assets::huffman_archive::encode_huffman_archive(
                base,
                symbol_count,
                &banks,
            )
            .map_err(|error| error.to_string())?;
            for (key, actual) in [
                ("offset_table_address", archive.offset_table),
                ("message_address", archive.messages),
                ("directory_address", archive.directory),
            ] {
                if number(&document[key], key)? != actual as usize {
                    return Err(format!("{key} differs from the built archive"));
                }
            }
            Ok((
                archive.bytes,
                vec![entry_source.to_string()],
                serde_json::json!({"banks":banks.len(),"messages":banks.iter().map(Vec::len).sum::<usize>(),"contexts":archive.contexts}),
            ))
        }
        "typed-table" => {
            let source = json(&source_path(entry_source)?)?;
            let document = if let Some(pointer) = entry.get("pointer") {
                source
                    .pointer(json_string(pointer, "table pointer")?)
                    .ok_or("table pointer is absent")?
            } else {
                &source
            };
            if number(&document["address"], "table address")? != address {
                return Err("table address differs from manifest".into());
            }
            let mut document = document.clone();
            resolve_stream_offsets(&mut document, &source)?;
            let has_symbols = document["segments"]
                .as_array()
                .is_some_and(|segments| segments.iter().any(|s| s["element"] == "thumb-pointer"));
            if has_symbols {
                let symbols = SourcePaths::load_for_game(&ctx.root, ctx.game.compiler.as_str())?;
                resolve_table_symbols(&mut document, &symbols)?;
            }
            let mut sources = vec![entry_source.to_string()];
            sources.extend(resolve_table_bitmaps(&mut document, &ctx.root)?);
            let built = typed_table(&document)?;
            Ok((
                built,
                sources,
                serde_json::json!({"segments":document["segments"].as_array().map_or(0,Vec::len)}),
            ))
        }
        _ => Err(format!("unsupported asset kind: {kind}")),
    }
}
struct BuildOptions {
    target: crate::targets::DecompTargetId,
    rom: String,
    /// The target's cartridge size; a source-only build has no ROM to measure.
    rom_size: usize,
    manifest: PathBuf,
    output: PathBuf,
    source_only: bool,
}
fn parse_build_options(arguments: &[String], root: &Path) -> Result<BuildOptions, String> {
    let target = crate::targets::decomp_target(option_value(arguments, "--target")?.as_deref())?;
    let mut options = BuildOptions {
        target: target.id,
        rom: target.rom.to_string(),
        rom_size: target.rom_size as usize,
        manifest: root.join(target.asset_manifest),
        output: root.join(target.output_dir).join("assets"),
        source_only: false,
    };
    let mut positional = false;
    let cwd = env::current_dir().map_err(|error| error.to_string())?;
    let mut index = 0;
    while index < arguments.len() {
        let argument = &arguments[index];
        if argument == "--source-only" {
            options.source_only = true;
        } else {
            let (option, inline) = argument
                .split_once('=')
                .map_or((argument.as_str(), None), |(name, value)| {
                    (name, Some(value))
                });
            match option {
                "--target" => {
                    if inline.is_none() {
                        index += 1;
                    }
                }
                "--manifest" | "-o" | "--output" => {
                    let value = match inline {
                        Some(value) => value.to_string(),
                        None => {
                            index += 1;
                            arguments
                                .get(index)
                                .ok_or_else(|| format!("{option} requires a value"))?
                                .clone()
                        }
                    };
                    let path = PathBuf::from(value);
                    if option == "--manifest" {
                        options.manifest = if path.is_absolute() {
                            path
                        } else {
                            cwd.join(path)
                        };
                    } else {
                        options.output = if path.is_absolute() {
                            path
                        } else {
                            root.join(path)
                        };
                    }
                }
                _ if !argument.starts_with('-') && !positional => {
                    options.rom = argument.clone();
                    positional = true;
                }
                _ => return Err(format!("unrecognized argument: {argument}")),
            }
        }
        index += 1;
    }
    if options.source_only && positional {
        return Err("--source-only does not accept a ROM".to_string());
    }
    Ok(options)
}
/// The value of `--name VALUE` or `--name=VALUE`, if present once.
fn option_value(arguments: &[String], name: &str) -> Result<Option<String>, String> {
    let mut found = None;
    let mut index = 0;
    while index < arguments.len() {
        let argument = &arguments[index];
        let value = if argument == name {
            index += 1;
            Some(
                arguments
                    .get(index)
                    .ok_or_else(|| format!("{name} requires a value"))?
                    .clone(),
            )
        } else {
            argument
                .strip_prefix(name)
                .and_then(|rest| rest.strip_prefix('='))
                .map(str::to_string)
        };
        if let Some(value) = value {
            if found.replace(value).is_some() {
                return Err(format!("{name} was given more than once"));
            }
        }
        index += 1;
    }
    Ok(found)
}
#[test]
fn build_options_follow_the_target_and_explicit_paths_override_it() {
    let root = Path::new("/repo");
    let arguments = |items: &[&str]| items.iter().map(|s| s.to_string()).collect::<Vec<_>>();
    let tla = parse_build_options(&arguments(&["--target", "tla-en"]), root).unwrap();
    assert_eq!(tla.rom, "roms/tla-en.gba");
    assert_eq!(tla.manifest, root.join("recon/tla/assets.json"));
    assert_eq!(tla.output, root.join("out/tla-en/assets"));
    let tbs = parse_build_options(&arguments(&["-o", "out/x", "roms/a.gba"]), root).unwrap();
    assert_eq!(tbs.rom, "roms/a.gba");
    assert_eq!(tbs.manifest, root.join("recon/tbs/assets.json"));
    assert_eq!(tbs.output, root.join("out/x"));
    assert!(parse_build_options(&arguments(&["--target=tla"]), root).is_err());
    assert!(
        parse_build_options(&arguments(&["--target", "tla-en", "--target=tbs-en"]), root).is_err()
    );
}
fn stamp_files(
    root: &Path,
    directory: &Path,
    files: &mut BTreeMap<String, PathBuf>,
) -> Result<(), String> {
    if !directory.exists() {
        return Ok(());
    }
    let entries = walkdir::WalkDir::new(directory)
        .follow_links(true)
        .into_iter()
        .filter_entry(|entry| {
            entry.depth() == 0
                || !entry.file_type().is_dir()
                || !matches!(
                    entry.file_name().to_str(),
                    Some("target" | "out" | "scratch")
                )
        });
    for entry in entries {
        let entry = entry.map_err(|error| error.to_string())?;
        if entry.file_type().is_file() {
            files
                .entry(relative(root, entry.path()))
                .or_insert(entry.into_path());
        }
    }
    Ok(())
}
fn is_c_source(path: &Path) -> bool {
    path.extension()
        .and_then(|extension| extension.to_str())
        .is_some_and(|extension| {
            extension.eq_ignore_ascii_case("c") || extension.eq_ignore_ascii_case("h")
        })
}
fn stamp_record(stream: &mut Vec<u8>, label: &str, bytes: &[u8]) {
    stream.extend_from_slice(label.as_bytes());
    stream.push(0);
    stream.extend_from_slice(bytes.len().to_string().as_bytes());
    stream.push(0);
    stream.extend_from_slice(sha256::hex(bytes).as_bytes());
    stream.push(0);
}
struct StageSignatures<'a> {
    bundle: &'a str,
    host_binutils: &'a str,
    implementation: &'a str,
}
fn stage_stamp_with_signature(
    root: &Path,
    manifest: &Path,
    source_only: bool,
    rom: Option<&[u8]>,
    signatures: StageSignatures<'_>,
) -> Result<String, String> {
    let mut stream = Vec::new();
    stream.extend_from_slice(b"assets:native:v2\0");
    stream.extend_from_slice(if source_only {
        b"mode:source-only\0"
    } else {
        b"mode:rom\0"
    });
    let mut files = BTreeMap::new();
    for game in native::games() {
        for directory in ["SRC", "GRAPHICS", "SOUND", "TEXT"] {
            stamp_files(
                root,
                &root.join(game.game_dir()).join(directory),
                &mut files,
            )?;
        }
        for directory in ["overlays", "battle"] {
            stamp_files(root, &root.join(game.asm_dir).join(directory), &mut files)?;
        }
    }
    let mut names = Vec::new();
    for game in native::games() {
        let register = SourcePaths::load_for_game(root, game.compiler.as_str())?;
        let overlay_sources = register
            .all_sources()?
            .into_iter()
            .filter(|source| source.owner.overlay_id().is_some())
            .map(|source| source.path)
            .collect::<BTreeSet<_>>();
        for source in overlay_sources {
            let name = relative(root, &source);
            let flags = cflags_for_target_source(game.compiler, &name);
            let signature = compiler_source_tree_signature(root, &source, &[flags])?;
            stamp_record(&mut stream, &name, &signature);
        }
        // A declared overlay unit compiles into its overlay too.
        // An unreadable manifest is stamped by its bytes below and fails the build.
        let units =
            crate::compiler::translation_units::TranslationUnits::load_game(root, game.compiler)
                .map(|units| units.units)
                .unwrap_or_default();
        for unit in units.iter().filter(|unit| unit.overlay.is_some()) {
            let name = relative(root, &root.join(&unit.source));
            let flags = cflags_for_target_source(game.compiler, &name);
            let signature =
                compiler_source_tree_signature(root, &root.join(&unit.source), &[flags])?;
            stamp_record(&mut stream, &format!("unit:{name}"), &signature);
        }
        names.push(text(source_paths_manifest(game.compiler.as_str())?));
        names.push(native::NativePaths::of(&game).index);
        names.push(format!("{}/translation-units.json", game.recon_dir()));
        names.push(format!("{}/text.json", game.recon_dir()));
        names.push(format!("{}/machine.json", game.recon_dir()));
    }
    for name in names {
        let path = root.join(&name);
        if path.is_file() {
            files.insert(name, path);
        }
    }
    files
        .entry(relative(root, manifest))
        .or_insert_with(|| manifest.to_path_buf());
    for (relative, path) in files {
        // C sources and headers reach the assets only through the overlay
        // compiles stamped above, with every header they include; a main-image
        // source records only its name, so editing one rebuilds no asset.
        if is_c_source(&path) {
            stamp_record(&mut stream, &relative, b"<c-source>");
            continue;
        }
        let bytes = fs::read(&path).map_err(|error| format!("{}: {error}", path.display()))?;
        stamp_record(&mut stream, &relative, &bytes);
    }
    if !source_only {
        let document = json(manifest)?;
        for catalog in document["edition_catalogs"]
            .as_array()
            .into_iter()
            .flatten()
        {
            let name = json_string(catalog, "edition catalog")?;
            let spec = crate::text_catalog::ARCHIVES
                .iter()
                .find(|spec| spec.output == name)
                .ok_or("unregistered edition catalog")?;
            let bytes =
                fs::read(root.join(spec.rom)).map_err(|error| format!("{}: {error}", spec.rom))?;
            stamp_record(&mut stream, spec.rom, &bytes);
        }
    }
    stamp_record(
        &mut stream,
        "<compiler-bundle>",
        signatures.bundle.as_bytes(),
    );
    // Overlay runtime windows are built from the compiler container.
    stamp_record(
        &mut stream,
        "<compiler-runtime>",
        crate::compiler::runtime::signature(root)?.as_bytes(),
    );
    stamp_record(
        &mut stream,
        "<host-binutils>",
        signatures.host_binutils.as_bytes(),
    );
    stamp_record(
        &mut stream,
        "<build-assets-implementation>",
        signatures.implementation.as_bytes(),
    );
    if !source_only {
        let rom = rom.ok_or("normal asset mode is missing ROM bytes")?;
        stamp_record(&mut stream, "<rom>", rom);
    }
    Ok(sha256::hex(&stream))
}
fn stage_stamp(
    root: &Path,
    manifest: &Path,
    source_only: bool,
    rom: Option<&[u8]>,
) -> Result<String, String> {
    let bundle_signature = compiler_bundle_signature();
    let host_tool_signature = host_executable_signature(&[
        "arm-none-eabi-as",
        "arm-none-eabi-nm",
        "arm-none-eabi-ld",
        "arm-none-eabi-objcopy",
    ])?;
    let implementation = executable_signature()?;
    stage_stamp_with_signature(
        root,
        manifest,
        source_only,
        rom,
        StageSignatures {
            bundle: &bundle_signature,
            host_binutils: &host_tool_signature,
            implementation: &implementation,
        },
    )
}
#[test]
fn asset_stamp_tracks_sound_and_included_overlay_sources() {
    let directory = tempfile::tempdir().unwrap();
    let root = directory.path();
    for name in [
        "games/THE BROKEN SEAL/SRC/SYSTEM",
        "games/THE BROKEN SEAL/SOUND/SEQUENCE/out",
        "recon/tbs/raw/overlays",
        "recon/tbs/raw/battle",
    ] {
        fs::create_dir_all(root.join(name)).unwrap();
    }
    let manifest = root.join("recon/tbs/assets.json");
    let sound = root.join("games/THE BROKEN SEAL/SOUND/SEQUENCE/SEQUENCES.TSV");
    let header = root.join("games/THE BROKEN SEAL/SRC/shared.h");
    let unit = root.join("recon/tbs/translation-units.json");
    let machine = root.join("recon/tbs/machine.json");
    let overlay = root.join("recon/tbs/raw/overlays/fixture.s");
    let battle = root.join("recon/tbs/raw/battle/fixture.s");
    for path in [
        &manifest, &sound, &header, &unit, &machine, &overlay, &battle,
    ] {
        fs::write(path, "before").unwrap();
    }
    fs::write(
        root.join("games/THE BROKEN SEAL/SRC/resource_373_c_02001000.c"),
        "#include \"shared.h\"\nvoid Test(void) {}\n",
    )
    .unwrap();
    let stamp = || {
        stage_stamp_with_signature(
            root,
            &manifest,
            true,
            None,
            StageSignatures {
                bundle: "stock",
                host_binutils: "stock",
                implementation: "test",
            },
        )
    };
    let mut previous = stamp().unwrap();
    for name in [
        "SRC/FIELD/XIAN/MAP.json",
        "GRAPHICS/FONT/FONT.png",
        "TEXT/MESSAGES.json",
    ] {
        let path = root.join("games/THE BROKEN SEAL").join(name);
        fs::create_dir_all(path.parent().unwrap()).unwrap();
        fs::write(&path, "asset input").unwrap();
        let next = stamp().unwrap();
        assert_ne!(previous, next, "{}", path.display());
        previous = next;
    }
    for path in [&sound, &header, &unit, &machine, &overlay, &battle] {
        fs::write(path, "after").unwrap();
        let next = stamp().unwrap();
        assert_ne!(previous, next, "{}", path.display());
        previous = next;
    }
    fs::write(
        root.join("games/THE BROKEN SEAL/SOUND/SEQUENCE/out/fixture.bin"),
        "ignored",
    )
    .unwrap();
    assert_eq!(previous, stamp().unwrap());
    fs::write(
        root.join("games/THE BROKEN SEAL/SRC/palette.json"),
        "source",
    )
    .unwrap();
    let previous = stamp().unwrap();
    // A main-image source is stamped by name: adding one rebuilds the assets,
    // editing it does not, and editing an overlay source still does.
    let main = root.join("games/THE BROKEN SEAL/SRC/FIELD/MAIN.C");
    fs::write(&main, "void Main(void) {}\n").unwrap();
    let named = stamp().unwrap();
    assert_ne!(previous, named);
    fs::write(&main, "void Main(void) { Main(); }\n").unwrap();
    assert_eq!(named, stamp().unwrap());
    fs::write(
        root.join("games/THE BROKEN SEAL/SRC/resource_373_c_02001000.c"),
        "#include \"shared.h\"\nvoid Test(void) { Test(); }\n",
    )
    .unwrap();
    assert_ne!(named, stamp().unwrap());
    fs::write(&header, "#include \"resource_373_c_02001000.c\"\n").unwrap();
    assert!(stamp().unwrap_err().contains("recursive C source include"));
}
#[test]
fn material_audit_reports_only_game_material_no_build_read() {
    let directory = tempfile::tempdir().unwrap();
    let root = directory.path();
    let [target, _] = native::games();
    let game = target.game_dir();
    let manifest = root.join(target.asset_manifest);
    let git = |args: &[&str]| {
        let output = std::process::Command::new("git")
            .args(args)
            .current_dir(root)
            .output()
            .unwrap();
        assert!(output.status.success(), "git {args:?}");
    };
    git(&["init", "--quiet"]);
    for name in [
        target.asset_manifest.to_string(),
        format!("{game}/SRC/A.JSON"),
        format!("{game}/SRC/B.JSON"),
        format!("{game}/SRC/X.C"),
        format!("{}/source-paths.json", target.recon_dir()),
    ] {
        let path = root.join(name);
        fs::create_dir_all(path.parent().unwrap()).unwrap();
        fs::write(path, "{}\n").unwrap();
    }
    git(&["add", "games", "recon"]);
    let read = || [format!("{game}/SRC/A.JSON")];
    let error = audit_material_consumers(root, &manifest, read()).unwrap_err();
    assert!(error.contains("has no build consumer"), "{error}");
    assert_eq!(
        error.lines().skip(1).map(str::trim).collect::<Vec<_>>(),
        [format!("{game}/SRC/B.JSON")]
    );
    let everything = [format!("{game}/SRC/A.JSON"), format!("{game}/SRC/B.JSON")];
    audit_material_consumers(root, &manifest, everything).unwrap();
    // A manifest that is not a game's own reads no game's material.
    fs::write(root.join("OTHER.JSON"), "{}\n").unwrap();
    audit_material_consumers(root, &root.join("OTHER.JSON"), read()).unwrap();
    // A reused build audits the inputs its manifest recorded, and a manifest
    // without them is rebuilt.
    let output = root.join("region.bin");
    fs::write(&output, [7u8; 4]).unwrap();
    let mut built = serde_json::json!({"format": 1, "rom_base": ROM_BASE, "rom_size": 16,
        "verification": "source_only", "asset_bytes": 4, "inputs": read(), "regions": [{
            "address": ROM_BASE, "size": 4, "end": ROM_BASE + 4, "output_size": 4,
            "output_sha256": sha256::hex(&[7u8; 4]), "kind": "byte-fill", "sources": [],
            "output": output.to_string_lossy()}]});
    let (_, _, inputs) = reusable_asset_manifest(&built, true, 16).unwrap();
    assert!(audit_material_consumers(root, &manifest, inputs).is_err());
    built.as_object_mut().unwrap().remove("inputs");
    assert!(reusable_asset_manifest(&built, true, 16).is_none());
}
fn output_matches(region: &Value) -> bool {
    let Some(output) = region.get("output").and_then(Value::as_str) else {
        return false;
    };
    let Some(expected_size) = region.get("output_size").and_then(Value::as_u64) else {
        return false;
    };
    let Some(expected_digest) = region.get("output_sha256").and_then(Value::as_str) else {
        return false;
    };
    let path = Path::new(output);
    let Ok(metadata) = fs::metadata(path) else {
        return false;
    };
    if !metadata.is_file() || metadata.len() != expected_size {
        return false;
    }
    let Ok(bytes) = fs::read(path) else {
        return false;
    };
    bytes.len() as u64 == expected_size && sha256::hex(&bytes) == expected_digest
}
fn reusable_asset_manifest(
    manifest: &Value,
    source_only: bool,
    rom_size: usize,
) -> Option<(usize, u64, Vec<String>)> {
    if manifest.get("format")?.as_u64()? != 1
        || manifest.get("rom_base")?.as_u64()? != ROM_BASE as u64
        || manifest.get("rom_size")?.as_u64()? != rom_size as u64
        || manifest.get("verification")?.as_str()?
            != if source_only { "source_only" } else { "rom" }
    {
        return None;
    }
    let regions = manifest.get("regions")?.as_array()?;
    if let Some(archives) = manifest.get("text_archives") {
        if archives
            .as_array()?
            .iter()
            .any(|archive| !output_matches(archive))
        {
            return None;
        }
    }
    if regions.is_empty() {
        return None;
    }
    let mut total = 0u64;
    for region in regions {
        let address = region.get("address")?.as_u64()?;
        let size = region.get("size")?.as_u64()?;
        let end = region.get("end")?.as_u64()?;
        if size == 0
            || end != address.checked_add(size)?
            || region.get("output_size")?.as_u64()? != size
            || region.get("kind")?.as_str().is_none()
            || region.get("sources")?.as_array().is_none()
            || !output_matches(region)
        {
            return None;
        }
        total = total.checked_add(size)?;
    }
    if manifest.get("asset_bytes")?.as_u64()? != total {
        return None;
    }
    let inputs = manifest
        .get("inputs")?
        .as_array()?
        .iter()
        .map(|input| input.as_str().map(str::to_string))
        .collect::<Option<Vec<_>>>()?;
    Some((regions.len(), total, inputs))
}
/// Every piece of game material tracked under the manifest's game must be an
/// input this build read, or named by the game's declared review plan. Code
/// and the registries of the game's `recon` scaffolding are exempt by
/// category (`generated_files::unconsumed_material`); nothing else is.
fn audit_material_consumers(
    root: &Path,
    manifest: &Path,
    inputs: impl IntoIterator<Item = String>,
) -> Result<(), String> {
    let Ok(manifest) = fs::canonicalize(manifest) else {
        return Ok(());
    };
    let Some(game) = native::games().into_iter().find(|game| {
        fs::canonicalize(root.join(game.asset_manifest)).is_ok_and(|path| path == manifest)
    }) else {
        return Ok(());
    };
    let directory = game.game_dir();
    let mut consumed = inputs.into_iter().collect::<BTreeSet<_>>();
    consumed.insert(game.asset_manifest.to_string());
    consumed.extend(native::review_plan_inputs(root, directory)?);
    let mut unconsumed = Vec::new();
    for tree in [directory, game.recon_dir()] {
        unconsumed.extend(
            unconsumed_tracked_material(root, tree, &consumed)
                .map_err(|error| format!("tracked material audit: {error}"))?,
        );
    }
    if unconsumed.is_empty() {
        return Ok(());
    }
    Err(format!(
        "tracked game material has no build consumer; make the asset manifest read it or remove it:\n  {}",
        unconsumed.join("\n  ")
    ))
}
fn record_asset_failure(failures: &mut Vec<String>, failure: String) {
    eprintln!("diagnostic {failure}");
    failures.push(failure);
}
struct BuiltRegion {
    address: usize,
    bytes: Vec<u8>,
    sources: Vec<String>,
    details: Value,
}
/// Check one built region against its declared size and the reference ROM,
/// then write it. Returns its manifest record and inputs, or the failure.
fn finish_region(
    ctx: &Context,
    entry: &Value,
    built: BuiltRegion,
    rom: Option<&[u8]>,
    output_directory: &Path,
) -> Result<(Value, Vec<String>), String> {
    let BuiltRegion {
        address,
        bytes,
        sources,
        details,
    } = built;
    let size = number(&entry["size"], "asset size")?;
    if bytes.len() != size {
        return Err(format!(
            "asset at 0x{address:08x}: built 0x{:x}, expected 0x{size:x}",
            bytes.len()
        ));
    }
    if let Some(rom) = rom {
        let start = address - ROM_BASE;
        let expected = rom
            .get(start..start + size)
            .ok_or("asset region lies beyond ROM")?;
        if let Some(first) = bytes.iter().zip(expected).position(|(l, r)| l != r) {
            return Err(format!(
                "asset at 0x{address:08x}: encoded bytes differ at +0x{first:x} of 0x{size:x}"
            ));
        }
    }
    let sources = closure_sources(ctx, entry, sources)
        .map_err(|error| format!("asset at 0x{address:08x}: {error}"))?;
    let output = output_directory.join(format!("{address:08x}.bin"));
    let output_sha256 = sha256::hex(&bytes);
    write_cache_entry_atomically(&output, &bytes)
        .map_err(|error| format!("{}: {error}", output.display()))?;
    Ok((
        serde_json::json!({
            "address": address,
            "size": size,
            "output_size": bytes.len(),
            "output_sha256": output_sha256,
            "end": address + size,
            "kind": entry.get("kind"),
            "sources": sources,
            "output": output.to_string_lossy(),
            "details": details,
        }),
        sources,
    ))
}
/// Complete every region that stops short of its size with the alignment
/// bytes the archive's writers left after its stream: the packer heap,
/// replayed on the target's reference machine, for a stream file; zeros for
/// a stream inside a container file. Without a resource directory nothing
/// supplies them, and a short region fails its size check.
fn align_streams<'a>(
    ctx: &Context,
    manifest: &Value,
    target: crate::targets::DecompTargetId,
    built: Vec<(&'a Value, BuiltRegion)>,
    failures: &mut Vec<String>,
) -> Result<Vec<(&'a Value, BuiltRegion)>, String> {
    let mut aligned = match manifest.get("resource_directory") {
        Some(name) => {
            let directory = json(&ctx.source(json_string(name, "resource directory")?)?)?;
            let machine = json(&ctx.source(json_string(
                &manifest["machine"],
                "reference machine definition",
            )?)?)?;
            let host = packer::Host::of(&machine, target.as_str())?;
            let regions = built
                .iter()
                .map(|(entry, region)| {
                    Ok((
                        region.address,
                        packer::Region {
                            bytes: &region.bytes,
                            size: number(&entry["size"], "asset size")?,
                        },
                    ))
                })
                .collect::<Result<BTreeMap<_, _>, String>>()?;
            packer::alignment(&host, &packer::resource_starts(&directory)?, &regions)
        }
        None => packer::Alignment::new(),
    };
    let mut ready = Vec::new();
    for (entry, mut region) in built {
        match aligned.remove(&region.address) {
            None => ready.push((entry, region)),
            Some(Ok(padding)) => {
                region.bytes.extend(padding);
                ready.push((entry, region));
            }
            Some(Err(error)) => record_asset_failure(
                failures,
                format!("asset at 0x{:08x}: {error}", region.address),
            ),
        }
    }
    Ok(ready)
}
fn native_asset_main(arguments: &[String]) -> Result<(), String> {
    let root = repository_root();
    let options = parse_build_options(arguments, &root)?;
    let rom = if options.source_only {
        None
    } else {
        let path = PathBuf::from(&options.rom);
        let path = if path.is_absolute() {
            path
        } else {
            env::current_dir()
                .map_err(|error| error.to_string())?
                .join(path)
        };
        Some(fs::read(&path).map_err(|error| format!("{}: {error}", path.display()))?)
    };
    let rom_size = rom.as_ref().map_or(options.rom_size, Vec::len);
    let manifest = json(&options.manifest)?;
    if number(&manifest["format"], "asset manifest format")? != 1 {
        return Err("unsupported asset manifest format".to_string());
    }
    fs::create_dir_all(&options.output)
        .map_err(|error| format!("{}: {error}", options.output.display()))?;
    let stamp = stage_stamp(
        &root,
        &options.manifest,
        options.source_only,
        rom.as_deref(),
    )?;
    let stamp_path = options.output.join("stage.stamp");
    let built_manifest = options.output.join("manifest.json");
    if stamp_path.exists()
        && built_manifest.exists()
        && fs::read_to_string(&stamp_path)
            .map_err(|error| error.to_string())?
            .trim()
            == stamp
    {
        if let Ok(previous) = json(&built_manifest) {
            if let Some((count, bytes, inputs)) =
                reusable_asset_manifest(&previous, options.source_only, rom_size)
            {
                audit_material_consumers(&root, &options.manifest, inputs)?;
                println!("assets={count} bytes={bytes} reused=stamp");
                return Ok(());
            }
        }
    }
    let mut ctx = Context::for_game(&root, crate::targets::target_for(options.target));
    if let Some(name) = manifest.get("machine") {
        let name = json_string(name, "reference machine definition")?;
        ctx.lz_machine = Some(
            LzMachine::of(&json(&ctx.source(name)?)?)
                .map_err(|error| format!("{name}: {error}"))?,
        );
    }
    let edition_catalogs = manifest
        .get("edition_catalogs")
        .and_then(Value::as_array)
        .cloned()
        .unwrap_or_default();
    let mut text_archives = Vec::new();
    for catalog in &edition_catalogs {
        let name = json_string(catalog, "edition catalog")?;
        text_archives.push(crate::text_catalog::build_source(
            &root,
            &ctx.source(name)?,
            options.source_only,
        )?);
    }
    let mut entries = manifest
        .get("regions")
        .and_then(Value::as_array)
        .cloned()
        .unwrap_or_default();
    let directory = match manifest.get("resource_directory") {
        Some(name) => Some(json(
            &ctx.source(json_string(name, "resource directory")?)?,
        )?),
        None => None,
    };
    check_fill_placements(&entries, directory.as_ref(), ROM_BASE + rom_size)?;
    expand_closure_packages(&mut ctx, &manifest, &mut entries)?;
    expand_series(&mut ctx, &manifest, &mut entries)?;
    entries.sort_unstable_by_key(|entry| {
        number(
            entry.get("address").unwrap_or(&Value::Null),
            "asset address",
        )
        .unwrap_or(usize::MAX)
    });
    let mut previous_end = ROM_BASE;
    let mut regions = Vec::new();
    let mut failures = Vec::new();
    let mut all_sources = edition_catalogs
        .iter()
        .map(|catalog| json_string(catalog, "edition catalog").map(str::to_string))
        .collect::<Result<Vec<_>, _>>()?;
    // Every built region, streams without their alignment, for the replay.
    let mut built = Vec::new();
    for entry in &entries {
        let address = number(&entry["address"], "asset address")?;
        let size = number(&entry["size"], "asset size")?;
        let end = address.checked_add(size).ok_or("asset region overflows")?;
        if address < previous_end {
            return Err(format!("overlapping asset region at 0x{address:08x}"));
        }
        if address < ROM_BASE || end > ROM_BASE + rom_size {
            return Err(format!("asset region outside ROM at 0x{address:08x}"));
        }
        previous_end = end;
        let (bytes, sources, details) = match build_entry(&mut ctx, entry) {
            Ok(result) => result,
            Err(error) => {
                record_asset_failure(
                    &mut failures,
                    format!(
                        "asset at 0x{address:08x} ({}): {error}",
                        entry
                            .get("kind")
                            .and_then(Value::as_str)
                            .unwrap_or("unknown")
                    ),
                );
                continue;
            }
        };
        built.push((
            entry,
            BuiltRegion {
                address,
                bytes,
                sources,
                details,
            },
        ));
    }
    for (entry, built) in align_streams(&ctx, &manifest, options.target, built, &mut failures)? {
        match finish_region(&ctx, entry, built, rom.as_deref(), &options.output) {
            Ok((region, sources)) => {
                all_sources.extend(sources);
                regions.push(region);
            }
            Err(failure) => record_asset_failure(&mut failures, failure),
        }
    }
    regions.sort_by_key(|region| region["address"].as_u64());
    let inputs = all_sources
        .iter()
        .map(|source| root_relative(&root, Path::new(source)).unwrap_or_else(|_| source.clone()))
        .chain(ctx.opened_names())
        .chain([relative(&root, &options.manifest)])
        .collect::<BTreeSet<_>>();
    if let Err(error) = audit_material_consumers(&root, &options.manifest, inputs.iter().cloned()) {
        record_asset_failure(&mut failures, error);
    }
    if !failures.is_empty() {
        return Err(format!(
            "asset build found {} failure(s):\n  {}",
            failures.len(),
            failures.join("\n  ")
        ));
    }
    let keep = regions
        .iter()
        .filter_map(|region| {
            region
                .get("output")
                .and_then(Value::as_str)
                .map(PathBuf::from)
        })
        .collect::<Vec<_>>();
    prune_files(&options.output, "*.bin", keep.iter())
        .map_err(|error| format!("asset output cleanup: {error}"))?;
    let asset_bytes = regions
        .iter()
        .map(|region| number(&region["size"], "asset size").unwrap_or(0))
        .sum::<usize>();
    let output_manifest = serde_json::json!({
        "format": 1,
        "rom_base": ROM_BASE,
        "rom_size": rom_size,
        "verification": if options.source_only { "source_only" } else { "rom" },
        "asset_bytes": asset_bytes,
        "text_archives": text_archives,
        "inputs": inputs,
        "regions": regions,
    });
    let manifest_bytes = format!("{}\n", canonical_json(&output_manifest));
    write_cache_entry_atomically(&built_manifest, manifest_bytes.as_bytes())
        .map_err(|error| format!("{}: {error}", built_manifest.display()))?;
    write_cache_entry_atomically(&stamp_path, format!("{stamp}\n").as_bytes())
        .map_err(|error| format!("{}: {error}", stamp_path.display()))?;
    println!(
        "assets={} bytes={asset_bytes}",
        output_manifest["regions"].as_array().map_or(0, Vec::len)
    );
    Ok(())
}
fn run(arguments: Vec<String>) -> Result<ExitCode, String> {
    if arguments.first().map(String::as_str) == Some("--verify-text") {
        if arguments.len() > 2 {
            return Err(USAGE.into());
        }
        println!(
            "{}",
            crate::text_catalog::verify(&repository_root(), arguments.get(1).map(String::as_str))?
        );
        return Ok(ExitCode::SUCCESS);
    }
    if arguments.first().map(String::as_str) == Some("--extract-text") {
        if arguments.len() > 2 {
            return Err(USAGE.into());
        }
        println!(
            "{}",
            crate::text_catalog::extract(&repository_root(), arguments.get(1).map(String::as_str))?
        );
        return Ok(ExitCode::SUCCESS);
    }
    if arguments.first().map(String::as_str) == Some("--review-images") {
        let update = arguments.len() == 3 && arguments[2] == "--update-baseline";
        let target = (arguments.len() == 4 && arguments[2] == "--target")
            .then(|| crate::targets::decomp_target(Some(&arguments[3])))
            .transpose()?;
        if arguments.len() != 2 && !update && target.is_none() {
            return Err(USAGE.into());
        }
        // The Broken Seal keeps its registered review plan; other games review
        // the field maps their private-inputs.json scenes load.
        match target.filter(|t| t.source_dir != native::broken_seal().source) {
            Some(target) => {
                native::export_field_review(&repository_root(), Path::new(&arguments[1]), &target)?
            }
            None => native::export_review(&repository_root(), Path::new(&arguments[1]), update)?,
        }
        return Ok(ExitCode::SUCCESS);
    }
    if arguments.first().map(String::as_str) == Some("--network") {
        derive_index::network::run(&repository_root(), &arguments[1..])?;
        return Ok(ExitCode::SUCCESS);
    }
    if arguments.first().map(String::as_str) == Some("--derive-index") {
        derive_index::run(&repository_root(), &arguments[1..])?;
        return Ok(ExitCode::SUCCESS);
    }
    if arguments.first().map(String::as_str) == Some("--extract-missing-sources") {
        let (rom, target) = rom_and_target(&arguments[1..])?;
        native::extract_missing(&repository_root(), Path::new(&rom), &target)?;
        return Ok(ExitCode::SUCCESS);
    }
    if arguments.first().map(String::as_str) == Some("--audit-characters") {
        native::audit_characters(&repository_root(), &arguments[1..])?;
        return Ok(ExitCode::SUCCESS);
    }
    if arguments.first().map(String::as_str) == Some("--extract-sources") {
        let (rom, target) = rom_and_target(&arguments[1..])?;
        native::extract(&repository_root(), Path::new(&rom), &target)?;
        return Ok(ExitCode::SUCCESS);
    }
    if matches!(
        arguments.first().map(String::as_str),
        Some("--verify-smsh-midi" | "--verify-smsh-source")
    ) {
        if arguments.len() != 3 {
            return Err(USAGE.to_string());
        }
        let rom = fs::read(&arguments[1]).map_err(|error| format!("{}: {error}", arguments[1]))?;
        let (built, report, label) = if arguments[0] == "--verify-smsh-midi" {
            let (built, report) =
                build_midi_sequence(&repository_root(), Path::new(&arguments[2]))?;
            (built, report, "MIDI")
        } else {
            let (built, report) = build_sequence_source(&json(Path::new(&arguments[2]))?)?;
            (built, report, "source")
        };
        let base = number(&report["base"], "base")?;
        let end = base
            .checked_add(built.len())
            .ok_or("sequence extent overflows")?;
        let start = base.checked_sub(ROM_BASE).ok_or("sequence precedes ROM")?;
        if rom.get(start..start + built.len()) != Some(built.as_slice()) {
            return Err(format!("sequence {label} differs at 0x{base:08x}"));
        }
        println!(
            "identical=true base=0x{base:08x} end=0x{end:08x} bytes={}",
            built.len()
        );
        return Ok(ExitCode::SUCCESS);
    }
    if arguments.first().map(String::as_str) == Some("--adopt-smsh-midi") {
        if arguments.len() != 4 {
            return Err(USAGE.to_string());
        }
        let source = json(Path::new(&arguments[1]))?;
        let midi = fs::read(&arguments[2]).map_err(|error| format!("{}: {error}", arguments[2]))?;
        let adopted = adopt_smsh_midi(&source, &midi)?;
        fs::write(&arguments[3], adopted).map_err(|error| format!("{}: {error}", arguments[3]))?;
        println!("adopted=true output={}", arguments[3]);
        return Ok(ExitCode::SUCCESS);
    }
    if arguments.as_slice() == ["--self-test"] {
        native_sequence_self_test()?;
        println!("{}", closure_self_test()?);
        return Ok(ExitCode::SUCCESS);
    }
    if arguments.as_slice() == ["-h"] || arguments.as_slice() == ["--help"] {
        println!("{USAGE}");
        return Ok(ExitCode::SUCCESS);
    }
    native_asset_main(&arguments)?;
    Ok(ExitCode::SUCCESS)
}
/// `ROM [--target TARGET]`; the target selects the game's native source index.
fn rom_and_target(arguments: &[String]) -> Result<(String, crate::targets::DecompTarget), String> {
    let target = crate::targets::decomp_target(option_value(arguments, "--target")?.as_deref())?;
    let rest = arguments
        .iter()
        .enumerate()
        .filter(|(index, argument)| {
            !argument.starts_with("--target") && !(*index > 0 && arguments[index - 1] == "--target")
        })
        .map(|(_, argument)| argument.clone())
        .collect::<Vec<_>>();
    match rest.as_slice() {
        [rom] if !rom.starts_with('-') => Ok((rom.clone(), target)),
        _ => Err(USAGE.into()),
    }
}
pub fn entry(arguments: &[String]) -> ExitCode {
    match run(arguments.to_vec()) {
        Ok(code) => code,
        Err(error) => {
            eprintln!("error: {error}");
            ExitCode::FAILURE
        }
    }
}
pub(crate) fn check_source_tracking() -> Result<(), String> {
    native::check_tracking(&repository_root())
}
