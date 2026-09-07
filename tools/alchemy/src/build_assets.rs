//! Native entry point for the asset build stage.
use alignment_tail::parse_alignment_tail;
use archive_asset::{build_archive, ArchivePlan, ArchiveStream, PixelFormat};
use asset_paths::AssetPaths;
use cache_entry::write_cache_entry_atomically;
use canonical_json::canonical_json;
use compiler_core::build_io::relative;
use compiler_core::bundle::{
    compiler_bundle_signature, executable_signature, host_executable_signature,
};
use compiler_core::routing::{cflags_for_target_source, CompilerTarget};
use compiler_core::sha256;
use compiler_core::source_inputs::compiler_source_tree_signature;
use compiler_core::source_paths::{SourcePaths, SOURCE_PATHS_MANIFEST};
use disassemble::{assemble_overlay, OverlaySource};
use extract_resource::{PaletteGroup, PaletteOperation};
use gba_header::{build_gba_header_component, read_gba_header_source};
use generated_files::{prune_files, unused_tracked_images};
use import_asset::import_tilemap;
use import_asset::{
    append_conductor_meta, gba_graphics, gba_palette_rgba, indexed_png, midi_events, one_bit_tiles,
    rgba_png, EventBody, MidiEvent, MIDI_BUILD_DIRECTIVE,
};
use serde_json::Value;
use sha1::{Digest, Sha1};
use std::collections::{BTreeMap, BTreeSet, HashMap};
use std::env;
use std::fs;
use std::path::{Path, PathBuf};
use std::process::ExitCode;
const USAGE: &str = "usage: build-assets [-h] [--source-only] [--manifest MANIFEST] [-o OUTPUT] [rom] | --verify-smsh-source ROM SOURCE | --adopt-smsh-midi SOURCE INPUT OUTPUT | --verify-smsh-midi ROM MIDI | --self-test";
const ROM_BASE: usize = 0x0800_0000;
const ROM_SIZE: usize = 0x0080_0000;
const MAP_CONTAINER_HEADER_SIZE: usize = 0x3c;
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
fn required<'a>(
    object: &'a serde_json::Map<String, Value>,
    key: &str,
) -> Result<&'a Value, String> {
    object
        .get(key)
        .ok_or_else(|| format!("archive plan is missing {key}"))
}
fn parse_operation(value: &Value) -> Result<PaletteOperation, String> {
    let items = value
        .as_array()
        .ok_or_else(|| "archive token operation is not an array".to_string())?;
    let tag = items
        .first()
        .and_then(Value::as_str)
        .ok_or_else(|| "archive token operation has no tag".to_string())?;
    match tag {
        "l" if items.len() == 1 => Ok(PaletteOperation::Literal),
        "e" if items.len() == 1 => Ok(PaletteOperation::End),
        "c" if items.len() == 3 => Ok(PaletteOperation::Copy {
            length: number(&items[1], "copy length")? as u32,
            distance: number(&items[2], "copy distance")? as u32,
        }),
        _ => Err("unsupported archive token operation".to_string()),
    }
}
fn parse_group(value: &Value) -> Result<PaletteGroup, String> {
    let items = value
        .as_array()
        .ok_or_else(|| "archive token group is not an array".to_string())?;
    let tag = items
        .first()
        .and_then(Value::as_str)
        .ok_or_else(|| "archive token group has no tag".to_string())?;
    match tag {
        "z" if items.len() == 1 => Ok(PaletteGroup::Zeros),
        "g" if items.len() == 2 => {
            let operations = items[1]
                .as_array()
                .ok_or_else(|| "archive token group operations are not an array".to_string())?
                .iter()
                .map(parse_operation)
                .collect::<Result<Vec<_>, _>>()?;
            Ok(PaletteGroup::Group(operations))
        }
        _ => Err("unsupported archive token group".to_string()),
    }
}
fn hex_bytes(value: &Value, label: &str) -> Result<Vec<u8>, String> {
    let text = value
        .as_str()
        .ok_or_else(|| format!("{label} is not hexadecimal text"))?;
    if text.len() % 2 != 0 {
        return Err(format!("{label} has odd length"));
    }
    (0..text.len())
        .step_by(2)
        .map(|index| {
            u8::from_str_radix(&text[index..index + 2], 16)
                .map_err(|_| format!("{label} is not hexadecimal text"))
        })
        .collect()
}
fn parse_plan(value: Value) -> Result<ArchivePlan, String> {
    let object = value
        .as_object()
        .ok_or_else(|| "archive plan is not an object".to_string())?;
    if number(required(object, "format")?, "format")? != 1
        || required(object, "codec")?.as_str() != Some("golden-sun-offset-palette-lz")
    {
        return Err("unsupported archive plan".to_string());
    }
    let pixel_format = match object
        .get("pixel_format")
        .and_then(Value::as_str)
        .unwrap_or("rgba")
    {
        "rgba" => PixelFormat::Rgba,
        "indexed8" => PixelFormat::Indexed8,
        _ => return Err("unsupported archive pixel format".to_string()),
    };
    let streams = required(object, "streams")?
        .as_array()
        .ok_or_else(|| "archive streams are not an array".to_string())?
        .iter()
        .map(|raw| {
            let stream = raw
                .as_object()
                .ok_or_else(|| "archive stream is not an object".to_string())?;
            let tokens = required(stream, "tokens")?
                .as_array()
                .ok_or_else(|| "archive stream tokens are not an array".to_string())?
                .iter()
                .map(parse_group)
                .collect::<Result<Vec<_>, _>>()?;
            Ok(ArchiveStream {
                decoded_size: number(required(stream, "decoded_size")?, "decoded_size")?,
                encoded_size: number(required(stream, "encoded_size")?, "encoded_size")?,
                tokens,
                lookahead: hex_bytes(required(stream, "lookahead")?, "lookahead")?,
            })
        })
        .collect::<Result<Vec<_>, String>>()?;
    let alignment_tail = match object.get("alignment_tail") {
        None => None,
        Some(value) => {
            let size = number(
                value
                    .as_object()
                    .and_then(|item| item.get("size"))
                    .ok_or_else(|| "alignment_tail is missing size".to_string())?,
                "alignment_tail size",
            )?;
            Some(
                parse_alignment_tail(value, size, 3, "alignment_tail")
                    .map_err(|error| error.to_string())?,
            )
        }
    };
    Ok(ArchivePlan {
        chunk_width: number(required(object, "chunk_width")?, "chunk_width")?,
        chunk_height: number(required(object, "chunk_height")?, "chunk_height")?,
        columns: number(required(object, "columns")?, "columns")?,
        pixel_format,
        offset_width: object
            .get("offset_width")
            .map(|value| number(value, "offset_width"))
            .transpose()?
            .unwrap_or(4),
        stream_alignment: object
            .get("stream_alignment")
            .map(|value| number(value, "stream_alignment"))
            .transpose()?
            .unwrap_or(1),
        streams,
        alignment_tail,
    })
}
fn build_offset_archive(plan_path: &Path, atlas_path: &Path) -> Result<Vec<u8>, String> {
    let plan = parse_plan(
        serde_json::from_slice(&fs::read(plan_path).map_err(|error| error.to_string())?)
            .map_err(|error| format!("invalid archive plan: {error}"))?,
    )?;
    build_archive(
        &fs::read(atlas_path).map_err(|error| error.to_string())?,
        &plan,
    )
    .map_err(|error| error.to_string())
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
    let path = if Path::new(name).is_absolute() {
        PathBuf::from(name)
    } else {
        root.join(name)
    };
    if !path.starts_with(root) {
        return Err(format!(
            "asset source must stay inside the repository: {name}"
        ));
    }
    Ok(path)
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
fn root_sources(root: &Path, paths: &[PathBuf]) -> Result<Vec<String>, String> {
    paths.iter().map(|path| root_relative(root, path)).collect()
}
#[test]
fn library_sources_preserve_order_and_reject_paths_outside_the_root() {
    let root = Path::new("/repo");
    let paths = [
        root.join("index.json"),
        root.join("image.png"),
        root.join("index.json"),
    ];
    assert_eq!(
        root_sources(root, &paths).unwrap(),
        ["index.json", "image.png", "index.json"]
    );
    assert!(root_sources(root, &[PathBuf::from("/elsewhere/image.png")]).is_err());
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
    let (fallback_bytes, _, _) =
        gba_graphics(&fs::read(&fallback_path).map_err(|e| e.to_string())?, 4.0)
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
fn build_component(root: &Path, entry: &Value) -> Result<ComponentResult, String> {
    let kind = json_string(&entry["kind"], "component kind")?;
    let source_name = json_string(&entry["source"], "component source")?;
    let source = root_path(root, source_name)?;
    let (data, details, sources) = match kind {
        "u8-array" | "s8-array" | "be-s16-array" | "le-u16-array" | "le-u32-array" => {
            let document = json(&source)?;
            let pointer = json_string(&entry["pointer"], "array pointer")?;
            let values = document.pointer(pointer).ok_or("array pointer is absent")?;
            let data = integer_array(values, kind)?;
            (
                data,
                serde_json::json!({"pointer":pointer}),
                vec![source_name.to_string()],
            )
        }
        "gba-4bpp-object-bank" => {
            let result = build_object_bank(root, &source)?;
            return Ok(result);
        }
        "gba-4bpp-tiles" | "gba-8bpp-tiles" | "gba-palette" => {
            let bpp = if kind == "gba-4bpp-tiles" { 4.0 } else { 8.0 };
            let (graphics, palette, report) =
                gba_graphics(&fs::read(&source).map_err(|e| e.to_string())?, bpp)
                    .map_err(|e| e.to_string())?;
            let details: Value = serde_json::from_str(&import_asset::sorted_json(&report))
                .map_err(|e| e.to_string())?;
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
                    let count = if bpp == 4.0 { 16 } else { 256 };
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
                let tile_bytes = if bpp == 4.0 { 32 } else { 64 };
                let mut ordered = Vec::new();
                for frame in 0..frames {
                    for y in 0..h {
                        let start = ((frame / columns * h + y) * columns * w + frame % columns * w)
                            * tile_bytes;
                        ordered.extend_from_slice(&built[start..start + w * tile_bytes]);
                    }
                }
                built = ordered;
            }
            (built, details, vec![source_name.to_string()])
        }
        "gba-palette-rgba" => {
            let (built, report) = gba_palette_rgba(&fs::read(&source).map_err(|e| e.to_string())?)
                .map_err(|e| e.to_string())?;
            let details: Value = serde_json::from_str(&import_asset::sorted_json(&report))
                .map_err(|e| e.to_string())?;
            (built, details, vec![source_name.to_string()])
        }
        "1bpp-tiles" => {
            let (built, report) = one_bit_tiles(&fs::read(&source).map_err(|e| e.to_string())?)
                .map_err(|e| e.to_string())?;
            let details: Value = serde_json::from_str(&import_asset::sorted_json(&report))
                .map_err(|e| e.to_string())?;
            (built, details, vec![source_name.to_string()])
        }
        "gba-tilemap16" => {
            let built = import_tilemap(&fs::read_to_string(&source).map_err(|e| e.to_string())?)
                .map_err(|e| e.to_string())?;
            (
                built.clone(),
                serde_json::json!({"entries": built.len() / 2}),
                vec![source_name.to_string()],
            )
        }
        "indexed-bytes" | "raw-lz-bytes" => {
            let encoded = fs::read(&source).map_err(|e| e.to_string())?;
            let image = indexed_png(&encoded).map_err(|e| e.to_string())?;
            let mut built: Vec<u8> = image.pixels.into_iter().map(|pixel| pixel as u8).collect();
            if kind == "raw-lz-bytes" {
                let size = number(&entry["size"], "component size")?;
                built.truncate(size);
                (
                    built.clone(),
                    serde_json::json!({"width": image.width, "height": image.height, "bytes": built.len()}),
                    vec![source_name.to_string()],
                )
            } else {
                let built = import_asset::indexed_bytes(
                    &encoded,
                    number(&entry["size"], "component size")?,
                )
                .map_err(|error| error.to_string())?;
                (
                    built,
                    serde_json::json!({"width": image.width, "height": image.height}),
                    vec![source_name.to_string()],
                )
            }
        }
        "rgba-bytes" => {
            let image = rgba_png(&fs::read(&source).map_err(|e| e.to_string())?)
                .map_err(|e| e.to_string())?;
            (
                image.pixels.clone(),
                serde_json::json!({"width": image.width, "height": image.height, "pixels": image.pixels.len() / 4}),
                vec![source_name.to_string()],
            )
        }
        "little-u16-text" | "little-u16-pairs" => {
            let text = fs::read_to_string(&source).map_err(|error| error.to_string())?;
            let data = if kind == "little-u16-text" {
                import_asset::import_words(&text)?
            } else {
                import_asset::import_pairs(&text)?
            };
            (data, serde_json::json!({}), vec![source_name.to_string()])
        }
        "zero-skip-sprite-archive" => {
            let plan_name = json_string(&entry["plan"], "archive plan")?;
            let palette_name = json_string(&entry["palette"], "archive palette")?;
            let plan = root_path(root, plan_name)?;
            let palette = root_path(root, palette_name)?;
            let plan_section = entry.get("plan_section").and_then(Value::as_str);
            let plan_document = json(&plan)?;
            let document = plan_section
                .and_then(|section| plan_document.get(section))
                .unwrap_or(&plan_document);
            let built = skip_sprite_archive::build_archive(document, &source, &palette)
                .map_err(|error| error.to_string())?;
            (
                built,
                serde_json::json!({"images": number(&document["images"], "images")?, "width": number(&document["width"], "width")?, "height": number(&document["height"], "height")?}),
                vec![
                    source_name.to_string(),
                    plan_name.to_string(),
                    palette_name.to_string(),
                ],
            )
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
    let expected = number(&entry["size"], "component size")?;
    if data.len() != expected {
        return Err(format!(
            "{source_name}: built 0x{:x}, expected 0x{:x}",
            data.len(),
            expected
        ));
    }
    Ok(ComponentResult {
        data,
        sources: dedup_sources(sources),
        details,
    })
}
fn integer_array(value: &Value, kind: &str) -> Result<Vec<u8>, String> {
    let mut output = Vec::new();
    for value in value.as_array().ok_or("integer array is not an array")? {
        if value.is_array() {
            output.extend(integer_array(value, kind)?);
        } else {
            let value = match value.as_i64() {
                Some(value) => value,
                None => i64::try_from(number(value, "array member")?)
                    .map_err(|_| "array member exceeds i64")?,
            };
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
            let mut bytes = Vec::new();
            for record in segment["records"].as_array().ok_or("records missing")? {
                let record = record.as_object().ok_or("record must be an object")?;
                if record.len() != fields.len() {
                    return Err("record fields differ".into());
                }
                let start = bytes.len();
                for field in fields {
                    let name = json_string(&field["name"], "field name")?;
                    bytes.extend(table_values(
                        record.get(name).ok_or("record field absent")?,
                        field,
                        stride,
                        &labels,
                    )?);
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
            let addresses = segment["values"]
                .as_array()
                .ok_or("pointer values missing")?
                .iter()
                .map(|index| {
                    pool.get(number(index, "pool index")?)
                        .copied()
                        .ok_or_else(|| "pool index is outside its pool".to_string())
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
            out.push(value.as_i64().ok_or("field value is not an integer")?);
        }
        Ok(())
    }
    let mut values = Vec::new();
    flatten(value, spec, labels, &mut values)?;
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
    let mut reversed = source.clone();
    reversed["segments"].as_array_mut().unwrap().reverse();
    reversed["segments"][0]["address"] = Value::from(256);
    reversed["segments"][0]["end"] = Value::from(268);
    reversed["segments"][1]["address"] = Value::from(268);
    reversed["segments"][1]["end"] = Value::from(282);
    assert!(typed_table(&reversed).is_err());
}

fn parse_general_tokens(value: &Value) -> Result<Vec<extract_resource::GeneralToken>, String> {
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
                "l" if values.len() == 2 => Ok(extract_resource::GeneralToken::Literal(number(
                    &values[1], "literal",
                )?
                    as u32)),
                "c" if values.len() == 3 => Ok(extract_resource::GeneralToken::Copy {
                    length: number(&values[1], "copy length")? as u32,
                    distance: number(&values[2], "copy distance")? as u32,
                }),
                _ => Err("unsupported general-LZ token".to_string()),
            }
        })
        .collect()
}
fn parse_hex_text(value: &Value, label: &str) -> Result<Vec<u8>, String> {
    json_string(value, label)?;
    hex_bytes(value, label)
}
fn build_general_lz(root: &Path, entry: &Value) -> Result<(Vec<u8>, Vec<String>, Value), String> {
    let components = entry
        .get("components")
        .and_then(Value::as_array)
        .ok_or("general-LZ components are not an array".to_string())?;
    let mut decoded = Vec::new();
    let mut sources = Vec::new();
    let mut reports = Vec::new();
    for component in components {
        let result = build_component(root, component)?;
        if component.get("kind").and_then(Value::as_str) == Some("zero-skip-sprite-archive") {
            let plan_name = json_string(&component["plan"], "archive plan")?;
            let plan_path = root_path(root, plan_name)?;
            let plan_document = json(&plan_path)?;
            let plan = component
                .get("plan_section")
                .and_then(Value::as_str)
                .and_then(|section| plan_document.get(section))
                .unwrap_or(&plan_document);
            let image_count = number(&plan["images"], "images")?;
            let source_name = json_string(&component["source"], "archive source")?;
            if plan.get("atlas_columns").is_some() {
                sources.push(format!("{source_name}_images.8bpp.png"));
            } else {
                for index in 0..image_count {
                    sources.push(format!("{source_name}_images_frame_{index:02}.png"));
                }
            }
            sources.push(plan_name.to_string());
            sources.push(json_string(&component["palette"], "archive palette")?.to_string());
        } else {
            sources.push(json_string(&component["source"], "component source")?.to_string());
            sources.extend(result.sources.iter().skip(1).cloned());
        }
        decoded.extend(result.data);
        reports.push(serde_json::json!({"kind": component.get("kind"), "source": component.get("source"), "details": result.details}));
    }
    let plan_name = json_string(&entry["plan"], "general-LZ plan")?;
    let plan_path = root_path(root, plan_name)?;
    let plan_document = json(&plan_path)?;
    let plan = entry
        .get("plan_section")
        .and_then(Value::as_str)
        .and_then(|section| plan_document.get(section))
        .unwrap_or(&plan_document);
    let codec = json_string(&plan["codec"], "codec")?;
    let decoded_size = number(&plan["decoded_size"], "decoded_size")?;
    if decoded.len() != decoded_size {
        return Err("decoded components do not match plan size".to_string());
    }
    let mut built = match codec {
        "golden-sun-general-lz-prefill" => extract_resource::encode_general_prefill(
            &decoded,
            &parse_general_tokens(plan.get("tokens").ok_or("general-LZ tokens are missing")?)?,
            number(&plan["prefill"], "prefill")?,
            number(plan.get("header").unwrap_or(&Value::from(1)), "header")?,
        )
        .map_err(|e| e.to_string())?,
        "golden-sun-general-lz" => extract_resource::encode_general(
            &decoded,
            &parse_general_tokens(plan.get("tokens").ok_or("general-LZ tokens are missing")?)?,
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
            extract_resource::encode_palette(&decoded, &groups).map_err(|e| e.to_string())?
        }
        _ => return Err("unsupported custom-LZ plan".to_string()),
    };
    if codec == "golden-sun-tagged-palette-lz" {
        if number(&plan["tag"], "tag")? != 1 {
            return Err("tagged palette-LZ plan is missing tag 1".to_string());
        }
        built.insert(0, 1);
    }
    if let Some(lookahead) = plan.get("lookahead") {
        built.extend(parse_hex_text(lookahead, "lookahead")?);
    }
    sources.push(plan_name.to_string());
    Ok((
        built,
        dedup_sources(sources),
        serde_json::json!({"decoded_size": decoded.len(), "tokens": plan.get("tokens").and_then(Value::as_array).map_or(0, Vec::len), "components": reports}),
    ))
}
fn closure_self_test() -> Result<String, String> {
    let root = repository_root();
    let missing = root.join("games/gs1/assets/data/closure/__self_test_missing__/index.json");
    if missing.exists() {
        return Err("closure package self-test path exists".to_string());
    }
    let index = root.join("games/gs1/sound/samples.tsv");
    let text =
        fs::read_to_string(index).map_err(|error| format!("PCM self-test index: {error}"))?;
    let mut rows = text.lines().filter(|line| !line.starts_with('#'));
    if rows.next() != Some("sample\taddress\tfrequency\tloop_start\tsample_count\tsource") {
        return Err("PCM self-test index differs".to_string());
    }
    let present_regions = rows.count();
    if present_regions == 0 {
        return Err("PCM self-test index is empty".to_string());
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
#[derive(Debug, Clone)]
struct BuiltMapContainer {
    id: usize,
    address: usize,
    kind: String,
    data: Vec<u8>,
    sources: Vec<PathBuf>,
}
fn map_container_id(value: &Value) -> Result<usize, String> {
    let value = json_string(value, "map resource id")?;
    let value = value
        .strip_prefix("0x")
        .or_else(|| value.strip_prefix("0X"))
        .unwrap_or(value);
    if value.is_empty() {
        return Err("map resource id must be hexadecimal".into());
    }
    usize::from_str_radix(value, 16).map_err(|_| "map resource id must be hexadecimal".into())
}
fn map_container_source_base(index_path: &Path, source: &str) -> Result<PathBuf, String> {
    if source.is_empty()
        || source.contains('\\')
        || Path::new(source).is_absolute()
        || source
            .split('/')
            .any(|part| part.is_empty() || part == "." || part == "..")
    {
        return Err("map container source must be a relative path below its index".into());
    }
    Ok(index_path
        .parent()
        .ok_or("map container index has no parent")?
        .join(source))
}
fn map_container_required_slots(index: &Value) -> Result<BTreeSet<usize>, String> {
    let mut slots = BTreeSet::new();
    for value in series_values(index, "required_components")? {
        let slot = number(value, "required map component")?;
        if slot >= 6 || !slots.insert(slot) {
            return Err("required map components must be unique slots from 0 through 5".into());
        }
    }
    if !slots.contains(&0) {
        return Err("map container must require component slot 0".into());
    }
    Ok(slots)
}
fn map_container_offsets(
    header: &[u8],
    size: usize,
    required_slots: &BTreeSet<usize>,
) -> Result<[usize; 6], String> {
    if header.len() != MAP_CONTAINER_HEADER_SIZE || size <= MAP_CONTAINER_HEADER_SIZE {
        return Err("invalid map container extent".into());
    }
    let mut offsets = [0usize; 6];
    for (slot, offset) in offsets.iter_mut().enumerate() {
        let start = 0x24 + slot * 4;
        *offset = u32::from_le_bytes(header[start..start + 4].try_into().unwrap()) as usize;
    }
    if required_slots.iter().any(|slot| offsets[*slot] == 0) {
        return Err("map container lacks a required component".into());
    }
    if offsets[0] != MAP_CONTAINER_HEADER_SIZE {
        return Err("map container has an invalid header offset".into());
    }
    let mut previous = MAP_CONTAINER_HEADER_SIZE - 1;
    for offset in offsets {
        if offset == 0 {
            continue;
        }
        if offset <= previous || offset >= size {
            return Err("map container component offsets are not ordered".into());
        }
        previous = offset;
    }
    Ok(offsets)
}
fn map_container_component_end(
    offsets: &[usize; 6],
    slot: usize,
    size: usize,
) -> Result<usize, String> {
    if offsets[slot] == 0 {
        return Err("map component is absent".into());
    }
    Ok(offsets[slot + 1..]
        .iter()
        .copied()
        .find(|offset| *offset != 0)
        .unwrap_or(size))
}
fn map_container_component(
    source_base: &Path,
    header_path: &Path,
    slot: usize,
) -> Result<(Vec<u8>, Vec<PathBuf>), String> {
    let (data, sources) = match slot {
        0 => (
            map_container_components::build_metatiles(header_path, header_path)?,
            vec![header_path.to_path_buf()],
        ),
        1 => (
            map_container_components::build_descriptors(header_path, header_path)?,
            vec![header_path.to_path_buf()],
        ),
        2 => {
            let plan_path =
                PathBuf::from(format!("{}_grid_grid.kind1.json", source_base.display()));
            let plan = json(&plan_path)?;
            let mut sources = vec![plan_path];
            if plan.get("atlas_layers").and_then(Value::as_u64) == Some(5) {
                sources.push(PathBuf::from(format!(
                    "{}_grid_layers.png",
                    source_base.display()
                )));
            } else {
                for name in [
                    "value_low.png",
                    "value_high.png",
                    "attribute_a.png",
                    "attribute_b.png",
                    "sentinels.png",
                ] {
                    sources.push(PathBuf::from(format!(
                        "{}_grid_{name}",
                        source_base.display()
                    )));
                }
            }
            (kind1_map_grid::build_grid(&plan, source_base)?, sources)
        }
        3 => (
            map_container_components::build_queues(header_path, header_path)?,
            vec![header_path.to_path_buf()],
        ),
        4 => (
            map_container_components::build_blend_animation(header_path, header_path)?,
            vec![header_path.to_path_buf()],
        ),
        5 => (
            map_container_components::build_sparse(header_path)?,
            vec![header_path.to_path_buf()],
        ),
        _ => return Err("unsupported map component slot".into()),
    };
    Ok((data, sources))
}
fn build_map_container_series(
    root: &Path,
    index_name: &str,
) -> Result<Vec<BuiltMapContainer>, String> {
    let index_path = root_path(root, index_name)?;
    let index = json(&index_path)?;
    if index.get("format") != Some(&Value::from(1))
        || index.get("kind").and_then(Value::as_str) != Some("golden-sun-map-container-series")
    {
        return Err("unsupported map container index".into());
    }
    let asset_kind = json_string(
        index
            .get("asset_kind")
            .ok_or("map container asset kind is missing")?,
        "map container asset kind",
    )?
    .to_string();
    if !matches!(
        asset_kind.as_str(),
        "golden-sun-tokushu-map" | "golden-sun-chiiki-map"
    ) {
        return Err("unsupported map container asset kind".into());
    }
    let required_slots = map_container_required_slots(&index)?;
    let mut ids = BTreeSet::new();
    let mut addresses = BTreeSet::new();
    let mut built = Vec::new();
    for resource in series_values(&index, "resources")? {
        let resource = resource
            .as_object()
            .ok_or("map container resource must be an object")?;
        let id = map_container_id(resource.get("id").ok_or("map resource id is missing")?)?;
        let address = number(
            resource
                .get("address")
                .ok_or("map resource address is missing")?,
            "map resource address",
        )?;
        let size = number(
            resource.get("size").ok_or("map resource size is missing")?,
            "map resource size",
        )?;
        let source_name = json_string(
            resource
                .get("source")
                .ok_or("map resource source is missing")?,
            "map resource source",
        )?;
        if !ids.insert(id) || !addresses.insert(address) {
            return Err("map container index repeats a resource id or address".into());
        }
        let source_base = map_container_source_base(&index_path, source_name)?;
        let header_path = PathBuf::from(format!("{}.json", source_base.display()));
        let header = map_container_components::build_header(&header_path, None)?;
        let offsets = map_container_offsets(&header, size, &required_slots)?;
        let mut data = header;
        let mut sources = vec![header_path.clone()];
        for slot in 0..6 {
            if offsets[slot] == 0 {
                continue;
            }
            let end = map_container_component_end(&offsets, slot, size)?;
            let (component, component_sources) =
                map_container_component(&source_base, &header_path, slot)
                    .map_err(|error| format!("map component {slot}: {error}"))?;
            if component.len() != end - offsets[slot] {
                return Err(format!("map component {slot} has the wrong size"));
            }
            data.extend(component);
            sources.extend(component_sources);
        }
        if data.len() != size {
            return Err(format!("map resource {id:03x} has the wrong rebuilt size"));
        }
        built.push(BuiltMapContainer {
            id,
            address,
            kind: asset_kind.clone(),
            data,
            sources,
        });
    }
    Ok(built)
}
struct Context {
    root: PathBuf,
    paths: AssetPaths,
    maps: HashMap<String, Vec<BuiltMapContainer>>,
    battle: HashMap<String, Vec<battle_assets::BuiltBattleResource>>,
}
impl Context {
    fn new(root: &Path) -> Self {
        Self {
            root: root.to_path_buf(),
            paths: AssetPaths::new(root),
            maps: HashMap::new(),
            battle: HashMap::new(),
        }
    }
    fn source(&self, name: &str) -> Result<PathBuf, String> {
        root_path(&self.root, name)
    }
    fn map_series(&mut self, index_name: &str) -> Result<Vec<BuiltMapContainer>, String> {
        if !self.maps.contains_key(index_name) {
            let built = build_map_container_series(&self.root, index_name)?;
            self.maps.insert(index_name.to_string(), built);
        }
        Ok(self.maps[index_name].clone())
    }
    fn battle_resources(
        &mut self,
        index_name: &str,
    ) -> Result<&[battle_assets::BuiltBattleResource], String> {
        if !self.battle.contains_key(index_name) {
            let built = battle_assets::build_resource_series(&self.source(index_name)?)?;
            self.battle.insert(index_name.to_string(), built);
        }
        Ok(&self.battle[index_name])
    }
}
#[test]
fn library_asset_builds_reject_invalid_plans_without_populating_caches() {
    let directory = tempfile::tempdir().unwrap();
    let root = directory.path();
    fs::write(root.join("index.json"), b"{}").unwrap();
    let mut ctx = Context::new(root);
    for kind in [
        "golden-sun-sentou-gamen-data",
        "golden-sun-kind2-resource",
        "golden-sun-general-lz",
        "golden-sun-tokushu-map",
        "golden-sun-chiiki-map",
    ] {
        let entry =
            serde_json::json!({"kind":kind,"source":"index.json","address":0,"resource_id":0});
        assert!(build_entry(&mut ctx, &entry).is_err(), "{kind}");
    }
    assert!(ctx.maps.is_empty());
    assert!(ctx.battle_resources("index.json").is_err());
    assert!(ctx.battle.is_empty());
}

#[test]
fn map_container_offsets_preserve_required_component_extents() {
    let mut header = vec![0u8; MAP_CONTAINER_HEADER_SIZE];
    for (slot, offset) in [0x3c_u32, 0x80, 0xa0, 0x120, 0, 0x140]
        .into_iter()
        .enumerate()
    {
        header[0x24 + slot * 4..0x28 + slot * 4].copy_from_slice(&offset.to_le_bytes());
    }
    let required = [0, 1, 2, 3, 5].into_iter().collect();
    let offsets = map_container_offsets(&header, 0x148, &required).unwrap();
    assert_eq!(
        map_container_component_end(&offsets, 3, 0x148).unwrap(),
        0x140
    );
    assert_eq!(
        map_container_component_end(&offsets, 5, 0x148).unwrap(),
        0x148
    );

    let mut missing = header.clone();
    missing[0x24 + 3 * 4..0x28 + 3 * 4].fill(0);
    assert!(map_container_offsets(&missing, 0x148, &required).is_err());
    assert!(map_container_offsets(&[0u8; MAP_CONTAINER_HEADER_SIZE], 0x100, &required).is_err());
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
#[test]
fn battle_resources_report_the_actual_palette_and_build_once() {
    let directory = tempfile::tempdir().unwrap();
    let root = directory.path();
    let png = |pixels: &[u8], width: u32, height: u32, format: PixelFormat| {
        let mut image = Vec::new();
        let mut encoder = png::Encoder::new(&mut image, width, height);
        match format {
            PixelFormat::Rgba => {
                encoder.set_color(png::ColorType::Rgba);
                encoder.set_depth(png::BitDepth::Eight);
            }
            PixelFormat::Indexed8 => {
                encoder.set_color(png::ColorType::Indexed);
                encoder.set_depth(png::BitDepth::Eight);
                encoder.set_palette(
                    (0..256)
                        .flat_map(|value| [value as u8; 3])
                        .collect::<Vec<_>>(),
                );
            }
        }
        encoder
            .write_header()
            .unwrap()
            .write_image_data(pixels)
            .unwrap();
        image
    };
    let mut pixels = vec![0; 512];
    pixels[..4].copy_from_slice(b"TEST");
    for (name, pixels, width, height, format) in [
        ("battle_naiyou.png", pixels, 64, 8, PixelFormat::Indexed8),
        (
            "battle_custom.rgba.png",
            vec![248, 0, 0, 255],
            1,
            1,
            PixelFormat::Rgba,
        ),
    ] {
        fs::write(
            root.join(name),
            png(&pixels, width as u32, height as u32, format),
        )
        .unwrap();
    }
    let stream = extract_resource::encode_general_prefill(
        b"TEST",
        &[extract_resource::GeneralToken::Literal(4)],
        0x1000,
        1,
    )
    .unwrap();
    let size = stream.len() + 2;
    let mut plan = serde_json::json!({
        "kind":"golden-sun-sentou-resource", "source_size":size, "resource_boundary_size":size,
        "image":{"source":"naiyou.png","encoding":"naiyou","canvas_size":512},
        "stream":{"decoded_size":4,"codec":"general-lz-prefill","tokens":[["l",4]]},
        "prefix_palette":{"source":"custom.rgba.png"}
    });
    fs::write(root.join("battle_stream.json"), plan.to_string()).unwrap();
    fs::write(
        root.join("index.json"),
        serde_json::json!({"resources":[{
            "address":"0x08001000","size":size,"source":"battle_stream.json"
        }]})
        .to_string(),
    )
    .unwrap();
    let mut ctx = Context::new(root);
    let mut entries = Vec::new();
    expand_series(
        &mut ctx,
        &serde_json::json!({"series":[{
            "kind":"golden-sun-sentou-resource-series","index":"index.json"
        }]}),
        &mut entries,
    )
    .unwrap();
    assert_eq!(entries[0]["source"], "battle_naiyou.png");
    plan["prefix_palette"]["source"] = Value::Null;
    fs::write(root.join("battle_stream.json"), plan.to_string()).unwrap();
    let (data, sources, report) = build_entry(&mut ctx, &entries[0]).unwrap();
    assert_eq!(data, [&[31, 0][..], &stream].concat());
    assert_eq!(report["source_bytes"], size);
    assert_eq!(
        sources,
        [
            "index.json",
            "battle_stream.json",
            "battle_naiyou.png",
            "battle_custom.rgba.png"
        ]
    );
    assert!(Context::new(root)
        .battle_resources("index.json")
        .unwrap_err()
        .contains("palette source must be a string"));
}
fn expand_series(
    ctx: &mut Context,
    manifest: &Value,
    entries: &mut Vec<Value>,
) -> Result<(), String> {
    let mut grid_addresses: HashMap<String, usize> = HashMap::new();
    if let Some(series) = manifest.get("series").and_then(Value::as_array) {
        for item in series {
            if item.get("kind").and_then(Value::as_str) != Some("golden-sun-map-grid-series") {
                continue;
            }
            for grid in series_values(item, "grids")? {
                let tuple = grid.as_array().ok_or("grid tuple is malformed")?;
                grid_addresses.insert(
                    json_string(&tuple[0], "grid id")?.to_ascii_lowercase(),
                    number(&tuple[1], "grid address")?,
                );
            }
        }
    }
    let series_list = manifest
        .get("series")
        .and_then(Value::as_array)
        .cloned()
        .unwrap_or_default();
    for series in &series_list {
        let kind = json_string(&series["kind"], "series kind")?;
        match kind {
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
            "golden-sun-zero-skip-sprite-series" => {
                let palette = json_string(&series["palette"], "palette")?;
                for resource in series_values(series, "resources")? {
                    let name =
                        json_string(&resource["id"], "sprite resource id")?.to_ascii_lowercase();
                    let directory = ctx.paths.resource_graphics_dir(&name);
                    entries.push(serde_json::json!({
                        "address": resource.get("address"),
                        "size": resource.get("size"),
                        "kind": "golden-sun-general-lz",
                        "plan": format!("{directory}.json"),
                        "plan_section": "compression",
                        "components": [{
                            "kind": "zero-skip-sprite-archive",
                            "size": resource.get("decoded_size"),
                            "source": directory,
                            "plan": format!("{directory}.json"),
                            "plan_section": "archive",
                            "palette": palette
                        }]
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
                    let directory = ctx.paths.resource_graphics_dir(&name);
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
                    let directory = ctx.paths.resource_graphics_dir(&name);
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
                    let directory = ctx.paths.resource_graphics_dir(&name);
                    entries.push(serde_json::json!({"address":resource.get("address"),"size":resource.get("size"),"kind":"gba-palette-rgba","source":format!("{directory}_color_table.rgba.png")}));
                }
            }
            "golden-sun-standalone-tile-series" => {
                for resource in series_values(series, "resources")? {
                    let name = json_string(&resource["id"], "tile id")?.to_ascii_lowercase();
                    let directory = ctx.paths.resource_graphics_dir(&name);
                    entries.push(serde_json::json!({"address":resource.get("address"),"size":resource.get("size"),"kind":"golden-sun-kind2-lz","plan":format!("{directory}_tiles.kind2.json"),"components":[{"kind":"gba-4bpp-tiles","size":"0x4000","source":format!("{directory}_tiles.4bpp.png")}] }));
                }
            }
            "golden-sun-prefill-lz-series" => {
                for resource in series_values(series, "resources")? {
                    let name = json_string(&resource["id"], "prefill id")?.to_ascii_lowercase();
                    let directory = format!("games/gs1/assets/data/resource_{name}");
                    entries.push(serde_json::json!({"address":resource.get("address"),"size":resource.get("size"),"kind":"golden-sun-general-lz","plan":format!("{directory}_stream.lz.json"),"components":[{"kind":"raw-lz-bytes","size":resource.get("decoded_size"),"source":format!("{directory}_content.png")}] }));
                }
            }
            "golden-sun-thumb-overlay-series" => {
                for resource in series_values(series, "resources")? {
                    let tuple = resource
                        .as_array()
                        .ok_or("overlay resource tuple malformed")?;
                    let name = json_string(&tuple[0], "overlay id")?.to_ascii_lowercase();
                    let directory = format!("games/gs1/assets/code/resource_{name}");
                    entries
                        .push(serde_json::json!({"address":tuple[1],"size":tuple[2],"kind":"golden-sun-general-lz","plan":format!("{directory}_stream.lz.json"),"components":[{"kind":"golden-sun-thumb-overlay","size":tuple[3],"source":format!("{directory}_overlay.s"),"base":series.get("base")}] }));
                }
            }
            "golden-sun-map-grid-series" => {
                for grid in series_values(series, "grids")? {
                    let tuple = grid.as_array().ok_or("grid tuple malformed")?;
                    let name = json_string(&tuple[0], "grid id")?.to_ascii_lowercase();
                    let directory = format!("games/gs1/assets/maps/map_{name}");
                    entries.push(serde_json::json!({"address":tuple[1],"size":tuple[2],"kind":"golden-sun-kind1-grid","source":directory,"plan":format!("{directory}_grid_grid.kind1.json")}));
                }
            }
            "golden-sun-map-component-series" => {
                for family in series_values(series, "families")? {
                    let tuple = family.as_array().ok_or("map family malformed")?;
                    let name = json_string(&tuple[0], "map family id")?.to_ascii_lowercase();
                    let directory = format!("games/gs1/assets/maps/map_{name}");
                    let container = number(&tuple[1], "map container")?;
                    let mut offsets = serde_json::Map::new();
                    for raw in &tuple[3..] {
                        let item = raw.as_array().ok_or("map component malformed")?;
                        let slot = number(&item[0], "map component slot")?;
                        let address = number(&item[1], "map component address")?;
                        offsets.insert(slot.to_string(), Value::from(address - container));
                    }
                    offsets.insert(
                        "2".to_string(),
                        Value::from(
                            grid_addresses
                                .get(&name)
                                .ok_or_else(|| format!("missing grid address for {name}"))?
                                - container,
                        ),
                    );
                    entries.push(serde_json::json!({"address":tuple[1],"size":tuple[2],"kind":"golden-sun-map-container-header","source":format!("{directory}.json"),"offsets_check":Value::Object(offsets)}));
                    for raw in &tuple[3..] {
                        let item = raw.as_array().ok_or("map component malformed")?;
                        let slot = number(&item[0], "map component slot")?;
                        let component_kind = match slot {
                            0 => "golden-sun-map-metatiles",
                            1 => "golden-sun-map-descriptors",
                            3 => "golden-sun-map-animation-queues",
                            4 => "golden-sun-map-blend-animation",
                            5 => "golden-sun-map-sparse-cells",
                            _ => return Err("unsupported map component slot".into()),
                        };
                        let source = format!("{directory}.json");
                        entries.push(serde_json::json!({"address":item[1],"size":item[2],"kind":component_kind,"source":source}));
                    }
                }
            }
            "golden-sun-sentou-resource-series" => {
                let index_name = json_string(&series["index"], "sentou index")?;
                let root = ctx.root.clone();
                for resource in ctx.battle_resources(index_name)? {
                    let image = root_relative(&root, &resource.sources[1])?;
                    entries.push(serde_json::json!({"address":hex_address(resource.address),"size":resource.data.len(),"kind":"golden-sun-sentou-resource","source":image,"index":index_name}));
                }
            }
            "golden-sun-kind2-resource-series" => {
                let index_name = json_string(&series["index"], "kind2 index")?;
                let index_path = ctx.source(index_name)?;
                let index = json(&index_path)?;
                for resource in series_values(&index, "resources")? {
                    let plan_name = index_path
                        .parent()
                        .unwrap_or(Path::new("."))
                        .join(json_string(&resource["source"], "kind2 plan")?);
                    let plan = json(&plan_name)?;
                    let image = format!(
                        "{}{}",
                        plan_name.to_string_lossy().replace("stream.json", ""),
                        json_string(&plan["image"]["source"], "kind2 image")?
                    );
                    entries.push(serde_json::json!({"address":resource.get("address"),"size":resource.get("size"),"kind":"golden-sun-kind2-resource","source":root_relative(&ctx.root, &ctx.source(&image)?)?,"index":index_name}));
                }
            }
            "golden-sun-map-container-series" => {
                let index_name = json_string(&series["index"], "map index")?;
                for resource in ctx.map_series(index_name)? {
                    entries.push(serde_json::json!({"address":resource.address,"size":resource.data.len(),"kind":resource.kind,"source":index_name,"resource_id":resource.id}));
                }
            }
            "golden-sun-sound-sequence-series" => {
                let index_name = json_string(&series["index"], "sequence index")?;
                if !index_name.ends_with(".tsv") {
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
                if !index_name.ends_with(".tsv") {
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
fn series_values<'a>(value: &'a Value, key: &str) -> Result<&'a Vec<Value>, String> {
    value
        .get(key)
        .and_then(Value::as_array)
        .ok_or_else(|| format!("{key} is missing or is not an array"))
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
    let supported = [
        "golden-sun-asset-fragment",
        "golden-sun-kind2-resource-series",
        "golden-sun-pcm-wave-series",
    ];
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
            let source = source_path(entry_source)?;
            let document = json(&source)?;
            let logo = json_string(&document["standard"]["logo"]["source"], "header logo")?;
            let built = build_gba_header_bytes(
                ctx,
                &source,
                address,
                number(&entry["size"], "header size")?,
            )?;
            Ok((
                built.clone(),
                vec![entry_source.to_string(), logo.to_string()],
                serde_json::json!({"standard_header_bytes":built.len()}),
            ))
        }
        "gba-4bpp-tiles" | "gba-8bpp-tiles" | "1bpp-tiles" | "gba-palette" | "gba-palette-rgba"
        | "indexed-bytes" | "u8-array" | "s8-array" | "be-s16-array" | "le-u16-array"
        | "le-u32-array" => {
            let result = build_component(&ctx.root, entry)?;
            Ok((result.data, result.sources, result.details))
        }
        "golden-sun-general-lz" => {
            let (built, sources, report) = build_general_lz(&ctx.root, entry)?;
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
                let result = build_component(&ctx.root, component)?;
                decoded.extend(result.data);
                sources.extend(result.sources);
                reports.push(serde_json::json!({"kind":component.get("kind"),"source":component.get("source"),"details":result.details}));
            }
            let plan_name = json_string(&entry["plan"], "kind-2 plan")?;
            let plan_path = source_path(plan_name)?;
            let plan_document = json(&plan_path)?;
            let plan_section = entry.get("plan_section").and_then(Value::as_str);
            let plan = plan_section
                .and_then(|section| plan_document.get(section))
                .unwrap_or(&plan_document);
            if let Some(layout) = entry.get("layout") {
                if layout != &Value::Null && plan.get("layout") != Some(layout) {
                    return Err("tag-2 plan layout differs from manifest".to_string());
                }
            }
            if decoded.len() != number(&plan["decoded_size"], "decoded_size")? {
                return Err("decoded tag-2 components do not match plan".to_string());
            }
            let built = kind2_resources::encode_kind2_plan(&decoded, &plan_path, plan_section)?;
            sources.push(plan_name.to_string());
            Ok((
                built,
                dedup_sources(sources),
                serde_json::json!({"decoded_size":decoded.len(),"tokens":plan["stream"]["tokens"].as_array().map_or(0,Vec::len),"layout":plan.get("layout").cloned().unwrap_or(Value::Null),"components":reports}),
            ))
        }
        "golden-sun-kind1-grid" => {
            let plan_name = json_string(&entry["plan"], "grid plan")?;
            let directory = source_path(entry_source)?;
            let plan = json(&source_path(plan_name)?)?;
            let built = kind1_map_grid::build_grid(&plan, &directory)?;
            let nested = vec![format!("{entry_source}_grid_layers.png")];
            Ok((
                built,
                std::iter::once(plan_name.to_string())
                    .chain(nested)
                    .collect(),
                serde_json::json!({"decoded_size":number(&plan["decoded_size"], "decoded_size")?,"tokens":plan["tokens"].as_array().map_or(0,Vec::len),"planes":4}),
            ))
        }
        "golden-sun-map-metatiles"
        | "golden-sun-map-descriptors"
        | "golden-sun-map-animation-queues"
        | "golden-sun-map-blend-animation" => {
            let component = match kind {
                "golden-sun-map-metatiles" => "metatiles",
                "golden-sun-map-descriptors" => "descriptors",
                "golden-sun-map-animation-queues" => "queues",
                _ => "blend",
            };
            let plan_name = entry
                .get("plan")
                .map(|plan| json_string(plan, "map component plan"))
                .transpose()?
                .unwrap_or(entry_source);
            let build = match component {
                "metatiles" => map_container_components::build_metatiles,
                "descriptors" => map_container_components::build_descriptors,
                "queues" => map_container_components::build_queues,
                _ => map_container_components::build_blend_animation,
            };
            let built = build(&source_path(entry_source)?, &source_path(plan_name)?)?;
            let document = json(&source_path(plan_name)?)?;
            let section = match component {
                "queues" => "animation_queues",
                "blend" => "blend_animation",
                other => other,
            };
            let document = document.get(section).unwrap_or(&document);
            let plan = document.get("compression").unwrap_or(&document);
            Ok((
                built,
                vec![entry_source.to_string()],
                serde_json::json!({"decoded_size":number(&plan["decoded_size"], "decoded_size")?,"tokens":plan["tokens"].as_array().map_or(0,Vec::len),"component":plan.get("component")}),
            ))
        }
        "golden-sun-map-container-header" => {
            let built = map_container_components::build_header(&source_path(entry_source)?, None)?;
            let document = json(&source_path(entry_source)?)?;
            let document = document.get("header").unwrap_or(&document);
            Ok((
                built,
                vec![entry_source.to_string()],
                serde_json::json!({"records":document["records"].as_array().map_or(0,Vec::len),"component_offsets":document.get("component_offsets")}),
            ))
        }
        "golden-sun-map-sparse-cells" => {
            let built = map_container_components::build_sparse(&source_path(entry_source)?)?;
            let document = json(&source_path(entry_source)?)?;
            let document = document.get("sparse_cells").unwrap_or(&document);
            Ok((
                built,
                vec![entry_source.to_string()],
                serde_json::json!({"records":document["records"].as_array().map_or(0,Vec::len),"alignment_zeros":document.get("alignment_zeros")}),
            ))
        }
        "record-table" | "pointer-table" => {
            let document = json(&source_path(entry_source)?)?;
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
fn build_gba_header_bytes(
    ctx: &Context,
    source: &Path,
    address: usize,
    size: usize,
) -> Result<Vec<u8>, String> {
    let document = read_gba_header_source(source)?;
    let logo = json_string(&document["standard"]["logo"]["source"], "header logo")?;
    let logo_path = ctx.source(logo)?;
    build_gba_header_component(
        &document,
        &fs::read(logo_path).map_err(|e| e.to_string())?,
        address as u32,
        size,
    )
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
fn midi_hex(data: &str) -> Result<Vec<u8>, String> {
    if data.len() % 2 != 0 {
        return Err("MIDI metadata has odd hexadecimal length".to_string());
    }
    (0..data.len())
        .step_by(2)
        .map(|index| {
            u8::from_str_radix(&data[index..index + 2], 16)
                .map_err(|_| "MIDI metadata is not hexadecimal".to_string())
        })
        .collect()
}
#[derive(Clone)]
struct MidiNode {
    compact_tick: i64,
    raw_tick: i64,
    order: usize,
    event: Value,
}
fn reconstruct_midi_stream(events: &[MidiEvent]) -> Result<Vec<Value>, String> {
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
                let text = String::from_utf8(midi_hex(data)?)
                    .map_err(|_| "MIDI cue is not UTF-8".to_string())?;
                match text.as_str() {
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
                let value = serde_json::from_slice::<Value>(&midi_hex(data)?)
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
    let durations = [
        96usize, 92, 90, 88, 84, 80, 78, 76, 72, 68, 66, 64, 60, 56, 54, 52, 48, 44, 42, 40, 36,
        32, 30, 28, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4,
        3, 2, 1,
    ];
    let mut output = Vec::new();
    let mut cursor = 0i64;
    for index in grid {
        let tick = nodes[index].compact_tick;
        if tick < cursor {
            return Err("MIDI event order moves backwards".to_string());
        }
        let mut gap = (tick - cursor) as usize;
        while gap > 0 {
            let duration = durations
                .iter()
                .copied()
                .find(|duration| *duration <= gap)
                .ok_or("MIDI wait cannot be tokenized")?;
            output.push(serde_json::json!(["wait", duration]));
            gap -= duration;
        }
        cursor = tick;
        output.push(nodes[index].event.clone());
    }
    Ok(output)
}
fn sha1_hex(data: &[u8]) -> String {
    format!("{:x}", Sha1::digest(data))
}
fn greedy_sequence(events: &[Value]) -> Result<Vec<Value>, String> {
    let mut output = Vec::new();
    let mut running: Option<u8> = None;
    let (mut key, mut velocity) = (0usize, 0usize);
    for event in events {
        let values = event.as_array().ok_or("sequence event is malformed")?;
        let kind = values
            .first()
            .and_then(Value::as_str)
            .ok_or("sequence event has no kind")?;
        if kind == "note" || kind == "note_running" {
            let duration = number(
                values.get(1).ok_or("note duration is missing")?,
                "note duration",
            )?;
            let effective_key = number(values.get(2).ok_or("note key is missing")?, "note key")?;
            let effective_velocity = number(
                values.get(3).ok_or("note velocity is missing")?,
                "note velocity",
            )?;
            let opcode = SEQUENCE_DURATIONS
                .iter()
                .position(|candidate| *candidate == duration)
                .map(|index| 0xcf + index as u8);
            let params = if effective_velocity != velocity {
                vec![effective_key, effective_velocity]
            } else if effective_key != key {
                vec![effective_key]
            } else {
                Vec::new()
            };
            let mut rebuilt = vec![
                Value::String(
                    if opcode.is_some_and(|opcode| running == Some(opcode)) && !params.is_empty() {
                        "note_running"
                    } else {
                        "note"
                    }
                    .to_string(),
                ),
                Value::from(duration),
            ];
            rebuilt.extend(params.into_iter().map(Value::from));
            output.push(Value::Array(rebuilt));
            running = opcode;
            key = effective_key;
            velocity = effective_velocity;
        } else if let Some(opcode) = sequence_control_opcode(kind) {
            output.push(event.clone());
            if sequence_sets_running_status(opcode) {
                running = Some(opcode);
            }
        } else if kind == "control_running" {
            output.push(event.clone());
            running = Some(
                sequence_control_opcode(json_string(
                    values.get(1).ok_or("running control name is missing")?,
                    "running control name",
                )?)
                .ok_or("unknown running control")?,
            );
        } else if kind == "note_end" || kind == "note_end_running" {
            output.push(event.clone());
            running = Some(0xce);
        } else {
            output.push(event.clone());
        }
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
fn apply_sequence_deviations(
    default: &[Value],
    track: &Value,
    name: &str,
) -> Result<Vec<Value>, String> {
    let events = number(&track["events"], "sidecar events")?;
    let hash = json_string(&track["hash"], "sidecar hash")?;
    let encoded = serde_json::to_vec(default).map_err(|e| e.to_string())?;
    if default.len() != events || hash.len() < 16 || sha1_hex(&encoded)[..16] != hash[..16] {
        return Err(format!("sidecar {name} default stream drift"));
    }
    let deviations = track
        .get("deviations")
        .and_then(Value::as_array)
        .ok_or("sidecar deviations are missing")?;
    if deviations.is_empty() {
        return Err(format!("sidecar {name} has no deviations"));
    }
    let mut ordered = deviations.clone();
    ordered.sort_by_key(|deviation| {
        deviation
            .get(0)
            .and_then(Value::as_u64)
            .unwrap_or(usize::MAX as u64)
    });
    let mut output = Vec::new();
    let mut cursor = 0usize;
    let mut previous_end = 0usize;
    for deviation in ordered {
        let items = deviation
            .as_array()
            .ok_or("sidecar deviation is malformed")?;
        if items.len() < 2 {
            return Err("sidecar deviation is too short".to_string());
        }
        let start = number(&items[0], "sidecar deviation start")?;
        let count = number(&items[1], "sidecar deviation length")?;
        if count == 0 || start < previous_end || start + count > default.len() {
            return Err("sidecar deviation span is invalid".to_string());
        }
        output.extend_from_slice(&default[cursor..start]);
        output.extend(items.iter().skip(2).cloned());
        cursor = start + count;
        previous_end = cursor;
    }
    output.extend_from_slice(&default[cursor..]);
    Ok(output)
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
                let data = midi_hex(data)?;
                output.extend([0xff, *meta]);
                output.extend(midi_variable(data.len()));
                output.extend(data);
            }
            EventBody::Sysex { status, data } => {
                let data = midi_hex(data)?;
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
fn build_midi_sequence(_root: &Path, source: &Path) -> Result<(Vec<u8>, Value), String> {
    let report = midi_events(&fs::read(source).map_err(|e| format!("{}: {e}", source.display()))?)
        .map_err(|e| e.to_string())?;
    let mut by_track = HashMap::<usize, Vec<MidiEvent>>::new();
    for event in report.events {
        by_track.entry(event.track).or_default().push(event);
    }
    let conductor = by_track.get(&0).cloned().unwrap_or_default();
    let midi_directive_data = conductor
        .iter()
        .find_map(|event| match &event.body {
            EventBody::Meta { meta: 0x7f, data } => Some(midi_hex(data)),
            _ => None,
        })
        .transpose()?;
    let midi_directive = midi_directive_data
        .as_deref()
        .and_then(|data| data.strip_prefix(MIDI_BUILD_DIRECTIVE))
        .map(|source| {
            serde_json::from_slice::<Value>(source)
                .map_err(|error| format!("MIDI build directive: {error}"))
        })
        .transpose()?;
    let marker = conductor
        .iter()
        .find_map(|event| match &event.body {
            EventBody::Meta { meta: 0x01, data } => Some(data),
            _ => None,
        })
        .ok_or("MIDI conductor skeleton is missing")?;
    let skeleton: Value = serde_json::from_slice(&midi_hex(marker)?)
        .map_err(|e| format!("MIDI conductor skeleton: {e}"))?;
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
        let events = reconstruct_midi_stream(
            by_track
                .get(&stream_index)
                .map(Vec::as_slice)
                .unwrap_or(&[]),
        )?;
        let canonical = greedy_sequence(&events)?;
        layout.push(serde_json::json!({
            "kind": "stream",
            "label": label,
            "events": canonical
        }));
    }
    if let Some(sidecar) = midi_directive {
        if number(&sidecar["format"], "sidecar format")? != 1
            || sidecar.get("engine").and_then(Value::as_str) != Some("smsh-sequence-sidecar")
        {
            return Err("invalid sequence sidecar".to_string());
        }
        let tracks = sidecar
            .get("tracks")
            .and_then(Value::as_object)
            .ok_or("sidecar tracks are missing")?;
        if tracks.is_empty() {
            return Err("empty sequence sidecar should be omitted".to_string());
        }
        for segment in layout
            .iter_mut()
            .filter(|segment| segment.get("kind").and_then(Value::as_str) == Some("stream"))
        {
            let label = json_string(&segment["label"], "sidecar stream label")?;
            if let Some(track) = tracks.get(label) {
                let defaults = segment["events"]
                    .as_array()
                    .ok_or("sidecar stream is not in MIDI")?;
                let applied = apply_sequence_deviations(defaults, track, label)?;
                segment["events"] = Value::Array(applied);
            }
        }
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

fn adopt_smsh_midi(source: &Value, midi: &[u8]) -> Result<Vec<u8>, String> {
    let native_tracks = source
        .get("layout")
        .and_then(Value::as_array)
        .ok_or("sequence source layout is missing")?
        .iter()
        .filter(|segment| segment.get("kind").and_then(Value::as_str) == Some("stream"))
        .count();
    let midi = repack_midi_tracks(midi, native_tracks)?;
    let report = midi_events(&midi).map_err(|error| error.to_string())?;
    let mut by_track = HashMap::<usize, Vec<MidiEvent>>::new();
    for event in report.events {
        by_track.entry(event.track).or_default().push(event);
    }
    let source_layout = source
        .get("layout")
        .and_then(Value::as_array)
        .ok_or("sequence source layout is missing")?;
    let mut skeleton_layout = Vec::new();
    let mut sidecar_tracks = serde_json::Map::new();
    let mut stream_index = 0usize;
    for segment in source_layout {
        if segment.get("kind").and_then(Value::as_str) != Some("stream") {
            skeleton_layout.push(segment.clone());
            continue;
        }
        stream_index += 1;
        let label = json_string(&segment["label"], "stream label")?;
        let native = segment
            .get("events")
            .and_then(Value::as_array)
            .ok_or("sequence stream events missing")?;
        let defaults = greedy_sequence(&reconstruct_midi_stream(
            by_track
                .get(&stream_index)
                .map(Vec::as_slice)
                .unwrap_or(&[]),
        )?)?;
        if defaults.is_empty() {
            return Err(format!(
                "MIDI track {stream_index} has no reconstructible events"
            ));
        }
        let encoded = serde_json::to_vec(&defaults).map_err(|error| error.to_string())?;
        let mut replacement = vec![Value::from(0), Value::from(defaults.len())];
        replacement.extend(native.iter().cloned());
        sidecar_tracks.insert(
            label.to_string(),
            serde_json::json!({
                "events":defaults.len(),
                "hash":&sha1_hex(&encoded)[..16],
                "deviations":[replacement]
            }),
        );
        skeleton_layout.push(serde_json::json!({"kind":"stream", "label":label}));
    }
    if by_track.keys().copied().max().unwrap_or(0) != stream_index {
        return Err("MIDI and native sequence track counts differ".to_string());
    }
    let mut skeleton = source.clone();
    skeleton["layout"] = Value::Array(skeleton_layout);
    let skeleton = serde_json::to_vec(&skeleton).map_err(|error| error.to_string())?;
    let sidecar = serde_json::to_vec(&serde_json::json!({
        "format":1,
        "engine":"smsh-sequence-sidecar",
        "tracks":sidecar_tracks
    }))
    .map_err(|error| error.to_string())?;
    if midi
        .windows(MIDI_BUILD_DIRECTIVE.len())
        .any(|part| part == MIDI_BUILD_DIRECTIVE)
    {
        return Err("MIDI already has build directives".to_string());
    }
    let mut directive = MIDI_BUILD_DIRECTIVE.to_vec();
    directive.extend(sidecar);
    let midi = append_conductor_meta(&midi, 0x01, &skeleton).map_err(|error| error.to_string())?;
    append_conductor_meta(&midi, 0x7f, &directive).map_err(|error| error.to_string())
}

fn build_pcm_record(entry: &Value, wav: &[u8]) -> Result<(Vec<u8>, Value), String> {
    let word = |v: &Value, label: &str| {
        u32::try_from(number(v, label)?).map_err(|_| format!("{label} exceeds u32"))
    };
    let header = entry.get("header");
    let frequency = word(&entry["frequency"], "wave frequency")?;
    let (rate, samples) = import_asset::wav_pcm8(wav).map_err(|e| e.to_string())?;
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
            let (built, report) = if entry_source.ends_with(".json") {
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
            let built = import_asset::delta7_image(
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
        "golden-sun-static-sprite-series" => {
            let index = json(&source_path(entry_source)?)?;
            let palette_name = json_string(&entry["palette"], "palette")?;
            let built = static_sprite_series::build_series(
                &index,
                &source_path(entry_source)?,
                &source_path(palette_name)?,
            )
            .map_err(|error| error.to_string())?;
            let directory = Path::new(entry_source)
                .parent()
                .unwrap_or(Path::new("."))
                .to_string_lossy()
                .replace('\\', "/");
            let mut sources = vec![entry_source.to_string(), palette_name.to_string()];
            for item in series_values(&index, "packages")? {
                let plan_name = ctx.paths.character_bank_path(
                    ctx.root.join(&directory),
                    json_string(&item["plan"], "static sprite plan")?,
                );
                let plan_rel = root_relative(&ctx.root, &plan_name)?;
                sources.push(plan_rel.clone());
                let plan = json(&plan_name)?;
                let prefix = plan_rel.replace("bank.json", "");
                if let Some(atlases) = plan.get("atlases").and_then(Value::as_array) {
                    for atlas in atlases {
                        sources.push(format!(
                            "{}{}",
                            prefix,
                            json_string(&atlas["source"], "static atlas source")?
                        ));
                    }
                } else if plan.get("atlas_columns").is_some() {
                    sources.push(format!(
                        "{directory}/{}",
                        json_string(&item["source"], "static package source")?
                    ));
                } else if let Some(frames) = plan.get("frames").and_then(Value::as_array) {
                    for frame in 0..frames.len() {
                        sources.push(format!("{prefix}koma_{frame:03}.png"));
                    }
                }
            }
            Ok((
                built,
                dedup_sources(sources),
                serde_json::json!({"packages":index["packages"].as_array().map_or(0,Vec::len)}),
            ))
        }
        "golden-sun-message-archive" => {
            let document = json(&source_path(entry_source)?)?;
            let built = message_archive::cli::build_message_archive(&document)?;
            let messages = document["banks"].as_array().map_or(0, |banks| {
                banks
                    .iter()
                    .map(|bank| bank.as_array().map_or(0, Vec::len))
                    .sum()
            });
            Ok((
                built,
                vec![entry_source.to_string()],
                serde_json::json!({"banks":document["banks"].as_array().map_or(0,Vec::len),"messages":messages}),
            ))
        }
        "golden-sun-localization-font" => {
            let document = json(&source_path(entry_source)?)?;
            let mut nested = Vec::new();
            for item in document["direct_tiles"]
                .as_array()
                .into_iter()
                .flatten()
                .chain(document["mtf_banks"].as_array().into_iter().flatten())
            {
                nested.push(format!(
                    "games/gs1/assets/{}",
                    flat_asset_name(json_string(&item["source"], "font source")?)
                ));
            }
            nested.push(format!(
                "games/gs1/assets/{}",
                flat_asset_name(json_string(
                    &document["packed_images"]["source"],
                    "packed image source"
                )?)
            ));
            nested.push(format!(
                "games/gs1/assets/{}",
                flat_asset_name(json_string(&document["font"]["source"], "font source")?)
            ));
            for name in &nested {
                ctx.source(name)?;
            }
            let built = localization_font::build_localization_font(
                &document,
                &ctx.root.join("games/gs1/assets"),
            )
            .map_err(|error| error.to_string())?;
            Ok((
                built,
                std::iter::once(entry_source.to_string())
                    .chain(nested)
                    .collect(),
                serde_json::json!({"mtf_images":document["mtf_banks"].as_array().map_or(0,|banks|banks.iter().map(|b|number(&b["images"],"images").unwrap_or(0)).sum()),"packed_images":document["packed_images"]["images"],"font_glyphs":document["font"]["glyphs"],"article_entries":document["articles"]["entries"].as_array().map_or(0,Vec::len)}),
            ))
        }
        "typed-table" => {
            let document = json(&source_path(entry_source)?)?;
            let document = if let Some(pointer) = entry.get("pointer") {
                document
                    .pointer(json_string(pointer, "table pointer")?)
                    .ok_or("table pointer is absent")?
            } else {
                &document
            };
            if number(&document["address"], "table address")? != address {
                return Err("table address differs from manifest".into());
            }
            let mut document = document.clone();
            let symbols = SourcePaths::load(&ctx.root)?;
            resolve_table_symbols(&mut document, &symbols)?;
            let built = typed_table(&document)?;
            Ok((
                built,
                vec![entry_source.to_string()],
                serde_json::json!({"segments":document["segments"].as_array().map_or(0,Vec::len)}),
            ))
        }
        "golden-sun-battle-effect-data" => {
            let document = json(&source_path(entry_source)?)?;
            let mut nested = Vec::new();
            for item in document["direct_graphics"].as_array().into_iter().flatten() {
                nested.push(format!(
                    "games/gs1/assets/{}",
                    flat_asset_name(json_string(item, "battle graphic source")?)
                ));
            }
            for item in std::iter::once(&document["halfword_graphic"]).chain(
                document["palette_graphics"]
                    .as_array()
                    .into_iter()
                    .flatten(),
            ) {
                nested.push(format!(
                    "games/gs1/assets/{}",
                    flat_asset_name(json_string(&item["source"], "battle graphic source")?)
                ));
            }
            for name in &nested {
                ctx.source(name)?;
            }
            let built =
                battle_assets::build_effect_data(&document, &ctx.root.join("games/gs1/assets"))?;
            Ok((
                built,
                std::iter::once(entry_source.to_string())
                    .chain(nested)
                    .collect(),
                serde_json::json!({"graphics":document["direct_graphics"].as_array().map_or(0,Vec::len)+1+document["palette_graphics"].as_array().map_or(0,Vec::len),"weighted_records":document["weighted_records"].as_array().map_or(0,Vec::len),"typed_tables":document["typed_tables"].as_array().map_or(0,Vec::len)}),
            ))
        }
        "golden-sun-sentou-gamen-data" => {
            let (built, sources) = battle_assets::build_screen(&source_path(entry_source)?)?;
            if address != battle_assets::SCREEN_ADDRESS || built.len() != battle_assets::SCREEN_SIZE
            {
                return Err(
                    "battle-screen package differs from canonical manifest extent".to_string(),
                );
            }
            Ok((
                built,
                root_sources(&ctx.root, &sources)?,
                serde_json::json!({"source_bytes":battle_assets::SCREEN_SIZE,"graphics":5,"display_glyph_cells":14,"derived_zero_bytes":3308}),
            ))
        }
        "golden-sun-sentou-hyouji" => {
            let document = json(&source_path(entry_source)?)?;
            let prefix = entry_source.replace("index.json", "");
            let mut nested = vec![entry_source.to_string()];
            for value in [
                &document["sources"]["kihon"],
                &document["sources"]["koma"]["source"],
                &document["sources"]["haichi"],
                &document["sources"]["hosei"],
                &document["sources"]["gauge"]["source"],
            ] {
                nested.push(format!(
                    "{prefix}{}",
                    json_string(value, "battle display source")?
                ));
            }
            for name in &nested {
                ctx.source(name)?;
            }
            let built = battle_assets::build_display(&source_path(entry_source)?)?;
            Ok((
                built.clone(),
                nested,
                serde_json::json!({"source_bytes":built.len(),"typed_tables":3,"atlases":2}),
            ))
        }
        "golden-sun-sentou-kouka-runtime" => {
            let document = json(&source_path(entry_source)?)?;
            let built = battle_assets::build_effect_runtime(&source_path(entry_source)?)?;
            if address != battle_assets::EFFECT_RUNTIME_ADDRESS
                || built.len() != battle_assets::EFFECT_RUNTIME_SIZE
                || built.len() != number(&entry["size"], "effect runtime size")?
            {
                return Err("battle-effect runtime differs from manifest".to_string());
            }
            let directory = Path::new(entry_source).parent().unwrap_or(Path::new("."));
            let mut nested = vec![entry_source.to_string()];
            if let Some(sources) = document["sources"].as_object() {
                for value in sources.values() {
                    nested.push(
                        directory
                            .join(json_string(value, "effect source")?)
                            .to_string_lossy()
                            .replace('\\', "/"),
                    );
                }
            }
            for name in &nested {
                ctx.source(name)?;
            }
            Ok((
                built.clone(),
                dedup_sources(nested),
                serde_json::json!({"source_bytes":built.len(),"callback_slots":407,"derived_zero_bytes":4012}),
            ))
        }
        "golden-sun-sentou-resource" => {
            let index_name = json_string(&entry["index"], "sentou resource index")?;
            let resource = ctx
                .battle_resources(index_name)?
                .iter()
                .find(|item| item.address == address)
                .cloned()
                .ok_or("sentou resource address is absent from its index")?;
            let mut nested = vec![index_name.to_string()];
            nested.extend(root_sources(&ctx.root, &resource.sources)?);
            let report = serde_json::json!({"source_bytes":resource.data.len()});
            Ok((resource.data, dedup_sources(nested), report))
        }
        "golden-sun-kind2-resource" => {
            let plan_path = source_path(entry_source)?;
            let plan = json(&plan_path)?;
            let built = kind2_resources::build_kind2_resource(&plan_path)
                .map_err(|error| error.to_string())?;
            let mut sources = vec![entry["index"].as_str().unwrap_or(entry_source).to_string()];
            sources.extend(root_sources(&ctx.root, &built.sources)?);
            let size = built.data.len();
            Ok((
                built.data,
                dedup_sources(sources),
                serde_json::json!({"resource_id":plan.get("resource_id"),"source_bytes":size}),
            ))
        }
        "golden-sun-tokushu-map" | "golden-sun-chiiki-map" => {
            let index_name = entry_source;
            let id = number(&entry["resource_id"], "map resource id")?;
            let resource = ctx
                .map_series(index_name)?
                .into_iter()
                .find(|item| item.id == id)
                .ok_or("map resource differs from manifest")?;
            if resource.kind != kind {
                return Err("map resource kind differs from manifest".into());
            }
            let data_len = resource.data.len();
            let mut sources = vec![index_name.to_string()];
            sources.extend(root_sources(&ctx.root, &resource.sources)?);
            Ok((
                resource.data,
                sources,
                serde_json::json!({"resource_id":format!("0x{id:03x}"),"source_bytes":data_len}),
            ))
        }
        "golden-sun-gameplay-databases" => {
            let document = json(&source_path(entry_source)?)?;
            let built = resource_5::build_gameplay_databases(&document)?;
            Ok((
                built,
                vec![entry_source.to_string()],
                serde_json::json!({"items":document["items"].as_array().map_or(0,Vec::len),"abilities":document["abilities"].as_array().map_or(0,Vec::len),"combatants":document["combatants"].as_array().map_or(0,Vec::len),"classes":document["classes"].as_array().map_or(0,Vec::len),"djinn":document["djinn"].as_array().map_or(0,Vec::len),"alignment_bytes":document.get("alignment_bytes")}),
            ))
        }
        "golden-sun-title-lz" => {
            let document = json(&source_path(entry_source)?)?;
            let title_prefix = entry_source.replace("container.json", "");
            let mut sources = vec![entry_source.to_string()];
            for component in document["components"].as_array().into_iter().flatten() {
                let relative = format!(
                    "{}{}",
                    title_prefix,
                    json_string(&component["source"], "title component source")?.replace('/', "_")
                );
                ctx.source(&relative)?;
                sources.push(relative);
            }
            let built = title_resources::build_title_resource(&source_path(entry_source)?)
                .map_err(|error| error.to_string())?;
            Ok((
                built,
                sources,
                serde_json::json!({"resource_id":document["resource_id"],"decoded_size":document["decoded_size"],"components":document["components"].as_array().map_or(0,Vec::len),"fallback_tail":if document["tail"]["policy"].as_str()==Some("fallback"){number(&document["tail"]["size"],"tail size")?}else{0}}),
            ))
        }
        "golden-sun-offset-palette-lz" => {
            let plan_name = json_string(&entry["plan"], "offset palette plan")?;
            let plan = json(&source_path(plan_name)?)?;
            let built =
                build_offset_archive(&source_path(plan_name)?, &source_path(entry_source)?)?;
            Ok((
                built,
                vec![entry_source.to_string(), plan_name.to_string()],
                serde_json::json!({"streams":plan["streams"].as_array().map_or(0,Vec::len),"chunk_width":plan["chunk_width"],"chunk_height":plan["chunk_height"]}),
            ))
        }
        "golden-sun-mtf4-archive" => {
            let plan_name = json_string(&entry["plan"], "F0 plan")?;
            let plan = json(&source_path(plan_name)?)?;
            let built = f0_archive::build_archive(&plan, &source_path(entry_source)?)
                .map_err(|error| error.to_string())?;
            let images = number(&plan["images"], "F0 images")?;
            let sources = if plan.get("atlas_columns").is_some() {
                vec![
                    plan_name.to_string(),
                    format!("{entry_source}_images.rgba.png"),
                ]
            } else {
                std::iter::once(plan_name.to_string())
                    .chain(
                        (0..images)
                            .map(|index| format!("{entry_source}_images_image_{index:02}.png")),
                    )
                    .collect()
            };
            Ok((
                built,
                sources,
                serde_json::json!({"entries":plan["entries"].as_array().map_or(0,Vec::len),"images":images}),
            ))
        }
        _ => Err(format!("unsupported asset kind: {kind}")),
    }
}
fn flat_asset_name(name: &str) -> String {
    let parts: Vec<&str> = name.split('/').collect();
    if parts.len() <= 2 {
        return name.to_string();
    }
    format!("{}/{}", parts[0], parts[1..].join("_"))
}
struct BuildOptions {
    rom: String,
    manifest: PathBuf,
    output: PathBuf,
    source_only: bool,
}
fn parse_build_options(arguments: &[String], root: &Path) -> Result<BuildOptions, String> {
    let mut options = BuildOptions {
        rom: "roms/gs1-en.gba".to_string(),
        manifest: root.join("games/gs1/assets/manifest.json"),
        output: root.join("out/gs1-en/assets"),
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
            if entry.path() == root.join("games/gs1/assets/readme") {
                return false; // Coverage figures are outputs, never ROM asset inputs.
            }
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
    for directory in ["games/gs1/assets", "games/gs1/sound"] {
        stamp_files(root, &root.join(directory), &mut files)?;
    }
    let source_paths = SourcePaths::load(root)?;
    let overlay_sources = source_paths
        .all_sources()?
        .into_iter()
        .filter(|source| source.owner.overlay_id().is_some())
        .map(|source| source.path)
        .collect::<BTreeSet<_>>();
    for source in overlay_sources {
        let name = relative(root, &source);
        let flags = cflags_for_target_source(CompilerTarget::Gs1, &name);
        let signature = compiler_source_tree_signature(root, &source, &[flags])?;
        stamp_record(&mut stream, &name, &signature);
    }
    for name in [
        SOURCE_PATHS_MANIFEST,
        "games/gs1/recon/translation-units.json",
    ] {
        let path = root.join(name);
        if path.is_file() {
            files.insert(name.into(), path);
        }
    }
    files
        .entry(relative(root, manifest))
        .or_insert_with(|| manifest.to_path_buf());
    for (relative, path) in files {
        let bytes = fs::read(&path).map_err(|error| format!("{}: {error}", path.display()))?;
        stamp_record(&mut stream, &relative, &bytes);
    }
    stamp_record(
        &mut stream,
        "<compiler-bundle>",
        signatures.bundle.as_bytes(),
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
    for name in ["assets", "sound/out", "src", "recon"] {
        fs::create_dir_all(root.join("games/gs1").join(name)).unwrap();
    }
    let manifest = root.join("games/gs1/assets/manifest.json");
    let sound = root.join("games/gs1/sound/sequences.tsv");
    let header = root.join("games/gs1/src/shared.h");
    let unit = root.join("games/gs1/recon/translation-units.json");
    for path in [&manifest, &sound, &header, &unit] {
        fs::write(path, "before").unwrap();
    }
    fs::write(
        root.join("games/gs1/src/resource_373_c_02001000.c"),
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
    for path in [&sound, &header, &unit] {
        fs::write(path, "after").unwrap();
        let next = stamp().unwrap();
        assert_ne!(previous, next, "{}", path.display());
        previous = next;
    }
    fs::write(root.join("games/gs1/sound/out/fixture.bin"), "ignored").unwrap();
    assert_eq!(previous, stamp().unwrap());
    fs::create_dir_all(root.join("games/gs1/assets/readme")).unwrap();
    fs::write(
        root.join("games/gs1/assets/readme/coverage.svg"),
        "generated",
    )
    .unwrap();
    assert_eq!(previous, stamp().unwrap());
    fs::write(root.join("games/gs1/assets/palette.json"), "source").unwrap();
    assert_ne!(previous, stamp().unwrap());
    fs::write(&header, "#include \"resource_373_c_02001000.c\"\n").unwrap();
    assert!(stamp().unwrap_err().contains("recursive C source include"));
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
) -> Option<(usize, u64)> {
    if manifest.get("format")?.as_u64()? != 1
        || manifest.get("rom_base")?.as_u64()? != ROM_BASE as u64
        || manifest.get("rom_size")?.as_u64()? != rom_size as u64
        || manifest.get("verification")?.as_str()?
            != if source_only { "source_only" } else { "rom" }
    {
        return None;
    }
    let regions = manifest.get("regions")?.as_array()?;
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
    Some((regions.len(), total))
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
    let rom_size = rom.as_ref().map_or(ROM_SIZE, Vec::len);
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
            if let Some((count, bytes)) =
                reusable_asset_manifest(&previous, options.source_only, rom_size)
            {
                println!("assets={count} bytes={bytes} reused=stamp");
                return Ok(());
            }
        }
    }
    let mut ctx = Context::new(&root);
    let mut entries = manifest
        .get("regions")
        .and_then(Value::as_array)
        .cloned()
        .unwrap_or_default();
    expand_closure_packages(&mut ctx, &manifest, &mut entries)?;
    expand_series(&mut ctx, &manifest, &mut entries)?;
    let mut index = 0;
    while index < entries.len() {
        if entries[index].get("kind").and_then(Value::as_str) != Some("integer-region-package") {
            index += 1;
            continue;
        }
        let package = entries.remove(index);
        let source_name = json_string(&package["source"], "integer package source")?;
        let encoding = json_string(&package["encoding"], "integer package encoding")?;
        if !matches!(encoding, "u8-array" | "s8-array" | "be-s16-array") {
            return Err("unknown integer encoding".into());
        }
        let text = fs::read_to_string(ctx.source(source_name)?).map_err(|e| e.to_string())?;
        let document: Value = serde_json::from_str(&text).map_err(|e| e.to_string())?;
        if document["kind"] != "integer-regions"
            || document["format"] != 1
            || !canonical_json::is_canonical_json_text(&text, &document)
        {
            return Err("integer package source differs".into());
        }
        let regions = document
            .get("regions")
            .and_then(Value::as_array)
            .ok_or("integer package regions are missing")?;
        let generated = regions
            .iter()
            .enumerate()
            .map(|(i, region)| {
                let size = integer_array(&region["values"], encoding)?.len();
                if size == 0 {
                    return Err("empty integer region".to_string());
                }
                Ok(serde_json::json!({
                    "address": region.get("address"),
                    "size": size,
                    "kind": encoding,
                    "source": source_name,
                    "pointer": format!("/regions/{i}/values"),
                }))
            })
            .collect::<Result<Vec<_>, String>>()?;
        entries.splice(index..index, generated);
    }
    entries.sort_unstable_by_key(|entry| {
        number(
            entry.get("address").unwrap_or(&Value::Null),
            "asset address",
        )
        .unwrap_or(usize::MAX)
    });
    let mut previous_end = ROM_BASE;
    let mut regions = Vec::new();
    let mut all_sources = Vec::<String>::new();
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
        let (built, source_names, details) = build_entry(&mut ctx, entry).map_err(|error| {
            format!(
                "asset at 0x{address:08x} ({}): {error}",
                entry
                    .get("kind")
                    .and_then(Value::as_str)
                    .unwrap_or("unknown")
            )
        })?;
        if built.len() != size {
            return Err(format!(
                "asset at 0x{address:08x}: built 0x{:x}, expected 0x{:x}",
                built.len(),
                size
            ));
        }
        if let Some(rom) = rom.as_ref() {
            let start = address
                .checked_sub(ROM_BASE)
                .ok_or("asset address precedes ROM")?;
            let expected = rom
                .get(start..start + size)
                .ok_or("asset region lies beyond ROM")?;
            if built != expected {
                return Err(format!("asset at 0x{address:08x}: encoded bytes differ"));
            }
        }
        let sources = closure_sources(&ctx, entry, source_names)?;
        all_sources.extend(sources.iter().cloned());
        let output = options.output.join(format!("{address:08x}.bin"));
        let output_sha256 = sha256::hex(&built);
        write_cache_entry_atomically(&output, &built)
            .map_err(|error| format!("{}: {error}", output.display()))?;
        regions.push(serde_json::json!({
            "address": address,
            "size": size,
            "output_size": built.len(),
            "output_sha256": output_sha256,
            "end": end,
            "kind": entry.get("kind"),
            "sources": sources,
            "output": output.to_string_lossy(),
            "details": details,
        }));
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
    let unused = unused_tracked_images(&root, all_sources.iter(), ["games/gs1/assets/readme/"])
        .map_err(|error| format!("tracked image audit: {error}"))?;
    if !unused.is_empty() {
        let shown = unused
            .iter()
            .take(20)
            .cloned()
            .collect::<Vec<_>>()
            .join("\n  ");
        let suffix = if unused.len() > 20 {
            format!("\n  ... and {} more", unused.len() - 20)
        } else {
            String::new()
        };
        return Err(format!(
            "tracked images are not byte-verified asset sources:\n  {shown}{suffix}"
        ));
    }
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
pub fn entry(arguments: &[String]) -> ExitCode {
    match run(arguments.to_vec()) {
        Ok(code) => code,
        Err(error) => {
            eprintln!("error: {error}");
            ExitCode::FAILURE
        }
    }
}
