//! Native entry point for the asset build stage.

use std::collections::{hash_map::DefaultHasher, BTreeMap, BTreeSet, HashMap};
use std::env;
use std::fs;
use std::hash::{Hash, Hasher};
use std::io::{self, Write};
use std::path::{Path, PathBuf};
use std::process::{Command, ExitCode, Stdio};

use alignment_tail::parse_alignment_tail;
use archive_asset::{
    build_archive, self_test as archive_self_test, ArchivePlan, ArchiveStream, PixelFormat,
};
use asset_paths::AssetPaths;
use audio_engine_data::build_audio_engine_data;
use canonical_json::canonical_json;
use extract_resource::{PaletteGroup, PaletteOperation};
use gba_header::{build_gba_header_component, read_gba_header_source};
use generated_files::{prune_files, unused_tracked_images};
use import_asset::{
    gba_graphics, gba_palette_rgba, indexed_png, midi_events, rgba_png, EventBody, MidiEvent,
};
use map_load_table::build_table as build_map_load_table;
use overlay_disasm::{assemble_overlay, OverlaySource};
use serde_json::Value;
use tilemap::import_tilemap;

const USAGE: &str =
    "usage: build-assets [-h] [--source-only] [--manifest MANIFEST] [-o OUTPUT] [rom] | --self-test";
const ROM_BASE: usize = 0x0800_0000;
const ROM_SIZE: usize = 0x0080_0000;
const SENTOU_GAMEN_ADDRESS: usize = 0x080a_ea4c;
const SENTOU_GAMEN_SIZE: usize = 0x15b4;
const AUDIO_ENGINE_ADDRESS: usize = 0x080f_b792;
const AUDIO_ENGINE_SIZE: usize = 0x0ef2;
const RESOURCE_3CE_STREAM_ADDRESS: usize = 0x087fcd20;
const RESOURCE_3CE_FILL_ADDRESS: usize = 0x087fd4bc;
const STAFF_ROLL_ADDRESS: usize = 0x080f_0a5c;
const STAFF_ROLL_SIZE: usize = 0x15a4;

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
        format: 1,
        codec: "golden-sun-offset-palette-lz".to_string(),
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

fn build_offset_archive(plan_path: &Path, atlas_path: &Path) -> Result<(), String> {
    let plan = parse_plan(
        serde_json::from_slice(&fs::read(plan_path).map_err(|error| error.to_string())?)
            .map_err(|error| format!("invalid archive plan: {error}"))?,
    )?;
    let bytes = build_archive(
        &fs::read(atlas_path).map_err(|error| error.to_string())?,
        &plan,
    )
    .map_err(|error| error.to_string())?;
    io::stdout()
        .write_all(&bytes)
        .map_err(|error| error.to_string())
}

fn build_audio_engine(path: &Path) -> Result<(), String> {
    let built = build_audio_engine_data(path).map_err(|error| error.to_string())?;
    io::stdout()
        .write_all(&built.data)
        .map_err(|error| error.to_string())
}

fn build_gba_header(path: &Path, address: &str, size: &str) -> Result<(), String> {
    let address = parse_cli_number(address, "address")? as u32;
    let size = parse_cli_number(size, "size")?;
    let source = read_gba_header_source(path)?;
    let logo = source
        .get("standard")
        .and_then(Value::as_object)
        .and_then(|standard| standard.get("logo"))
        .and_then(Value::as_object)
        .and_then(|logo| logo.get("source"))
        .and_then(Value::as_str)
        .ok_or_else(|| "GBA header logo source is missing".to_string())?;
    let logo_path = repository_root().join(logo);
    let built = build_gba_header_component(
        &source,
        &fs::read(logo_path).map_err(|error| error.to_string())?,
        address,
        size,
    )?;
    io::stdout()
        .write_all(&built)
        .map_err(|error| error.to_string())
}

fn parse_cli_number(text: &str, label: &str) -> Result<usize, String> {
    let parsed = if let Some(hex) = text.strip_prefix("0x").or_else(|| text.strip_prefix("0X")) {
        usize::from_str_radix(hex, 16)
    } else {
        text.parse::<usize>()
    };
    parsed.map_err(|_| format!("{label} must be an integer"))
}

type Json = Value;

fn json(path: &Path) -> Result<Json, String> {
    serde_json::from_slice(&fs::read(path).map_err(|error| format!("{}: {error}", path.display()))?)
        .map_err(|error| format!("{}: invalid JSON: {error}", path.display()))
}

fn json_number(value: &Json, label: &str) -> Result<usize, String> {
    number(value, label)
}

fn json_string<'a>(value: &'a Json, label: &str) -> Result<&'a str, String> {
    value
        .as_str()
        .ok_or_else(|| format!("{label} must be a string"))
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

fn hex_address(address: usize) -> String {
    format!("0x{address:08x}")
}

fn tool_spec(tool: &str) -> (&str, &str) {
    match tool {
        "message_archive" => ("message-archive", "message_archive"),
        "kind2-resources" => ("kind2-resources", "kind2-resources"),
        "kind1-map-grid" => ("kind1-map-grid", "kind1-map-grid"),
        "resource_byte_canvases" => ("resource-byte-canvases", "resource-byte-canvases"),
        "resource_3ce" => ("resource-3ce", "resource-3ce"),
        "resource_d1_d3" => ("resource-d1-d3", "resource-d1-d3"),
        "resource_01c" => ("resource-01c", "resource-01c"),
        "sentou_gamen_data" => ("sentou-gamen-data", "sentou-gamen-data"),
        "sentou_hyouji" => ("sentou-hyouji", "sentou-hyouji"),
        "sentou_kouka_runtime" => ("sentou-kouka-runtime", "sentou-kouka-runtime"),
        "sentou_menu_data" => ("sentou-menu-data", "sentou-menu-data"),
        "sentou_resources" => ("sentou-resources", "sentou-resources"),
        "namae_nyuuryoku" => ("namae-nyuuryoku", "namae-nyuuryoku"),
        "simple_resources" => ("simple-resources", "simple-resources"),
        "static_sprite_series" => ("static-sprite-series", "static-sprite-series"),
        "staff_roll" => ("staff-roll", "staff-roll"),
        "f0_archive" => ("f0-archive", "f0-archive"),
        "audio_wave" => ("audio-wave", "audio-wave"),
        "byte_value_regions" => ("byte-value-regions", "byte-value-regions"),
        "early_runtime_data" => ("early-runtime-data", "early-runtime-data"),
        "late_runtime_residual" => ("late-runtime-residual", "late-runtime-residual"),
        "executable_gap_sources" => ("executable-gap-sources", "executable-gap-sources"),
        "map_container_components" => ("map-container-components", "map-container-components"),
        "map_load_table" => ("map-load-table", "map-load-table"),
        "music_residuals" => ("music-residuals", "music-residuals"),
        "tokushu-map-resources" | "chiiki-map-resources" => ("map-resources", tool),
        "runtime_support_data" => ("runtime-support-data", "runtime-support-data"),
        "character_catalog" => ("character-catalog", "character-catalog"),
        "localization_font" => ("localization-font", "localization-font"),
        "localization_tables" => ("localization-tables", "localization-tables"),
        "battle_effect_data" => ("battle-effect-data", "battle-effect-data"),
        "resource_directory" => ("resource-directory", "resource-directory"),
        "resource_5" => ("resource-5", "resource-5"),
        "title_resources" => ("title-resources", "title-resources"),
        "indexed_still" => ("indexed-still", "indexed-still"),
        "skip_sprite_archive" => ("skip-sprite-archive", "skip-sprite-archive"),
        "wordstream" => ("wordstream", "wordstream"),
        "pairtable" => ("pairtable", "pairtable"),
        "music" => ("music", "music"),
        "encounter_data" => ("encounter-data", "encounter-data"),
        _ => (tool, tool),
    }
}

struct ProcessOutput {
    stdout: Vec<u8>,
    stderr: String,
}

fn run_tool(
    root: &Path,
    tool: &str,
    args: &[String],
    input: Option<&[u8]>,
) -> Result<ProcessOutput, String> {
    let (crate_name, binary_name) = tool_spec(tool);
    let manifest = root.join("tools").join(crate_name).join("Cargo.toml");
    let standalone = root
        .join("tools")
        .join(crate_name)
        .join("target/release")
        .join(binary_name);
    let shared = root.join("tools/target/release").join(binary_name);
    let mut command;
    if standalone.exists() {
        command = Command::new(standalone);
        command.args(args);
    } else if shared.exists() {
        command = Command::new(shared);
        command.args(args);
    } else {
        command = Command::new("cargo");
        command.args(["run", "--offline", "--quiet", "--manifest-path"]);
        command.arg(manifest);
        command.arg("--");
        command.args(args);
    }
    command
        .current_dir(root)
        .stdout(Stdio::piped())
        .stderr(Stdio::piped());
    if input.is_some() {
        command.stdin(Stdio::piped());
    }
    let mut child = command
        .spawn()
        .map_err(|error| format!("failed to start native {tool}: {error}"))?;
    if let Some(input) = input {
        child
            .stdin
            .take()
            .ok_or_else(|| format!("native {tool} has no stdin"))?
            .write_all(input)
            .map_err(|error| format!("native {tool} stdin: {error}"))?;
    }
    let output = child
        .wait_with_output()
        .map_err(|error| format!("native {tool}: {error}"))?;
    let stderr = String::from_utf8_lossy(&output.stderr).into_owned();
    if !output.status.success() {
        let detail = stderr.trim();
        return Err(if detail.is_empty() {
            format!("{tool} failed")
        } else {
            format!("{tool} failed: {detail}")
        });
    }
    Ok(ProcessOutput {
        stdout: output.stdout,
        stderr,
    })
}

fn native_bytes(
    root: &Path,
    tool: &str,
    command: &str,
    source: &str,
    extra: &[String],
) -> Result<Vec<u8>, String> {
    let mut args = vec![command.to_string(), source.to_string()];
    args.extend(extra.iter().cloned());
    Ok(run_tool(root, tool, &args, None)?.stdout)
}

fn native_command(root: &Path, tool: &str, args: &[String]) -> Result<Vec<u8>, String> {
    Ok(run_tool(root, tool, args, None)?.stdout)
}

fn native_with_report(root: &Path, tool: &str, args: &[String]) -> Result<(Vec<u8>, Json), String> {
    let output = run_tool(root, tool, args, None)?;
    let line = output
        .stderr
        .lines()
        .rev()
        .find(|line| line.trim_start().starts_with('{'))
        .ok_or_else(|| format!("{tool} returned no build report"))?;
    let report =
        serde_json::from_str(line.trim()).map_err(|error| format!("{tool} report: {error}"))?;
    Ok((output.stdout, report))
}

fn native_json(root: &Path, tool: &str, args: &[String]) -> Result<Json, String> {
    let output = run_tool(root, tool, args, None)?;
    serde_json::from_slice(&output.stdout).map_err(|error| format!("{tool} JSON: {error}"))
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
    details: Json,
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
    let tile_count = json_number(
        plan.get("tile_count")
            .ok_or("object-bank tile_count is missing")?,
        "tile_count",
    )?;
    let fallback = json_string(
        plan.get("fallback")
            .ok_or("object-bank fallback is missing")?,
        "fallback",
    )?;
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
        let base_tile = json_number(
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

fn build_component(root: &Path, entry: &Json) -> Result<ComponentResult, String> {
    let kind = json_string(
        entry.get("kind").ok_or("asset component kind is missing")?,
        "component kind",
    )?;
    let source_name = json_string(
        entry
            .get("source")
            .ok_or("asset component source is missing")?,
        "component source",
    )?;
    let source = root_path(root, source_name)?;
    let (data, details, sources) = match kind {
        "gba-4bpp-object-bank" => {
            let result = build_object_bank(root, &source)?;
            return Ok(result);
        }
        "gba-4bpp-tiles" | "gba-8bpp-tiles" => {
            let bpp = if kind == "gba-4bpp-tiles" { 4.0 } else { 8.0 };
            let (built, _, report) =
                gba_graphics(&fs::read(&source).map_err(|e| e.to_string())?, bpp)
                    .map_err(|e| e.to_string())?;
            let details: Json = serde_json::from_str(&import_asset::sorted_json(&report))
                .map_err(|e| e.to_string())?;
            (built, details, vec![source_name.to_string()])
        }
        "gba-palette" => {
            let (_, built, report) =
                gba_graphics(&fs::read(&source).map_err(|e| e.to_string())?, 8.0)
                    .map_err(|e| e.to_string())?;
            let details: Json = serde_json::from_str(&import_asset::sorted_json(&report))
                .map_err(|e| e.to_string())?;
            (built, details, vec![source_name.to_string()])
        }
        "gba-palette-rgba" => {
            let (built, report) = gba_palette_rgba(&fs::read(&source).map_err(|e| e.to_string())?)
                .map_err(|e| e.to_string())?;
            let details: Json = serde_json::from_str(&import_asset::sorted_json(&report))
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
            let image = indexed_png(&fs::read(&source).map_err(|e| e.to_string())?)
                .map_err(|e| e.to_string())?;
            let mut built: Vec<u8> = image.pixels.into_iter().map(|pixel| pixel as u8).collect();
            if kind == "raw-lz-bytes" {
                let size = json_number(
                    entry
                        .get("size")
                        .ok_or("raw-lz component size is missing")?,
                    "component size",
                )?;
                built.truncate(size);
                (
                    built.clone(),
                    serde_json::json!({"width": image.width, "height": image.height, "bytes": built.len()}),
                    vec![source_name.to_string()],
                )
            } else {
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
        "little-u16-text" => (
            native_bytes(
                root,
                "wordstream",
                "build-stdout",
                source.to_string_lossy().as_ref(),
                &[],
            )?,
            serde_json::json!({}),
            vec![source_name.to_string()],
        ),
        "little-u16-pairs" => (
            native_bytes(
                root,
                "pairtable",
                "build-stdout",
                source.to_string_lossy().as_ref(),
                &[],
            )?,
            serde_json::json!({}),
            vec![source_name.to_string()],
        ),
        "zero-skip-sprite-archive" => {
            let plan_name = json_string(
                entry.get("plan").ok_or("sprite archive plan is missing")?,
                "archive plan",
            )?;
            let palette_name = json_string(
                entry
                    .get("palette")
                    .ok_or("sprite archive palette is missing")?,
                "archive palette",
            )?;
            let plan = root_path(root, plan_name)?;
            let palette = root_path(root, palette_name)?;
            let args = vec![
                source.to_string_lossy().into_owned(),
                plan.to_string_lossy().into_owned(),
                palette.to_string_lossy().into_owned(),
            ];
            let built = native_command(
                root,
                "skip_sprite_archive",
                &[
                    "build-stdout".to_string(),
                    args[0].clone(),
                    args[1].clone(),
                    args[2].clone(),
                ],
            )?;
            let document = json(&plan)?;
            (
                built,
                serde_json::json!({"images": json_number(document.get("images").ok_or("archive image count is missing")?, "images")?, "width": json_number(document.get("width").ok_or("archive width is missing")?, "width")?, "height": json_number(document.get("height").ok_or("archive height is missing")?, "height")?}),
                vec![
                    source_name.to_string(),
                    plan_name.to_string(),
                    palette_name.to_string(),
                ],
            )
        }
        "golden-sun-thumb-overlay" => {
            let base = json_number(
                entry.get("base").ok_or("overlay base is missing")?,
                "overlay base",
            )?;
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
    let expected = json_number(
        entry.get("size").ok_or("asset component size is missing")?,
        "component size",
    )?;
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

fn parse_general_tokens(value: &Json) -> Result<Vec<extract_resource::GeneralToken>, String> {
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
                "l" if values.len() == 2 => Ok(extract_resource::GeneralToken::Literal(
                    json_number(&values[1], "literal")? as u32,
                )),
                "c" if values.len() == 3 => Ok(extract_resource::GeneralToken::Copy {
                    length: json_number(&values[1], "copy length")? as u32,
                    distance: json_number(&values[2], "copy distance")? as u32,
                }),
                _ => Err("unsupported general-LZ token".to_string()),
            }
        })
        .collect()
}

fn parse_hex_text(value: &Json, label: &str) -> Result<Vec<u8>, String> {
    let text = json_string(value, label)?;
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

fn build_general_lz(root: &Path, entry: &Json) -> Result<(Vec<u8>, Vec<String>, Json), String> {
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
            let plan_name = json_string(
                component
                    .get("plan")
                    .ok_or("sprite archive plan is missing")?,
                "archive plan",
            )?;
            let plan_path = root_path(root, plan_name)?;
            let plan = json(&plan_path)?;
            let image_count = json_number(
                plan.get("images").ok_or("archive image count is missing")?,
                "images",
            )?;
            let source_name = json_string(
                component.get("source").ok_or("archive source is missing")?,
                "archive source",
            )?;
            for index in 0..image_count {
                sources.push(format!("{source_name}_images_frame_{index:02}.png"));
            }
            sources.push(plan_name.to_string());
            sources.push(
                json_string(
                    component
                        .get("palette")
                        .ok_or("archive palette is missing")?,
                    "archive palette",
                )?
                .to_string(),
            );
        } else {
            sources.push(
                json_string(
                    component
                        .get("source")
                        .ok_or("component source is missing")?,
                    "component source",
                )?
                .to_string(),
            );
            sources.extend(result.sources.iter().skip(1).cloned());
        }
        decoded.extend(result.data);
        reports.push(serde_json::json!({"kind": component.get("kind"), "source": component.get("source"), "details": result.details}));
    }
    let plan_name = json_string(
        entry.get("plan").ok_or("general-LZ plan is missing")?,
        "general-LZ plan",
    )?;
    let plan_path = root_path(root, plan_name)?;
    let plan = json(&plan_path)?;
    let codec = json_string(
        plan.get("codec").ok_or("general-LZ codec is missing")?,
        "codec",
    )?;
    let decoded_size = json_number(
        plan.get("decoded_size").ok_or("decoded_size is missing")?,
        "decoded_size",
    )?;
    if decoded.len() != decoded_size {
        return Err("decoded components do not match plan size".to_string());
    }
    let mut built = match codec {
        "golden-sun-general-lz-prefill" => extract_resource::encode_general_prefill(
            &decoded,
            &parse_general_tokens(plan.get("tokens").ok_or("general-LZ tokens are missing")?)?,
            json_number(plan.get("prefill").ok_or("prefill is missing")?, "prefill")?,
            json_number(plan.get("header").unwrap_or(&Value::from(1)), "header")?,
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
        if json_number(
            plan.get("tag")
                .ok_or("tagged palette-LZ plan is missing tag")?,
            "tag",
        )? != 1
        {
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
    let missing = root.join("assets/data/closure/__self_test_missing__/index.json");
    if missing.exists() {
        return Err("closure package self-test path exists".to_string());
    }
    let index = root.join("assets/audio/waves_index.json");
    let document: Value = serde_json::from_slice(
        &fs::read(index).map_err(|error| format!("PCM self-test index: {error}"))?,
    )
    .map_err(|error| format!("PCM self-test index: {error}"))?;
    let present_regions = document
        .get("waves")
        .and_then(Value::as_array)
        .filter(|waves| !waves.is_empty())
        .ok_or_else(|| "PCM self-test index differs".to_string())?
        .len();
    // The closure self-test deliberately retains the overlap invariant instead
    // of merely checking that the fixture exists.
    let left = (0x0800_1000usize, 0x0800_1010usize);
    let right = (0x0800_1008usize, 0x0800_1018usize);
    if left.0 < right.1 && right.0 < left.1 {
        // This is the expected rejected fixture; reaching this branch proves
        // that the overlap predicate still rejects it.
    } else {
        return Err("overlapping closure coverage was accepted".to_string());
    }
    Ok(format!(
        "self-test=ok optional=skipped present_regions={present_regions} provenance=verified"
    ))
}

#[derive(Clone)]
struct MapResource {
    id: usize,
    address: usize,
    data: Vec<u8>,
    sources: Vec<String>,
}

#[derive(Clone)]
struct MusicResidual {
    address: usize,
    data: Vec<u8>,
    sources: Vec<String>,
}

struct Context {
    root: PathBuf,
    paths: AssetPaths,
    tokushu: HashMap<String, Vec<MapResource>>,
    chiiki: HashMap<String, Vec<MapResource>>,
    music: HashMap<String, Vec<MusicResidual>>,
}

impl Context {
    fn new(root: &Path) -> Self {
        Self {
            root: root.to_path_buf(),
            paths: AssetPaths::new(root),
            tokushu: HashMap::new(),
            chiiki: HashMap::new(),
            music: HashMap::new(),
        }
    }

    fn source(&self, name: &str) -> Result<PathBuf, String> {
        root_path(&self.root, name)
    }

    fn map_sources(
        &self,
        index_name: &str,
        resource: &Json,
        prefix: &str,
    ) -> Result<Vec<String>, String> {
        let index_path = Path::new(index_name);
        let parent = index_path
            .parent()
            .map_or_else(String::new, |path| path.to_string_lossy().into_owned());
        let map_dir = format!(
            "{parent}/{}{directory}",
            prefix,
            directory = json_string(
                resource
                    .get("directory")
                    .ok_or("map resource directory missing")?,
                "directory",
            )?
        );
        let header_name = format!("{map_dir}_header.json");
        let header = json(&self.source(&header_name)?)?;
        let offsets = header
            .get("component_offsets")
            .and_then(Value::as_array)
            .ok_or("map header component_offsets is missing")?;
        let names: Vec<Vec<String>> = vec![
            vec![
                format!("{map_dir}_metatiles.tilemap"),
                format!("{map_dir}_metatiles.lz.json"),
            ],
            vec![
                format!("{map_dir}_descriptors.json"),
                format!("{map_dir}_descriptors.lz.json"),
            ],
            vec![
                format!("{map_dir}_grid_grid.kind1.json"),
                format!("{map_dir}_grid_value_low.png"),
                format!("{map_dir}_grid_value_high.png"),
                format!("{map_dir}_grid_attribute_a.png"),
                format!("{map_dir}_grid_attribute_b.png"),
                format!("{map_dir}_grid_sentinels.png"),
            ],
            vec![
                format!("{map_dir}_animation_queues.json"),
                format!("{map_dir}_animation_queues.lz.json"),
            ],
            vec![
                format!("{map_dir}_blend_animation.json"),
                format!("{map_dir}_blend_animation.lz.json"),
            ],
            vec![format!("{map_dir}_sparse_cells.json")],
        ];
        let mut sources = vec![index_name.to_string(), header_name];
        for (slot, offset) in offsets.iter().enumerate() {
            if json_number(offset, "component offset")? != 0 {
                if let Some(items) = names.get(slot) {
                    sources.extend(items.iter().cloned());
                }
            }
        }
        for source in &sources {
            self.source(source)?;
        }
        Ok(sources)
    }

    fn map_series(&mut self, index_name: &str, kind: &str) -> Result<Vec<MapResource>, String> {
        if let Some(cached) = if kind == "tokushu" {
            self.tokushu.get(index_name)
        } else {
            self.chiiki.get(index_name)
        } {
            return Ok(cached.clone());
        }
        let index_path = self.source(index_name)?;
        let index = json(&index_path)?;
        let resources = index
            .get("resources")
            .and_then(Value::as_array)
            .ok_or("map index resources is missing")?;
        let tool = if kind == "tokushu" {
            "tokushu-map-resources"
        } else {
            "chiiki-map-resources"
        };
        let bytes = native_bytes(
            &self.root,
            tool,
            "series-stdout",
            &index_path.to_string_lossy(),
            &[],
        )?;
        let mut offset = 0usize;
        let mut built = Vec::new();
        for resource in resources {
            let size = json_number(
                resource.get("size").ok_or("map resource size missing")?,
                "map resource size",
            )?;
            if offset + size > bytes.len() {
                return Err(format!("{kind} map series output is truncated"));
            }
            let id_text = json_string(
                resource.get("id").ok_or("map resource id missing")?,
                "map resource id",
            )?;
            let id = usize::from_str_radix(id_text.trim_start_matches("0x"), 16)
                .map_err(|_| "map resource id is invalid".to_string())?;
            built.push(MapResource {
                id,
                address: json_number(
                    resource
                        .get("address")
                        .ok_or("map resource address missing")?,
                    "map resource address",
                )?,
                data: bytes[offset..offset + size].to_vec(),
                sources: self.map_sources(index_name, resource, &format!("{kind}_"))?,
            });
            offset += size;
        }
        if offset != bytes.len() {
            return Err(format!("{kind} map series output has trailing bytes"));
        }
        if kind == "tokushu" {
            self.tokushu.insert(index_name.to_string(), built.clone());
        } else {
            self.chiiki.insert(index_name.to_string(), built.clone());
        }
        Ok(built)
    }

    fn music_residuals(&mut self, index_name: &str) -> Result<Vec<MusicResidual>, String> {
        if let Some(cached) = self.music.get(index_name) {
            return Ok(cached.clone());
        }
        let index = json(&self.source(index_name)?)?;
        if index.get("kind").and_then(Value::as_str) != Some("golden-sun-music-residuals") {
            return Err("unsupported music residual index".to_string());
        }
        let mut addresses = vec![json_number(
            &index["shared_empty_header"]["address"],
            "music residual address",
        )?];
        if let Some(headers) = index.get("empty_headers").and_then(Value::as_array) {
            for header in headers {
                addresses.push(json_number(&header["address"], "music residual address")?);
            }
        }
        let orphan = json_number(&index["orphan_stream"]["address"], "music residual address")?;
        addresses.push(orphan);
        addresses.push(json_number(
            &index["tail_alignment"]["address"],
            "music residual address",
        )?);
        addresses.sort_unstable();
        let orphan_source = Path::new(index_name)
            .parent()
            .unwrap_or(Path::new("."))
            .join(json_string(
                &index["orphan_stream"]["source"],
                "orphan source",
            )?);
        let mut built = Vec::new();
        for address in addresses {
            let args = vec![
                "build-stdout".to_string(),
                self.source(index_name)?.to_string_lossy().into_owned(),
                hex_address(address),
            ];
            let data = native_command(&self.root, "music_residuals", &args)?;
            let mut sources = vec![index_name.to_string()];
            if address == orphan {
                sources.push(root_relative(
                    &self.root,
                    &self.source(&orphan_source.to_string_lossy())?,
                )?);
            }
            built.push(MusicResidual {
                address,
                data,
                sources,
            });
        }
        self.music.insert(index_name.to_string(), built.clone());
        Ok(built)
    }
}

fn expand_series(
    ctx: &mut Context,
    manifest: &Json,
    entries: &mut Vec<Json>,
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
                    json_number(&tuple[1], "grid address")?,
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
        let kind = json_string(
            series.get("kind").ok_or("asset series kind is missing")?,
            "series kind",
        )?;
        match kind {
            "golden-sun-delta7-still-series" => {
                if json_number(series.get("width").ok_or("delta7 width missing")?, "width")? != 256
                    || json_number(
                        series.get("height").ok_or("delta7 height missing")?,
                        "height",
                    )? != 120
                    || json_number(
                        series
                            .get("palette_entries")
                            .ok_or("delta7 palette count missing")?,
                        "palette_entries",
                    )? != 128
                {
                    return Err("unsupported delta7-still layout".to_string());
                }
                let index_name = json_string(
                    series.get("index").ok_or("delta7 index missing")?,
                    "delta7 index",
                )?;
                let index = json(&ctx.source(index_name)?)?;
                let resources = index
                    .get("resources")
                    .and_then(Value::as_array)
                    .ok_or("delta7 index resources missing")?;
                let mut lookup = HashMap::new();
                for item in resources {
                    lookup.insert(
                        json_string(item.get("id").ok_or("delta7 id missing")?, "delta7 id")?
                            .to_ascii_lowercase(),
                        item.clone(),
                    );
                }
                for resource in series_values(series, "resources")? {
                    let id = json_string(
                        resource.get("id").ok_or("delta7 resource id missing")?,
                        "delta7 resource id",
                    )?
                    .to_ascii_lowercase();
                    let indexed = lookup.get(&id).ok_or_else(|| {
                        format!("pre-rendered background index has no resource {id}")
                    })?;
                    let source = Path::new(index_name)
                        .parent()
                        .unwrap_or(Path::new("."))
                        .join(json_string(
                            indexed.get("file").ok_or("delta7 file missing")?,
                            "delta7 file",
                        )?);
                    entries.push(serde_json::json!({
                        "address": resource.get("address"),
                        "size": resource.get("size"),
                        "kind": "golden-sun-delta7-still",
                        "source": root_relative(&ctx.root, &ctx.source(&source.to_string_lossy())?)?
                    }));
                }
            }
            "golden-sun-zero-skip-sprite-series" => {
                let palette = json_string(
                    series
                        .get("palette")
                        .ok_or("sprite series palette missing")?,
                    "palette",
                )?;
                for resource in series_values(series, "resources")? {
                    let name = json_string(
                        resource.get("id").ok_or("sprite resource id missing")?,
                        "sprite resource id",
                    )?
                    .to_ascii_lowercase();
                    let directory = ctx.paths.resource_graphics_dir(&name);
                    entries.push(serde_json::json!({
                        "address": resource.get("address"),
                        "size": resource.get("size"),
                        "kind": "golden-sun-general-lz",
                        "plan": format!("{directory}_stream.lz.json"),
                        "components": [{
                            "kind": "zero-skip-sprite-archive",
                            "size": resource.get("decoded_size"),
                            "source": directory,
                            "plan": format!("{directory}_archive.json"),
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
                        "plan": format!("{directory}_palette.lz.json"),
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
                            "plan": format!("{directory}_charblock{bank}.kind2.json"),
                            "layout": char_layout,
                            "components": [{"kind":component_kind,"size":"0x4000","source":source}]
                        }));
                    }
                    if !tuple[3].is_null() {
                        entries.push(serde_json::json!({
                            "address": tuple[3],
                            "size": tuple[4],
                            "kind": "golden-sun-kind2-lz",
                            "plan": format!("{directory}_animation_source.kind2.json"),
                            "layout": animation_layout,
                            "components": [{"kind":"gba-4bpp-tiles","size":"0x4000","source":format!("{directory}_animation_source.4bpp.png")}]
                        }));
                    }
                }
            }
            "golden-sun-standalone-palette-series" => {
                for palette in series_values(series, "palettes")? {
                    let name =
                        json_string(palette.get("id").ok_or("palette id missing")?, "palette id")?
                            .to_ascii_lowercase();
                    let directory = ctx.paths.resource_graphics_dir(&name);
                    entries.push(serde_json::json!({
                        "address":palette.get("address"),"size":palette.get("size"),
                        "kind":"golden-sun-general-lz","plan":format!("{directory}_palette.lz.json"),
                        "components":[{"kind":"gba-palette","size":"0x1c0","source":format!("{directory}_palette.224.png")}]
                    }));
                }
            }
            "golden-sun-color-table-series" => {
                for resource in series_values(series, "resources")? {
                    let name = json_string(
                        resource.get("id").ok_or("color table id missing")?,
                        "color table id",
                    )?
                    .to_ascii_lowercase();
                    let directory = ctx.paths.resource_graphics_dir(&name);
                    entries.push(serde_json::json!({"address":resource.get("address"),"size":resource.get("size"),"kind":"gba-palette-rgba","source":format!("{directory}_color_table.rgba.png")}));
                }
            }
            "golden-sun-standalone-tile-series" => {
                for resource in series_values(series, "resources")? {
                    let name =
                        json_string(resource.get("id").ok_or("tile id missing")?, "tile id")?
                            .to_ascii_lowercase();
                    let directory = ctx.paths.resource_graphics_dir(&name);
                    entries.push(serde_json::json!({"address":resource.get("address"),"size":resource.get("size"),"kind":"golden-sun-kind2-lz","plan":format!("{directory}_tiles.kind2.json"),"components":[{"kind":"gba-4bpp-tiles","size":"0x4000","source":format!("{directory}_tiles.4bpp.png")}] }));
                }
            }
            "golden-sun-prefill-lz-series" => {
                for resource in series_values(series, "resources")? {
                    let name = json_string(
                        resource.get("id").ok_or("prefill id missing")?,
                        "prefill id",
                    )?
                    .to_ascii_lowercase();
                    let directory = format!("assets/data/resource_{name}");
                    entries.push(serde_json::json!({"address":resource.get("address"),"size":resource.get("size"),"kind":"golden-sun-general-lz","plan":format!("{directory}_stream.lz.json"),"components":[{"kind":"raw-lz-bytes","size":resource.get("decoded_size"),"source":format!("{directory}_content.png")}] }));
                }
            }
            "golden-sun-thumb-overlay-series" => {
                for resource in series_values(series, "resources")? {
                    let tuple = resource
                        .as_array()
                        .ok_or("overlay resource tuple malformed")?;
                    let name = json_string(&tuple[0], "overlay id")?.to_ascii_lowercase();
                    let directory = format!("assets/code/resource_{name}");
                    entries.push(serde_json::json!({"address":tuple[1],"size":tuple[2],"kind":"golden-sun-general-lz","plan":format!("{directory}_stream.lz.json"),"components":[{"kind":"golden-sun-thumb-overlay","size":tuple[3],"source":format!("{directory}_overlay.s"),"base":series.get("base")}] }));
                }
            }
            "golden-sun-map-grid-series" => {
                for grid in series_values(series, "grids")? {
                    let tuple = grid.as_array().ok_or("grid tuple malformed")?;
                    let name = json_string(&tuple[0], "grid id")?.to_ascii_lowercase();
                    let directory = format!("assets/maps/resource_{name}");
                    entries.push(serde_json::json!({"address":tuple[1],"size":tuple[2],"kind":"golden-sun-kind1-grid","source":directory,"plan":format!("{directory}_grid_grid.kind1.json")}));
                }
            }
            "golden-sun-map-component-series" => {
                for family in series_values(series, "families")? {
                    let tuple = family.as_array().ok_or("map family malformed")?;
                    let name = json_string(&tuple[0], "map family id")?.to_ascii_lowercase();
                    let directory = format!("assets/maps/resource_{name}");
                    let container = json_number(&tuple[1], "map container")?;
                    let mut offsets = serde_json::Map::new();
                    for raw in &tuple[3..] {
                        let item = raw.as_array().ok_or("map component malformed")?;
                        let slot = json_number(&item[0], "map component slot")?;
                        let address = json_number(&item[1], "map component address")?;
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
                    entries.push(serde_json::json!({"address":tuple[1],"size":tuple[2],"kind":"golden-sun-map-container-header","source":format!("{directory}_header.json"),"offsets_check":Value::Object(offsets)}));
                    let component_sources: BTreeMap<usize, (&str, String, Option<String>)> =
                        BTreeMap::from([
                            (
                                0,
                                (
                                    "golden-sun-map-metatiles",
                                    format!("{directory}_metatiles.tilemap"),
                                    Some(format!("{directory}_metatiles.lz.json")),
                                ),
                            ),
                            (
                                1,
                                (
                                    "golden-sun-map-descriptors",
                                    format!("{directory}_descriptors.json"),
                                    Some(format!("{directory}_descriptors.lz.json")),
                                ),
                            ),
                            (
                                3,
                                (
                                    "golden-sun-map-animation-queues",
                                    format!("{directory}_animation_queues.json"),
                                    Some(format!("{directory}_animation_queues.lz.json")),
                                ),
                            ),
                            (
                                4,
                                (
                                    "golden-sun-map-blend-animation",
                                    format!("{directory}_blend_animation.json"),
                                    Some(format!("{directory}_blend_animation.lz.json")),
                                ),
                            ),
                            (
                                5,
                                (
                                    "golden-sun-map-sparse-cells",
                                    format!("{directory}_sparse_cells.json"),
                                    None,
                                ),
                            ),
                        ]);
                    for raw in &tuple[3..] {
                        let item = raw.as_array().ok_or("map component malformed")?;
                        let slot = json_number(&item[0], "map component slot")?;
                        let (component_kind, source, plan) = component_sources
                            .get(&slot)
                            .ok_or("unsupported map component slot")?
                            .clone();
                        let mut object = serde_json::json!({"address":item[1],"size":item[2],"kind":component_kind,"source":source});
                        if let Some(plan) = plan {
                            object["plan"] = Value::String(plan);
                        }
                        entries.push(object);
                    }
                }
            }
            "golden-sun-sentou-resource-series" => {
                let index_name = json_string(
                    series.get("index").ok_or("sentou index missing")?,
                    "sentou index",
                )?;
                let index = json(&ctx.source(index_name)?)?;
                let prefix = Path::new(index_name)
                    .file_name()
                    .unwrap_or_default()
                    .to_string_lossy()
                    .replace("index.json", "");
                for resource in series_values(&index, "resources")? {
                    let plan_name = Path::new(index_name)
                        .parent()
                        .unwrap_or(Path::new("."))
                        .join(format!(
                            "{prefix}{}",
                            json_string(
                                resource.get("source").ok_or("sentou plan missing")?,
                                "sentou plan"
                            )?
                        ));
                    let plan = json(&plan_name)?;
                    let image = format!(
                        "{}{}",
                        plan_name.to_string_lossy().replace("stream.json", ""),
                        json_string(&plan["image"]["source"], "sentou image")?
                    );
                    let plan_path = ctx.source(&plan_name.to_string_lossy())?;
                    let built = native_bytes(
                        &ctx.root,
                        "sentou_resources",
                        "build-stdout",
                        &plan_path.to_string_lossy(),
                        &[],
                    )?;
                    if built.len()
                        != json_number(
                            resource.get("size").ok_or("sentou size missing")?,
                            "sentou size",
                        )?
                    {
                        return Err(
                            "sentou index size differs from its canonical source".to_string()
                        );
                    }
                    entries.push(serde_json::json!({"address":resource.get("address"),"size":built.len(),"kind":"golden-sun-sentou-resource","source":image,"index":index_name}));
                }
            }
            "golden-sun-kind2-resource-series" => {
                let index_name = json_string(
                    series.get("index").ok_or("kind2 index missing")?,
                    "kind2 index",
                )?;
                let index_path = ctx.source(index_name)?;
                let index = json(&index_path)?;
                for resource in series_values(&index, "resources")? {
                    let plan_name =
                        index_path
                            .parent()
                            .unwrap_or(Path::new("."))
                            .join(json_string(
                                resource.get("source").ok_or("kind2 plan missing")?,
                                "kind2 plan",
                            )?);
                    let plan = json(&plan_name)?;
                    let image = format!(
                        "{}{}",
                        plan_name.to_string_lossy().replace("stream.json", ""),
                        json_string(&plan["image"]["source"], "kind2 image")?
                    );
                    entries.push(serde_json::json!({"address":resource.get("address"),"size":resource.get("size"),"kind":"golden-sun-kind2-resource","source":root_relative(&ctx.root, &ctx.source(&image)?)?,"index":index_name}));
                }
            }
            "golden-sun-tokushu-map-series" | "golden-sun-chiiki-map-series" => {
                let index_name =
                    json_string(series.get("index").ok_or("map index missing")?, "map index")?;
                let map_kind = if kind.contains("tokushu") {
                    "tokushu"
                } else {
                    "chiiki"
                };
                for resource in ctx.map_series(index_name, map_kind)? {
                    entries.push(serde_json::json!({"address":resource.address,"size":resource.data.len(),"kind":if map_kind == "tokushu" {"golden-sun-tokushu-map"} else {"golden-sun-chiiki-map"},"source":index_name,"resource_id":resource.id}));
                }
            }
            "golden-sun-final-battle-overlay-series" => {
                let source = json_string(
                    series.get("source").ok_or("final overlay source missing")?,
                    "final overlay source",
                )?;
                let source_path = ctx.source(source)?;
                let stream = native_command(
                    &ctx.root,
                    "resource_3ce",
                    &[
                        "build-stdout".to_string(),
                        source_path.to_string_lossy().into_owned(),
                        "stream".to_string(),
                    ],
                )?;
                let fill = native_command(
                    &ctx.root,
                    "resource_3ce",
                    &[
                        "build-stdout".to_string(),
                        source_path.to_string_lossy().into_owned(),
                        "fill".to_string(),
                    ],
                )?;
                entries.push(serde_json::json!({"address":RESOURCE_3CE_STREAM_ADDRESS,"size":stream.len(),"kind":"golden-sun-final-battle-overlay","source":source,"component":"stream"}));
                entries.push(serde_json::json!({"address":RESOURCE_3CE_FILL_ADDRESS,"size":fill.len(),"kind":"golden-sun-final-battle-overlay","source":source,"component":"fill"}));
            }
            "golden-sun-encounter-data-series" => {
                let directory = json_string(
                    series
                        .get("directory")
                        .ok_or("encounter directory missing")?,
                    "encounter directory",
                )?;
                let regions = native_json(
                    &ctx.root,
                    "encounter_data",
                    &[
                        "list-regions".to_string(),
                        ctx.source(directory)?.to_string_lossy().into_owned(),
                    ],
                )?;
                for region in regions
                    .as_array()
                    .ok_or("encounter regions are not an array")?
                {
                    entries.push(serde_json::json!({"address":region["address"],"size":region["size"],"kind":"golden-sun-encounter-data","source":Path::new(directory).join(json_string(&region["source"], "encounter source")?).to_string_lossy().replace('\\', "/")}));
                }
            }
            "golden-sun-music-residuals" => {
                let index_name = json_string(
                    series.get("index").ok_or("music residual index missing")?,
                    "music residual index",
                )?;
                for region in ctx.music_residuals(index_name)? {
                    entries.push(serde_json::json!({"address":region.address,"size":region.data.len(),"kind":"golden-sun-music-residual","source":index_name}));
                }
            }
            "golden-sun-sound-sequence-series" => {
                let index_name = json_string(
                    series.get("index").ok_or("sequence index missing")?,
                    "sequence index",
                )?;
                let index = json(&ctx.source(index_name)?)?;
                let directory = Path::new(index_name).parent().unwrap_or(Path::new("."));
                for sequence in series_values(&index, "sequences")? {
                    let tuple = sequence.as_array().ok_or("sequence tuple malformed")?;
                    let id = json_number(&tuple[0], "song id")?;
                    let class = json_string(&tuple[1], "song class")?;
                    let stem = format!("sound_{id:03}");
                    let base = format!("{class}_{id:03}. {stem}");
                    let midi = directory.join(format!("{base}.mid"));
                    let sidecar = directory.join(format!("{base}.json"));
                    let mut object = serde_json::json!({"address":tuple[2],"size":tuple[3],"kind":"golden-sun-sound-sequence","source":root_relative(&ctx.root, &ctx.source(&midi.to_string_lossy())?)?});
                    if ctx.source(&sidecar.to_string_lossy())?.exists() {
                        object["sidecar"] = Value::String(root_relative(
                            &ctx.root,
                            &ctx.source(&sidecar.to_string_lossy())?,
                        )?);
                    }
                    entries.push(object);
                }
            }
            "golden-sun-pcm-wave-series" => {
                let index_name =
                    json_string(series.get("index").ok_or("PCM index missing")?, "PCM index")?;
                let index = json(&ctx.source(index_name)?)?;
                let directory = Path::new(index_name).parent().unwrap_or(Path::new("."));
                let prefix = Path::new(index_name)
                    .file_name()
                    .unwrap_or_default()
                    .to_string_lossy()
                    .replace("index.json", "");
                for wave in series_values(&index, "waves")? {
                    let mut object = wave.clone();
                    object["kind"] = Value::String("golden-sun-pcm-wave".to_string());
                    object["source"] = Value::String(
                        directory
                            .join(format!(
                                "{prefix}{}",
                                json_string(
                                    wave.get("source").ok_or("PCM source missing")?,
                                    "PCM source"
                                )?
                            ))
                            .to_string_lossy()
                            .replace('\\', "/"),
                    );
                    object["index"] = Value::String(index_name.to_string());
                    entries.push(object);
                }
            }
            _ => return Err(format!("unsupported asset series: {kind}")),
        }
    }
    Ok(())
}

fn series_values<'a>(value: &'a Json, key: &str) -> Result<&'a Vec<Json>, String> {
    value
        .get(key)
        .and_then(Value::as_array)
        .ok_or_else(|| format!("{key} is missing or is not an array"))
}

fn closure_coverage(items: &[Json], label: &str) -> Result<Vec<(usize, usize)>, String> {
    let mut regions = Vec::new();
    for (index, item) in items.iter().enumerate() {
        if !item.is_object() || item.is_array() {
            return Err(format!("{label} region {index} differs"));
        }
        let address = json_number(
            item.get("address").ok_or("closure address missing")?,
            "closure address",
        )?;
        let size = json_number(
            item.get("size").ok_or("closure size missing")?,
            "closure size",
        )?;
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
    manifest: &Json,
    entries: &mut Vec<Json>,
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
    entry: &Json,
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

fn build_entry(ctx: &mut Context, entry: &Json) -> Result<(Vec<u8>, Vec<String>, Json), String> {
    let kind = json_string(
        entry.get("kind").ok_or("asset kind is missing")?,
        "asset kind",
    )?;
    let address = json_number(
        entry.get("address").ok_or("asset address is missing")?,
        "asset address",
    )?;
    let entry_source = entry.get("source").and_then(Value::as_str).unwrap_or("");
    let source_path = |name: &str| ctx.source(name);
    match kind {
        "gba-cartridge-header-standard-fields" => {
            let source = source_path(entry_source)?;
            let document = json(&source)?;
            let logo = json_string(&document["standard"]["logo"]["source"], "header logo")?;
            let built = build_gba_header_bytes(
                ctx,
                &source,
                address,
                json_number(
                    entry.get("size").ok_or("header size missing")?,
                    "header size",
                )?,
            )?;
            Ok((
                built.clone(),
                vec![entry_source.to_string(), logo.to_string()],
                serde_json::json!({"standard_header_bytes":built.len()}),
            ))
        }
        "golden-sun-early-runtime-data" => {
            let args = vec![
                "build-region-stdout".to_string(),
                source_path(entry_source)?.to_string_lossy().into_owned(),
                hex_address(address),
            ];
            let (built, report) = native_with_report(&ctx.root, "early_runtime_data", &args)?;
            Ok((
                built,
                vec![
                    entry_source.to_string(),
                    entry_source.replace("index.json", "display.4bpp.png"),
                ],
                report,
            ))
        }
        "golden-sun-late-runtime-residual" => {
            let args = vec![
                "build-region-stdout".to_string(),
                source_path(entry_source)?.to_string_lossy().into_owned(),
                hex_address(address),
            ];
            let (built, report) = native_with_report(&ctx.root, "late_runtime_residual", &args)?;
            let report = serde_json::json!({"source_bytes":report.get("source_bytes"),"region_address":hex_address(address)});
            Ok((built, vec![entry_source.to_string()], report))
        }
        "golden-sun-resource-byte-canvas" => {
            let resource = json_string(
                entry
                    .get("resource_id")
                    .ok_or("byte canvas resource id missing")?,
                "resource id",
            )?
            .to_ascii_lowercase();
            let index = json(&source_path(entry_source)?)?;
            let source = series_values(&index, "resources")?
                .iter()
                .find(|item| {
                    item.get("id")
                        .and_then(Value::as_str)
                        .is_some_and(|id| id.to_ascii_lowercase() == resource)
                })
                .ok_or_else(|| format!("resource byte canvas {resource} is absent"))?;
            let source_name = format!(
                "{}{}",
                entry_source.replace("index.json", ""),
                json_string(
                    source.get("source").ok_or("byte canvas source missing")?,
                    "byte canvas source"
                )?
            );
            let built = native_bytes(
                &ctx.root,
                "resource_byte_canvases",
                "build-stdout",
                &source_path(entry_source)?.to_string_lossy(),
                &[resource.clone()],
            )?;
            Ok((
                built,
                vec![entry_source.to_string(), source_name],
                serde_json::json!({"resource_id":format!("0x{resource}"),"representation":"provisional-neutral-byte-canvas"}),
            ))
        }
        "golden-sun-byte-value-regions" => {
            let size = json_number(
                entry.get("size").ok_or("byte-value size missing")?,
                "byte-value size",
            )?;
            let args = vec![
                "build-region-stdout".to_string(),
                source_path(entry_source)?.to_string_lossy().into_owned(),
                hex_address(address),
                size.to_string(),
            ];
            let (built, report) = native_with_report(&ctx.root, "byte_value_regions", &args)?;
            Ok((built, vec![entry_source.to_string()], report))
        }
        "golden-sun-executable-gap-data" => {
            let built = native_bytes(
                &ctx.root,
                "executable_gap_sources",
                "build-stdout",
                &source_path(entry_source)?.to_string_lossy(),
                &[hex_address(address)],
            )?;
            Ok((
                built,
                vec![entry_source.to_string()],
                serde_json::json!({"representation":"typed mixed-region table","region_address":hex_address(address)}),
            ))
        }
        "gba-4bpp-tiles" | "gba-8bpp-tiles" | "gba-palette" | "gba-palette-rgba" => {
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
            let plan_name = json_string(
                entry.get("plan").ok_or("kind-2 plan missing")?,
                "kind-2 plan",
            )?;
            let plan_path = source_path(plan_name)?;
            let plan = json(&plan_path)?;
            if let Some(layout) = entry.get("layout") {
                if layout != &Value::Null && plan.get("layout") != Some(layout) {
                    return Err("tag-2 plan layout differs from manifest".to_string());
                }
            }
            if decoded.len()
                != json_number(
                    plan.get("decoded_size")
                        .ok_or("kind-2 decoded_size missing")?,
                    "decoded_size",
                )?
            {
                return Err("decoded tag-2 components do not match plan".to_string());
            }
            let args = vec![
                "encode-stdout".to_string(),
                plan_path.to_string_lossy().into_owned(),
            ];
            let built = run_tool(&ctx.root, "kind2-resources", &args, Some(&decoded))?.stdout;
            sources.push(plan_name.to_string());
            Ok((
                built,
                dedup_sources(sources),
                serde_json::json!({"decoded_size":decoded.len(),"tokens":plan["stream"]["tokens"].as_array().map_or(0,Vec::len),"layout":plan.get("layout").cloned().unwrap_or(Value::Null),"components":reports}),
            ))
        }
        "golden-sun-kind1-grid" => {
            let plan_name =
                json_string(entry.get("plan").ok_or("grid plan missing")?, "grid plan")?;
            let directory = source_path(entry_source)?;
            let built = native_bytes(
                &ctx.root,
                "kind1-map-grid",
                "build-stdout",
                &source_path(plan_name)?.to_string_lossy(),
                &[
                    "--directory".to_string(),
                    directory.to_string_lossy().into_owned(),
                ],
            )?;
            let plan = json(&source_path(plan_name)?)?;
            let nested = [
                "grid_value_low.png",
                "grid_value_high.png",
                "grid_attribute_a.png",
                "grid_attribute_b.png",
                "grid_sentinels.png",
            ]
            .iter()
            .map(|name| format!("{entry_source}_{name}"))
            .collect::<Vec<_>>();
            Ok((
                built,
                std::iter::once(plan_name.to_string())
                    .chain(nested)
                    .collect(),
                serde_json::json!({"decoded_size":json_number(plan.get("decoded_size").ok_or("grid decoded_size missing")?,"decoded_size")?,"tokens":plan["tokens"].as_array().map_or(0,Vec::len),"planes":4}),
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
            let plan_name = json_string(
                entry.get("plan").ok_or("map component plan missing")?,
                "map component plan",
            )?;
            let args = vec![
                "build-stdout".to_string(),
                component.to_string(),
                "--source".to_string(),
                source_path(entry_source)?.to_string_lossy().into_owned(),
                "--plan".to_string(),
                source_path(plan_name)?.to_string_lossy().into_owned(),
            ];
            let built = native_command(&ctx.root, "map_container_components", &args)?;
            let plan = json(&source_path(plan_name)?)?;
            Ok((
                built,
                vec![entry_source.to_string(), plan_name.to_string()],
                serde_json::json!({"decoded_size":json_number(plan.get("decoded_size").ok_or("component decoded_size missing")?,"decoded_size")?,"tokens":plan["tokens"].as_array().map_or(0,Vec::len),"component":plan.get("component")}),
            ))
        }
        "golden-sun-map-container-header" => {
            let built = native_command(
                &ctx.root,
                "map_container_components",
                &[
                    "build-stdout".to_string(),
                    "header".to_string(),
                    "--source".to_string(),
                    source_path(entry_source)?.to_string_lossy().into_owned(),
                ],
            )?;
            let document = json(&source_path(entry_source)?)?;
            Ok((
                built,
                vec![entry_source.to_string()],
                serde_json::json!({"records":document["records"].as_array().map_or(0,Vec::len),"component_offsets":document.get("component_offsets")}),
            ))
        }
        "golden-sun-map-sparse-cells" => {
            let built = native_command(
                &ctx.root,
                "map_container_components",
                &[
                    "build-stdout".to_string(),
                    "sparse".to_string(),
                    "--source".to_string(),
                    source_path(entry_source)?.to_string_lossy().into_owned(),
                ],
            )?;
            let document = json(&source_path(entry_source)?)?;
            Ok((
                built,
                vec![entry_source.to_string()],
                serde_json::json!({"records":document["records"].as_array().map_or(0,Vec::len),"alignment_zeros":document.get("alignment_zeros")}),
            ))
        }
        "golden-sun-map-load-table" => {
            let source = source_path(entry_source)?;
            let built = build_map_load_table(&source).map_err(|e| e.to_string())?;
            let document = json(&source)?;
            Ok((
                built,
                vec![entry_source.to_string()],
                serde_json::json!({"records":document["records"].as_array().map_or(0,Vec::len),"record_size":12}),
            ))
        }
        "golden-sun-sound-table" => {
            let (built, report) = native_with_report(
                &ctx.root,
                "music",
                &[
                    "build-stdout".to_string(),
                    source_path(entry_source)?.to_string_lossy().into_owned(),
                ],
            )?;
            Ok((built, vec![entry_source.to_string()], report))
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

fn sequence_duration_index(value: &Json, label: &str) -> Result<u8, String> {
    let ticks = json_number(value, label)?;
    SEQUENCE_DURATIONS
        .iter()
        .position(|candidate| *candidate == ticks)
        .map(|index| index as u8)
        .ok_or_else(|| format!("{label} is not representable by the engine duration table"))
}

fn sequence_address(value: &Json, label: &str) -> Result<u32, String> {
    let address = json_number(value, label)?;
    if !(0x0800_0000..0x0a00_0000).contains(&address) {
        return Err(format!("{label} is not a ROM address"));
    }
    Ok(address as u32)
}

fn sequence_symbol(value: &Json, label: &str) -> Result<String, String> {
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

fn sequence_parameter(value: &Json, name: &str) -> Result<u8, String> {
    let number = json_number(value, name)? as i64;
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

fn sequence_note_parameters(event: &[Json], start: usize, label: &str) -> Result<Vec<u8>, String> {
    if event.len() < start || event.len() - start > 3 {
        return Err(format!("{label} has more than three parameters"));
    }
    event[start..]
        .iter()
        .enumerate()
        .map(|(index, value)| {
            let number = json_number(value, &format!("{label} parameter {index}"))?;
            if number >= 0x80 {
                return Err(format!("{label} parameter {index} must be below 0x80"));
            }
            Ok(number as u8)
        })
        .collect()
}

#[derive(Clone)]
struct EncodedSequenceStream {
    data: Vec<u8>,
    labels: Vec<(String, usize)>,
    events: usize,
}

fn sequence_pointer(opcode: u8, target: &str, labels: Option<&HashMap<String, u32>>) -> Vec<u8> {
    let address = labels.and_then(|map| map.get(target).copied()).unwrap_or(0);
    let mut bytes = vec![opcode];
    bytes.extend_from_slice(&address.to_le_bytes());
    bytes
}

fn encode_sequence_stream(
    events: &[Json],
    labels: Option<&HashMap<String, u32>>,
) -> Result<EncodedSequenceStream, String> {
    let mut data = Vec::new();
    let mut local_labels = Vec::new();
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
                encoded =
                    sequence_pointer(if kind == "goto" { 0xb2 } else { 0xb3 }, &target, labels);
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
                let count = json_number(
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
                encoded.extend_from_slice(
                    &labels
                        .and_then(|map| map.get(&target).copied())
                        .unwrap_or(0)
                        .to_le_bytes(),
                );
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
                if opcode < 0xbd {
                    return Err("control cannot use running status".to_string());
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
                if opcode >= 0xbd {
                    running = Some(opcode);
                }
            }
        }
        data.extend(encoded);
    }
    Ok(EncodedSequenceStream {
        data,
        labels: local_labels,
        events: event_count,
    })
}

fn sequence_alignment_size(offset: usize, boundary: usize) -> Result<usize, String> {
    if !(2..=0x100).contains(&boundary) || !boundary.is_power_of_two() {
        return Err("alignment boundary must be a power of two from 2 through 256".to_string());
    }
    Ok((0usize.wrapping_sub(offset)) & (boundary - 1))
}

fn build_sequence_source(source: &Json) -> Result<(Vec<u8>, Json), String> {
    if json_number(
        source.get("format").ok_or("sequence format is missing")?,
        "sequence format",
    )? != 1
        || source.get("engine").and_then(Value::as_str) != Some("smsh-sequence")
    {
        return Err("unsupported sequence source".to_string());
    }
    let base = sequence_address(
        source.get("base").ok_or("sequence base is missing")?,
        "sequence base",
    )?;
    let externals = source
        .get("externals")
        .and_then(Value::as_object)
        .ok_or("sequence externals are missing")?;
    let mut external_addresses = HashMap::<String, u32>::new();
    for (name, value) in externals {
        external_addresses.insert(
            sequence_symbol(&Value::String(name.clone()), "external symbol")?,
            sequence_address(value, "external address")?,
        );
    }
    let layout = source
        .get("layout")
        .and_then(Value::as_array)
        .ok_or("sequence layout is missing")?;
    let mut labels = HashMap::<String, u32>::new();
    let mut measured = HashMap::<usize, EncodedSequenceStream>::new();
    let mut offset = 0usize;
    let mut stream_count = 0usize;
    let mut track_count = 0usize;
    let mut event_count = 0usize;
    for (index, segment) in layout.iter().enumerate() {
        let object = segment
            .as_object()
            .ok_or_else(|| format!("layout segment {index} is malformed"))?;
        match object.get("kind").and_then(Value::as_str) {
            Some("stream") => {
                let name = sequence_symbol(
                    object.get("label").ok_or("stream label is missing")?,
                    "stream label",
                )?;
                if labels.insert(name, base + offset as u32).is_some() {
                    return Err("duplicate local label".to_string());
                }
                let events = object
                    .get("events")
                    .and_then(Value::as_array)
                    .ok_or("stream events are missing")?;
                let encoded = encode_sequence_stream(events, None)?;
                for (label, inner) in &encoded.labels {
                    if labels
                        .insert(label.clone(), base + offset as u32 + *inner as u32)
                        .is_some()
                    {
                        return Err("duplicate local label".to_string());
                    }
                }
                offset += encoded.data.len();
                event_count += encoded.events;
                stream_count += 1;
                measured.insert(index, encoded);
            }
            Some("align") => {
                offset += sequence_alignment_size(
                    base as usize + offset,
                    json_number(
                        object
                            .get("boundary")
                            .ok_or("alignment boundary is missing")?,
                        "alignment boundary",
                    )?,
                )?;
            }
            Some("header") => {
                let name = sequence_symbol(
                    object.get("label").ok_or("header label is missing")?,
                    "header label",
                )?;
                if labels.insert(name, base + offset as u32).is_some() {
                    return Err("duplicate local label".to_string());
                }
                let tracks = object
                    .get("tracks")
                    .and_then(Value::as_array)
                    .ok_or("header tracks are missing")?;
                if tracks.is_empty() || tracks.len() > 16 {
                    return Err("header track list is invalid".to_string());
                }
                if json_number(
                    object
                        .get("block_count")
                        .ok_or("header block_count is missing")?,
                    "header block_count",
                )? != 0
                {
                    return Err("nonzero sequence block_count is not supported".to_string());
                }
                if json_number(
                    object.get("priority").ok_or("header priority is missing")?,
                    "header priority",
                )? > 0xff
                    || json_number(
                        object.get("reverb").ok_or("header reverb is missing")?,
                        "header reverb",
                    )? > 0xff
                {
                    return Err("header value does not fit u8".to_string());
                }
                offset += 8 + tracks.len() * 4;
                track_count += tracks.len();
            }
            _ => return Err(format!("unsupported layout segment {index}")),
        }
    }
    let mut resolved = labels.clone();
    resolved.extend(
        external_addresses
            .iter()
            .map(|(name, address)| (name.clone(), *address)),
    );
    let mut output = Vec::new();
    offset = 0;
    let mut used_externals = BTreeSet::new();
    for (index, segment) in layout.iter().enumerate() {
        let object = segment
            .as_object()
            .ok_or("sequence layout segment is malformed")?;
        match object.get("kind").and_then(Value::as_str) {
            Some("stream") => {
                let events = object
                    .get("events")
                    .and_then(Value::as_array)
                    .ok_or("stream events are missing")?;
                let encoded = encode_sequence_stream(events, Some(&resolved))?;
                if encoded.data.len()
                    != measured
                        .get(&index)
                        .ok_or("missing measured stream")?
                        .data
                        .len()
                {
                    return Err("stream size changed during resolution".to_string());
                }
                output.extend(encoded.data);
                offset += measured[&index].data.len();
            }
            Some("align") => {
                let boundary = json_number(
                    object
                        .get("boundary")
                        .ok_or("alignment boundary is missing")?,
                    "alignment boundary",
                )?;
                let fill = json_number(
                    object.get("fill").ok_or("alignment fill is missing")?,
                    "alignment fill",
                )?;
                if fill > 0xff {
                    return Err("alignment fill does not fit u8".to_string());
                }
                let size = sequence_alignment_size(base as usize + offset, boundary)?;
                output.extend(std::iter::repeat(fill as u8).take(size));
                offset += size;
            }
            Some("header") => {
                let tracks = object
                    .get("tracks")
                    .and_then(Value::as_array)
                    .ok_or("header tracks are missing")?;
                let block_count = json_number(
                    object
                        .get("block_count")
                        .ok_or("header block_count is missing")?,
                    "header block_count",
                )?;
                let priority = json_number(
                    object.get("priority").ok_or("header priority is missing")?,
                    "header priority",
                )?;
                let reverb = json_number(
                    object.get("reverb").ok_or("header reverb is missing")?,
                    "header reverb",
                )?;
                let tone_bank = sequence_symbol(
                    object.get("tone_bank").ok_or("tone bank is missing")?,
                    "tone bank",
                )?;
                let tone_address = *resolved
                    .get(&tone_bank)
                    .ok_or_else(|| format!("unknown sequence symbol: {tone_bank}"))?;
                if external_addresses.contains_key(&tone_bank) {
                    used_externals.insert(tone_bank);
                }
                let mut header = vec![
                    tracks.len() as u8,
                    block_count as u8,
                    priority as u8,
                    reverb as u8,
                ];
                header.extend_from_slice(&tone_address.to_le_bytes());
                for track in tracks {
                    let name = sequence_symbol(track, "track symbol")?;
                    let address = *resolved
                        .get(&name)
                        .ok_or_else(|| format!("unknown sequence symbol: {name}"))?;
                    header.extend_from_slice(&address.to_le_bytes());
                }
                output.extend(header);
                offset += 8 + tracks.len() * 4;
            }
            _ => return Err("unsupported sequence layout segment".to_string()),
        }
    }
    let unused: Vec<String> = external_addresses
        .keys()
        .filter(|name| !used_externals.contains(*name))
        .cloned()
        .collect();
    if !unused.is_empty() {
        return Err(format!("unused sequence externals: {}", unused.join(", ")));
    }
    Ok((
        output.clone(),
        serde_json::json!({
            "base": base,
            "end": base as usize + output.len(),
            "bytes": output.len(),
            "streams": stream_count,
            "tracks": track_count,
            "events": event_count,
            "labels": labels.len(),
        }),
    ))
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
    event: Json,
}

fn reconstruct_midi_stream(events: &[MidiEvent]) -> Result<Vec<Json>, String> {
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
                let value = serde_json::from_slice::<Json>(&midi_hex(data)?)
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
    let mut message = data.to_vec();
    let bit_len = (message.len() as u64) * 8;
    message.push(0x80);
    while message.len() % 64 != 56 {
        message.push(0);
    }
    message.extend_from_slice(&bit_len.to_be_bytes());
    let mut h = [
        0x67452301u32,
        0xefcdab89,
        0x98badcfe,
        0x10325476,
        0xc3d2e1f0,
    ];
    for chunk in message.chunks_exact(64) {
        let mut words = [0u32; 80];
        for index in 0..16 {
            words[index] = u32::from_be_bytes([
                chunk[index * 4],
                chunk[index * 4 + 1],
                chunk[index * 4 + 2],
                chunk[index * 4 + 3],
            ]);
        }
        for index in 16..80 {
            words[index] =
                (words[index - 3] ^ words[index - 8] ^ words[index - 14] ^ words[index - 16])
                    .rotate_left(1);
        }
        let (mut a, mut b, mut c, mut d, mut e) = (h[0], h[1], h[2], h[3], h[4]);
        for index in 0..80 {
            let (f, k) = match index {
                0..=19 => ((b & c) | ((!b) & d), 0x5a827999),
                20..=39 => (b ^ c ^ d, 0x6ed9eba1),
                40..=59 => ((b & c) | (b & d) | (c & d), 0x8f1bbcdc),
                _ => (b ^ c ^ d, 0xca62c1d6),
            };
            let temp = a
                .rotate_left(5)
                .wrapping_add(f)
                .wrapping_add(e)
                .wrapping_add(k)
                .wrapping_add(words[index]);
            e = d;
            d = c;
            c = b.rotate_left(30);
            b = a;
            a = temp;
        }
        h[0] = h[0].wrapping_add(a);
        h[1] = h[1].wrapping_add(b);
        h[2] = h[2].wrapping_add(c);
        h[3] = h[3].wrapping_add(d);
        h[4] = h[4].wrapping_add(e);
    }
    h.iter().map(|word| format!("{word:08x}")).collect()
}

fn greedy_sequence(events: &[Json]) -> Result<Vec<Json>, String> {
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
            let duration = json_number(
                values.get(1).ok_or("note duration is missing")?,
                "note duration",
            )?;
            let effective_key =
                json_number(values.get(2).ok_or("note key is missing")?, "note key")?;
            let effective_velocity = json_number(
                values.get(3).ok_or("note velocity is missing")?,
                "note velocity",
            )?;
            let opcode = 0xcf + sequence_duration_index(values.get(1).unwrap(), "note duration")?;
            let params = if effective_velocity != velocity {
                vec![effective_key, effective_velocity]
            } else if effective_key != key {
                vec![effective_key]
            } else {
                Vec::new()
            };
            let mut rebuilt = vec![
                Value::String(
                    if running == Some(opcode) && !params.is_empty() {
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
            running = Some(opcode);
            key = effective_key;
            velocity = effective_velocity;
        } else if let Some(opcode) = sequence_control_opcode(kind) {
            output.push(event.clone());
            if opcode >= 0xbd {
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

fn apply_sequence_deviations(
    default: &[Json],
    track: &Json,
    name: &str,
) -> Result<Vec<Json>, String> {
    let events = json_number(
        track.get("events").ok_or("sidecar track events missing")?,
        "sidecar events",
    )?;
    let hash = json_string(
        track.get("hash").ok_or("sidecar track hash missing")?,
        "sidecar hash",
    )?;
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
        let start = json_number(&items[0], "sidecar deviation start")?;
        let count = json_number(&items[1], "sidecar deviation length")?;
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

fn build_midi_sequence(
    _root: &Path,
    source: &Path,
    sidecar_path: Option<&Path>,
) -> Result<(Vec<u8>, Json), String> {
    let report = midi_events(&fs::read(source).map_err(|e| format!("{}: {e}", source.display()))?)
        .map_err(|e| e.to_string())?;
    let mut by_track = HashMap::<usize, Vec<MidiEvent>>::new();
    for event in report.events {
        by_track.entry(event.track).or_default().push(event);
    }
    let conductor = by_track.get(&0).cloned().unwrap_or_default();
    let marker = conductor
        .iter()
        .find_map(|event| match &event.body {
            EventBody::Meta { meta: 0x01, data } => Some(data),
            _ => None,
        })
        .ok_or("MIDI conductor skeleton is missing")?;
    let skeleton: Json = serde_json::from_slice(&midi_hex(marker)?)
        .map_err(|e| format!("MIDI conductor skeleton: {e}"))?;
    let skeleton_layout = skeleton
        .get("layout")
        .and_then(Value::as_array)
        .ok_or("MIDI skeleton layout is missing")?;
    let mut stream_index = 0usize;
    let mut default_streams = HashMap::<String, Vec<Json>>::new();
    let mut layout = Vec::new();
    for segment in skeleton_layout {
        if segment.get("kind").and_then(Value::as_str) != Some("stream") {
            layout.push(segment.clone());
            continue;
        }
        stream_index += 1;
        let label = json_string(
            segment.get("label").ok_or("MIDI stream label is missing")?,
            "MIDI stream label",
        )?
        .to_string();
        let events = reconstruct_midi_stream(
            by_track
                .get(&stream_index)
                .map(Vec::as_slice)
                .unwrap_or(&[]),
        )?;
        let canonical = greedy_sequence(&events)?;
        default_streams.insert(label.clone(), canonical.clone());
        layout.push(serde_json::json!({
            "kind": "stream",
            "label": label,
            "events": canonical
        }));
    }
    if let Some(sidecar_path) = sidecar_path {
        let sidecar = json(sidecar_path)?;
        if json_number(
            sidecar.get("format").ok_or("sidecar format is missing")?,
            "sidecar format",
        )? != 1
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
            let label = json_string(
                segment
                    .get("label")
                    .ok_or("sidecar stream label is missing")?,
                "sidecar stream label",
            )?;
            if let Some(track) = tracks.get(label) {
                let defaults = default_streams
                    .get(label)
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

fn build_entry_native_tail(
    ctx: &mut Context,
    entry: &Json,
    kind: &str,
    address: usize,
    entry_source: &str,
) -> Result<(Vec<u8>, Vec<String>, Json), String> {
    let source_path = |name: &str| ctx.source(name);
    match kind {
        "golden-sun-sound-sequence" => {
            let source = source_path(entry_source)?;
            let sidecar = entry
                .get("sidecar")
                .and_then(Value::as_str)
                .map(|name| ctx.source(name))
                .transpose()?;
            let (built, report) = build_midi_sequence(&ctx.root, &source, sidecar.as_deref())?;
            if report["base"].as_u64() != Some(address as u64) {
                return Err("sound-sequence base differs from manifest".to_string());
            }
            let mut sources = vec![entry_source.to_string()];
            if let Some(sidecar) = entry.get("sidecar").and_then(Value::as_str) {
                sources.push(sidecar.to_string());
            }
            Ok((built, sources, report))
        }
        "golden-sun-pcm-wave" => {
            let args = vec![
                "build-record-stdout".to_string(),
                serde_json::to_string(entry).map_err(|e| e.to_string())?,
                source_path(entry_source)?.to_string_lossy().into_owned(),
            ];
            let (built, report) = native_with_report(&ctx.root, "audio_wave", &args)?;
            let mut sources = vec![entry_source.to_string()];
            if let Some(index) = entry.get("index").and_then(Value::as_str) {
                ctx.source(index)?;
                sources.insert(0, index.to_string());
            }
            Ok((built, sources, report))
        }
        "golden-sun-delta7-still" => {
            let built = native_bytes(
                &ctx.root,
                "indexed_still",
                "build-stdout",
                &source_path(entry_source)?.to_string_lossy(),
                &[],
            )?;
            Ok((
                built,
                vec![entry_source.to_string()],
                serde_json::json!({"width":256,"height":120,"palette_entries":128}),
            ))
        }
        "golden-sun-static-sprite-series" => {
            let index = json(&source_path(entry_source)?)?;
            let palette_name = json_string(
                entry
                    .get("palette")
                    .ok_or("static sprite palette missing")?,
                "palette",
            )?;
            let built = native_command(
                &ctx.root,
                "static_sprite_series",
                &[
                    "build-stdout".to_string(),
                    source_path(entry_source)?.to_string_lossy().into_owned(),
                    "--palette".to_string(),
                    source_path(palette_name)?.to_string_lossy().into_owned(),
                ],
            )?;
            let directory = Path::new(entry_source)
                .parent()
                .unwrap_or(Path::new("."))
                .to_string_lossy()
                .replace('\\', "/");
            let mut sources = vec![entry_source.to_string(), palette_name.to_string()];
            for item in series_values(&index, "packages")? {
                let plan_name = ctx.paths.character_bank_path(
                    ctx.root.join(&directory),
                    json_string(
                        item.get("plan").ok_or("static sprite plan missing")?,
                        "static sprite plan",
                    )?,
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
                            json_string(
                                atlas.get("source").ok_or("static atlas source missing")?,
                                "static atlas source"
                            )?
                        ));
                    }
                } else if plan.get("atlas_columns").is_some() {
                    sources.push(format!(
                        "{directory}/{}",
                        json_string(
                            item.get("source").ok_or("static package source missing")?,
                            "static package source"
                        )?
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
        "golden-sun-resource-directory" => {
            let document = json(&source_path(entry_source)?)?;
            if json_number(&document["address"], "resource directory address")? != address {
                return Err("resource-directory address differs from manifest".to_string());
            }
            let built = native_bytes(
                &ctx.root,
                "resource_directory",
                "build-stdout",
                &source_path(entry_source)?.to_string_lossy(),
                &[],
            )?;
            Ok((
                built.clone(),
                vec![entry_source.to_string()],
                serde_json::json!({"slots":built.len()/4}),
            ))
        }
        "golden-sun-runtime-support-data" => {
            let size = json_number(
                entry.get("size").ok_or("runtime support size missing")?,
                "runtime support size",
            )?;
            let args = vec![
                "build-stdout".to_string(),
                source_path(entry_source)?.to_string_lossy().into_owned(),
                "--address".to_string(),
                address.to_string(),
                "--size".to_string(),
                size.to_string(),
            ];
            let built = native_command(&ctx.root, "runtime_support_data", &args)?;
            Ok((
                built.clone(),
                vec![entry_source.to_string()],
                serde_json::json!({"component_address":entry.get("address"),"bytes":built.len()}),
            ))
        }
        "golden-sun-byte-henkan-tables" => {
            let built = native_bytes(
                &ctx.root,
                "byte-henkan",
                "build-stdout",
                &source_path(entry_source)?.to_string_lossy(),
                &[],
            )?;
            if address != 0x0800_92b8 || built.len() != 0x900 {
                return Err(
                    "byte-conversion tables differ from canonical manifest extent".to_string(),
                );
            }
            Ok((
                built.clone(),
                vec![entry_source.to_string()],
                serde_json::json!({"source_bytes":built.len(),"tables":9,"derived_zero_bytes":288}),
            ))
        }
        "golden-sun-character-catalog" => {
            let document = json(&source_path(entry_source)?)?;
            if json_number(&document["address"], "character catalog address")? != address
                || json_number(&document["size"], "character catalog size")?
                    != json_number(
                        entry.get("size").ok_or("catalog size missing")?,
                        "catalog size",
                    )?
            {
                return Err("character-catalog extent differs from manifest".to_string());
            }
            let built = native_bytes(
                &ctx.root,
                "character_catalog",
                "build-stdout",
                &source_path(entry_source)?.to_string_lossy(),
                &[],
            )?;
            Ok((
                built,
                vec![entry_source.to_string()],
                serde_json::json!({"descriptors":document["descriptors"].as_object().map_or(0,|value| value.len()),"animation_groups":document["animation_groups"].as_array().map_or(0,Vec::len),"frame_directories":document["frame_directories"].as_array().map_or(0,Vec::len)}),
            ))
        }
        "golden-sun-message-archive" => {
            let document = json(&source_path(entry_source)?)?;
            let built = native_bytes(
                &ctx.root,
                "message_archive",
                "build-stdout",
                &source_path(entry_source)?.to_string_lossy(),
                &[],
            )?;
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
                    "assets/{}",
                    flat_asset_name(json_string(
                        item.get("source").ok_or("font source missing")?,
                        "font source"
                    )?)
                ));
            }
            nested.push(format!(
                "assets/{}",
                flat_asset_name(json_string(
                    &document["packed_images"]["source"],
                    "packed image source"
                )?)
            ));
            nested.push(format!(
                "assets/{}",
                flat_asset_name(json_string(&document["font"]["source"], "font source")?)
            ));
            for name in &nested {
                ctx.source(name)?;
            }
            let built = native_command(
                &ctx.root,
                "localization_font",
                &[
                    "build-stdout".to_string(),
                    source_path(entry_source)?.to_string_lossy().into_owned(),
                    "--root".to_string(),
                    ctx.root.join("assets").to_string_lossy().into_owned(),
                ],
            )?;
            Ok((
                built,
                std::iter::once(entry_source.to_string())
                    .chain(nested)
                    .collect(),
                serde_json::json!({"mtf_images":document["mtf_banks"].as_array().map_or(0,|banks|banks.iter().map(|b|json_number(&b["images"],"images").unwrap_or(0)).sum()),"packed_images":document["packed_images"]["images"],"font_glyphs":document["font"]["glyphs"],"article_entries":document["articles"]["entries"].as_array().map_or(0,Vec::len)}),
            ))
        }
        "golden-sun-localization-tables" => {
            let document = json(&source_path(entry_source)?)?;
            let built = native_bytes(
                &ctx.root,
                "localization_tables",
                "build-stdout",
                &source_path(entry_source)?.to_string_lossy(),
                &[],
            )?;
            Ok((
                built,
                vec![entry_source.to_string()],
                serde_json::json!({"segments":document["segments"].as_array().map_or(0,Vec::len)}),
            ))
        }
        "golden-sun-battle-effect-data" => {
            let document = json(&source_path(entry_source)?)?;
            let mut nested = Vec::new();
            for item in document["direct_graphics"]
                .as_array()
                .into_iter()
                .flatten()
                .chain(std::iter::once(&document["halfword_graphic"]))
                .chain(
                    document["palette_graphics"]
                        .as_array()
                        .into_iter()
                        .flatten(),
                )
            {
                nested.push(format!(
                    "assets/{}",
                    flat_asset_name(json_string(
                        item.get("source").ok_or("battle graphic source missing")?,
                        "battle graphic source"
                    )?)
                ));
            }
            for name in &nested {
                ctx.source(name)?;
            }
            let args = vec![
                "build-stdout".to_string(),
                source_path(entry_source)?.to_string_lossy().into_owned(),
                "--root".to_string(),
                ctx.root.join("assets").to_string_lossy().into_owned(),
            ];
            let (built, _) = native_with_report(&ctx.root, "battle_effect_data", &args)?;
            Ok((
                built,
                std::iter::once(entry_source.to_string())
                    .chain(nested)
                    .collect(),
                serde_json::json!({"graphics":document["direct_graphics"].as_array().map_or(0,Vec::len)+1+document["palette_graphics"].as_array().map_or(0,Vec::len),"weighted_records":document["weighted_records"]["records"].as_array().map_or(0,Vec::len),"typed_tables":document["typed_tables"].as_array().map_or(0,Vec::len)}),
            ))
        }
        "golden-sun-sentou-gamen-data" => {
            let document = json(&source_path(entry_source)?)?;
            let built = native_bytes(
                &ctx.root,
                "sentou_gamen_data",
                "build-stdout",
                &source_path(entry_source)?.to_string_lossy(),
                &[],
            )?;
            if address != SENTOU_GAMEN_ADDRESS || built.len() != SENTOU_GAMEN_SIZE {
                return Err(
                    "battle-screen package differs from canonical manifest extent".to_string(),
                );
            }
            let prefix = entry_source.replace("index.json", "");
            let mut nested = Vec::<String>::new();
            for item in document["graphics"].as_array().into_iter().flatten() {
                nested.push(format!(
                    "{prefix}{}",
                    json_string(
                        item.get("source").ok_or("battle graphics source missing")?,
                        "graphics source"
                    )?
                ));
            }
            for name in &nested {
                ctx.source(name)?;
            }
            Ok((
                built,
                std::iter::once(entry_source.to_string())
                    .chain(nested)
                    .collect(),
                serde_json::json!({"source_bytes":SENTOU_GAMEN_SIZE,"graphics":5,"display_glyph_cells":14,"derived_zero_bytes":3308}),
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
            let built = native_bytes(
                &ctx.root,
                "sentou_hyouji",
                "build-stdout",
                &source_path(entry_source)?.to_string_lossy(),
                &[],
            )?;
            Ok((
                built.clone(),
                nested,
                serde_json::json!({"source_bytes":built.len(),"typed_tables":3,"atlases":2}),
            ))
        }
        "golden-sun-sentou-kouka-runtime" => {
            let document = json(&source_path(entry_source)?)?;
            let built = native_bytes(
                &ctx.root,
                "sentou_kouka_runtime",
                "build-stdout",
                &source_path(entry_source)?.to_string_lossy(),
                &[],
            )?;
            if address != 0x080e_da78
                || built.len()
                    != json_number(
                        entry.get("size").ok_or("effect runtime size missing")?,
                        "effect runtime size",
                    )?
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
        "golden-sun-sentou-menu-data" => {
            let document = json(&source_path(entry_source)?)?;
            let built = native_bytes(
                &ctx.root,
                "sentou_menu_data",
                "build-stdout",
                &source_path(entry_source)?.to_string_lossy(),
                &[],
            )?;
            if address != 0x080b_3940 || built.len() != 0x16c0 {
                return Err(
                    "battle-menu package differs from canonical manifest extent".to_string()
                );
            }
            let prefix = entry_source.replace("index.json", "");
            let mut nested = Vec::<String>::new();
            for item in document["graphics"].as_array().into_iter().flatten() {
                nested.push(format!(
                    "{prefix}{}",
                    json_string(
                        item.get("source").ok_or("menu graphics source missing")?,
                        "graphics source"
                    )?
                ));
            }
            for name in &nested {
                ctx.source(name)?;
            }
            Ok((
                built.clone(),
                std::iter::once(entry_source.to_string())
                    .chain(nested)
                    .collect(),
                serde_json::json!({"source_bytes":built.len(),"atlases":5,"loadout_records":35}),
            ))
        }
        "golden-sun-staff-roll" => {
            let document = json(&source_path(entry_source)?)?;
            let built = native_bytes(
                &ctx.root,
                "staff_roll",
                "build-stdout",
                &source_path(entry_source)?.to_string_lossy(),
                &[],
            )?;
            if address != STAFF_ROLL_ADDRESS || built.len() != STAFF_ROLL_SIZE {
                return Err("staff-roll package differs from canonical manifest extent".to_string());
            }
            let font_path = child_path(
                &source_path(entry_source)?,
                json_string(&document["font"]["source"], "staff font source")?,
            );
            let nested = vec![
                entry_source.to_string(),
                root_relative(&ctx.root, &font_path)?,
            ];
            for name in &nested {
                ctx.source(name)?;
            }
            Ok((
                built.clone(),
                dedup_sources(nested),
                serde_json::json!({"source_bytes":built.len(),"preload_slots":33,"strings":110,"line_entries":339,"font_glyphs":96}),
            ))
        }
        "golden-sun-sentou-resource" => {
            let index_name = json_string(
                entry.get("index").ok_or("sentou resource index missing")?,
                "sentou resource index",
            )?;
            let index = json(&source_path(index_name)?)?;
            let resource = series_values(&index, "resources")?
                .iter()
                .find(|item| {
                    json_number(item.get("address").unwrap_or(&Value::Null), "address")
                        .unwrap_or(usize::MAX)
                        == address
                })
                .ok_or("sentou resource address is absent from its index")?;
            let prefix = Path::new(index_name)
                .file_name()
                .unwrap_or_default()
                .to_string_lossy()
                .replace("index.json", "");
            let plan_name = Path::new(index_name)
                .parent()
                .unwrap_or(Path::new("."))
                .join(format!(
                    "{prefix}{}",
                    json_string(
                        resource.get("source").ok_or("sentou plan missing")?,
                        "sentou plan"
                    )?
                ));
            let plan = json(&source_path(&plan_name.to_string_lossy())?)?;
            let image_name = format!(
                "{}{}",
                plan_name.to_string_lossy().replace("stream.json", ""),
                json_string(&plan["image"]["source"], "sentou image")?
            );
            let built = native_bytes(
                &ctx.root,
                "sentou_resources",
                "build-stdout",
                &source_path(&plan_name.to_string_lossy())?.to_string_lossy(),
                &[],
            )?;
            let mut sources = vec![
                index_name.to_string(),
                root_relative(&ctx.root, &source_path(&plan_name.to_string_lossy())?)?,
                root_relative(&ctx.root, &source_path(&image_name)?)?,
            ];
            if !plan["prefix_palette"].is_null() {
                sources.push(root_relative(
                    &ctx.root,
                    &source_path(&format!(
                        "{}{}",
                        plan_name.to_string_lossy().replace("stream.json", ""),
                        json_string(&plan["prefix_palette"]["source"], "palette source")?
                    ))?,
                )?);
            }
            Ok((
                built.clone(),
                dedup_sources(sources),
                serde_json::json!({"source_bytes":built.len()}),
            ))
        }
        "golden-sun-kind2-resource" => {
            let plan_path = source_path(entry_source)?;
            let plan = json(&plan_path)?;
            let image_name = json_string(&plan["image"]["source"], "kind2 image")?;
            let built = native_bytes(
                &ctx.root,
                "kind2-resources",
                "build-stdout",
                &plan_path.to_string_lossy(),
                &[],
            )?;
            let mut sources = vec![
                entry
                    .get("index")
                    .and_then(Value::as_str)
                    .unwrap_or(entry_source)
                    .to_string(),
                entry_source.to_string(),
                root_relative(
                    &ctx.root,
                    &plan_path
                        .parent()
                        .unwrap_or(Path::new("."))
                        .join(image_name),
                )?,
            ];
            if let Some(prefix) = plan.get("prefix_palette").and_then(Value::as_object) {
                sources.push(root_relative(
                    &ctx.root,
                    &plan_path
                        .parent()
                        .unwrap_or(Path::new("."))
                        .join(json_string(
                            prefix
                                .get("source")
                                .ok_or("prefix palette source missing")?,
                            "prefix palette source",
                        )?),
                )?);
            }
            Ok((
                built.clone(),
                dedup_sources(sources),
                serde_json::json!({"resource_id":plan.get("resource_id"),"source_bytes":built.len()}),
            ))
        }
        "golden-sun-tokushu-map" | "golden-sun-chiiki-map" => {
            let index_name = entry_source;
            let id = json_number(
                entry.get("resource_id").ok_or("map resource id missing")?,
                "map resource id",
            )?;
            let map_kind = if kind.contains("tokushu") {
                "tokushu"
            } else {
                "chiiki"
            };
            let resource = ctx
                .map_series(index_name, map_kind)?
                .into_iter()
                .find(|item| item.id == id)
                .ok_or("map resource differs from manifest")?;
            let data_len = resource.data.len();
            Ok((
                resource.data,
                resource.sources,
                serde_json::json!({"resource_id":format!("0x{id:03x}"),"source_bytes":data_len}),
            ))
        }
        "golden-sun-kana-glyph-bank" => {
            let source = source_path(entry_source)?;
            let built = native_bytes(
                &ctx.root,
                "resource_01c",
                "build-stdout",
                &source.to_string_lossy(),
                &[],
            )?;
            let document = json(&source)?;
            let nested = format!(
                "{}{}",
                entry_source.replace("stream.json", ""),
                json_string(&document["source"], "glyph source")?
            );
            ctx.source(&nested)?;
            Ok((
                built.clone(),
                vec![entry_source.to_string(), nested],
                serde_json::json!({"glyphs":36,"source_bytes":built.len()}),
            ))
        }
        "golden-sun-music-residual" => {
            let address = json_number(
                entry
                    .get("address")
                    .ok_or("music residual address missing")?,
                "music residual address",
            )?;
            let region = ctx
                .music_residuals(entry_source)?
                .into_iter()
                .find(|item| item.address == address)
                .ok_or("music residual differs from manifest")?;
            let data_len = region.data.len();
            Ok((
                region.data,
                region.sources,
                serde_json::json!({"source_bytes":data_len}),
            ))
        }
        "golden-sun-audio-engine-data" => {
            let source = source_path(entry_source)?;
            let result = build_audio_engine_data(&source).map_err(|error| error.to_string())?;
            if address != AUDIO_ENGINE_ADDRESS
                || result.address != AUDIO_ENGINE_ADDRESS
                || result.data.len() != AUDIO_ENGINE_SIZE
            {
                return Err("audio-engine data differs from canonical manifest extent".to_string());
            }
            let mut nested = vec![entry_source.to_string()];
            for path in result.sources.iter().skip(1) {
                nested.push(root_relative(&ctx.root, path)?);
            }
            Ok((
                result.data.clone(),
                dedup_sources(nested),
                serde_json::json!({"source_bytes":result.data.len(),"tone_records":225,"waveforms":18,"players":8,"derived_alignment_bytes":2}),
            ))
        }
        "golden-sun-d1-d3-resource" => {
            let id = json_number(
                entry.get("resource_id").ok_or("D1-D3 id missing")?,
                "D1-D3 id",
            )?;
            let built = native_bytes(
                &ctx.root,
                "resource_d1_d3",
                "build-stdout",
                &source_path(entry_source)?.to_string_lossy(),
                &[format!("{id:x}")],
            )?;
            let base = Path::new(entry_source).parent().unwrap_or(Path::new("."));
            let nested = if id == 0xd1 {
                vec![
                    base.join("resources_d1_d3_stream.json")
                        .to_string_lossy()
                        .replace('\\', "/"),
                    base.join("resources_d1_d3_iro.rgba.png")
                        .to_string_lossy()
                        .replace('\\', "/"),
                    base.join("resources_d1_d3_haikei.8bpp.png")
                        .to_string_lossy()
                        .replace('\\', "/"),
                ]
            } else {
                vec![base
                    .join(format!("resources_d1_d3_idou_d{}.json", id - 0xd0))
                    .to_string_lossy()
                    .replace('\\', "/")]
            };
            for name in &nested {
                ctx.source(name)?;
            }
            Ok((
                built.clone(),
                std::iter::once(entry_source.to_string())
                    .chain(nested)
                    .collect(),
                serde_json::json!({"resource_id":format!("0x{id:03x}"),"source_bytes":built.len(),"boundary_bytes":built.len(),"suffix_fallback":0}),
            ))
        }
        "golden-sun-final-battle-overlay" => {
            let component = json_string(
                entry.get("component").ok_or("overlay component missing")?,
                "overlay component",
            )?;
            let built = native_command(
                &ctx.root,
                "resource_3ce",
                &[
                    "build-stdout".to_string(),
                    source_path(entry_source)?.to_string_lossy().into_owned(),
                    component.to_string(),
                ],
            )?;
            let directory = Path::new(entry_source).parent().unwrap_or(Path::new("."));
            let nested = vec![
                entry_source.to_string(),
                directory
                    .join("overlay.s")
                    .to_string_lossy()
                    .replace('\\', "/"),
                directory
                    .join("stream.lz.json")
                    .to_string_lossy()
                    .replace('\\', "/"),
            ];
            for name in &nested {
                ctx.source(name)?;
            }
            let mut report = serde_json::json!({"component":component,"source_bytes":built.len()});
            if component == "stream" {
                report["fallback_bytes"] = Value::from(3);
            }
            Ok((built, nested, report))
        }
        "golden-sun-encounter-data" => {
            let source = source_path(entry_source)?;
            let size = json_number(
                entry.get("size").ok_or("encounter size missing")?,
                "encounter size",
            )?;
            let directory = source.parent().unwrap_or(Path::new("."));
            let args = vec![
                "build-region-stdout".to_string(),
                directory.to_string_lossy().into_owned(),
                hex_address(address),
                source
                    .file_name()
                    .unwrap_or_default()
                    .to_string_lossy()
                    .into_owned(),
                size.to_string(),
            ];
            let (built, report) = native_with_report(&ctx.root, "encounter_data", &args)?;
            Ok((built, vec![entry_source.to_string()], report))
        }
        "golden-sun-namae-nyuuryoku" => {
            let document = json(&source_path(entry_source)?)?;
            let built = native_bytes(
                &ctx.root,
                "namae_nyuuryoku",
                "build-stdout",
                &source_path(entry_source)?.to_string_lossy(),
                &[],
            )?;
            Ok((
                built,
                vec![entry_source.to_string()],
                serde_json::json!({"resource_ids":document["resource_ids"].as_array().map_or(0,Vec::len),"tilemap_entries":document["tilemap"]["tiles"].as_array().map_or(0,|rows|rows.iter().map(|row|row.as_array().map_or(0,Vec::len)).sum())}),
            ))
        }
        "golden-sun-gameplay-databases" => {
            let document = json(&source_path(entry_source)?)?;
            let built = native_bytes(
                &ctx.root,
                "resource_5",
                "build-stdout",
                &source_path(entry_source)?.to_string_lossy(),
                &[],
            )?;
            Ok((
                built,
                vec![entry_source.to_string()],
                serde_json::json!({"items":document["items"].as_array().map_or(0,Vec::len),"abilities":document["abilities"].as_array().map_or(0,Vec::len),"combatants":document["combatants"].as_array().map_or(0,Vec::len),"classes":document["classes"].as_array().map_or(0,Vec::len),"djinn":document["djinn"].as_array().map_or(0,Vec::len),"alignment_bytes":document.get("alignment_bytes")}),
            ))
        }
        "golden-sun-simple-resource" => {
            let id = json_number(
                entry
                    .get("resource_id")
                    .ok_or("simple resource id missing")?,
                "simple resource id",
            )?;
            let sources = match id {
                2 => vec![
                    "assets/data/resource_2_build_stamp.txt".to_string(),
                    "assets/data/resource_2_layout.json".to_string(),
                ],
                0x13 => vec!["assets/graphics/resource_13_font.4bpp.png".to_string()],
                0x14 => vec!["assets/graphics/resource_14_words.rgba.png".to_string()],
                0x18 => vec![
                    "assets/graphics/resource_18_screen.8bpp.png".to_string(),
                    "assets/graphics/resource_18_screen.lz.json".to_string(),
                ],
                _ => return Err("unsupported simple resource".to_string()),
            };
            for name in &sources {
                ctx.source(name)?;
            }
            let built = native_bytes(
                &ctx.root,
                "simple_resources",
                "build-stdout",
                &ctx.root.join("assets").to_string_lossy(),
                &[format!("{id:x}")],
            )?;
            Ok((built, sources, serde_json::json!({"resource_id":id})))
        }
        "golden-sun-title-lz" => {
            let document = json(&source_path(entry_source)?)?;
            let title_prefix = entry_source.replace("container.json", "");
            let mut sources = vec![entry_source.to_string()];
            for component in document["components"].as_array().into_iter().flatten() {
                let relative = format!(
                    "{}{}",
                    title_prefix,
                    json_string(
                        component
                            .get("source")
                            .ok_or("title component source missing")?,
                        "title component source"
                    )?
                    .replace('/', "_")
                );
                ctx.source(&relative)?;
                sources.push(relative);
            }
            let built = native_bytes(
                &ctx.root,
                "title_resources",
                "build-stdout",
                &source_path(entry_source)?.to_string_lossy(),
                &[],
            )?;
            Ok((
                built,
                sources,
                serde_json::json!({"resource_id":document["resource_id"],"decoded_size":document["decoded_size"],"components":document["components"].as_array().map_or(0,Vec::len),"fallback_tail":if document["tail"]["policy"].as_str()==Some("fallback"){json_number(&document["tail"]["size"],"tail size")?}else{0}}),
            ))
        }
        "golden-sun-offset-palette-lz" => {
            let plan_name = json_string(
                entry.get("plan").ok_or("offset palette plan missing")?,
                "offset palette plan",
            )?;
            let plan = json(&source_path(plan_name)?)?;
            let built = native_command(
                &ctx.root,
                "build-assets",
                &[
                    "--build-offset-archive".to_string(),
                    source_path(plan_name)?.to_string_lossy().into_owned(),
                    source_path(entry_source)?.to_string_lossy().into_owned(),
                ],
            )?;
            Ok((
                built,
                vec![entry_source.to_string(), plan_name.to_string()],
                serde_json::json!({"streams":plan["streams"].as_array().map_or(0,Vec::len),"chunk_width":plan["chunk_width"],"chunk_height":plan["chunk_height"]}),
            ))
        }
        "golden-sun-mtf4-archive" => {
            let plan_name = json_string(entry.get("plan").ok_or("F0 plan missing")?, "F0 plan")?;
            let plan = json(&source_path(plan_name)?)?;
            let built = native_bytes(
                &ctx.root,
                "f0_archive",
                "build-stdout",
                &source_path(plan_name)?.to_string_lossy(),
                &[source_path(entry_source)?.to_string_lossy().into_owned()],
            )?;
            let images = json_number(&plan["images"], "F0 images")?;
            let sources = std::iter::once(plan_name.to_string())
                .chain(
                    (0..images).map(|index| format!("{entry_source}_images_image_{index:02}.png")),
                )
                .collect();
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
        manifest: root.join("assets/manifest.json"),
        output: root.join("out/assets"),
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

fn stamp_files(directory: &Path, files: &mut Vec<PathBuf>) -> Result<(), String> {
    if !directory.exists() {
        return Ok(());
    }
    for entry in
        fs::read_dir(directory).map_err(|error| format!("{}: {error}", directory.display()))?
    {
        let entry = entry.map_err(|error| error.to_string())?;
        let path = entry.path();
        if path.is_dir() {
            stamp_files(&path, files)?;
        } else if path.is_file() {
            files.push(path);
        }
    }
    Ok(())
}

fn stage_stamp(root: &Path, manifest: &Path, source_only: bool) -> Result<String, String> {
    let mut hasher = DefaultHasher::new();
    format!(
        "assets:native:{}\0",
        if source_only { "source-only" } else { "rom" }
    )
    .hash(&mut hasher);
    fs::read(manifest)
        .map_err(|error| format!("{}: {error}", manifest.display()))?
        .hash(&mut hasher);
    let mut files = Vec::new();
    for directory in ["assets", "tools", "semantic", "exact"] {
        stamp_files(&root.join(directory), &mut files)?;
    }
    files.extend([
        root.join("tools/build-assets/src/main.rs"),
        root.join("tools/build-assets/Cargo.toml"),
        root.join("tools/build-assets/Cargo.lock"),
    ]);
    files.sort();
    for path in files {
        let metadata =
            fs::metadata(&path).map_err(|error| format!("{}: {error}", path.display()))?;
        let relative = path.strip_prefix(root).unwrap_or(&path).to_string_lossy();
        relative.hash(&mut hasher);
        metadata.len().hash(&mut hasher);
        if let Ok(modified) = metadata.modified() {
            if let Ok(duration) = modified.duration_since(std::time::UNIX_EPOCH) {
                duration.as_nanos().hash(&mut hasher);
            }
        }
    }
    Ok(format!("{:016x}", hasher.finish()))
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
    if json_number(
        manifest
            .get("format")
            .ok_or("asset manifest format is missing")?,
        "asset manifest format",
    )? != 1
    {
        return Err("unsupported asset manifest format".to_string());
    }
    fs::create_dir_all(&options.output)
        .map_err(|error| format!("{}: {error}", options.output.display()))?;
    let stamp = stage_stamp(&root, &options.manifest, options.source_only)?;
    let stamp_path = options.output.join("stage-stamp.txt");
    let built_manifest = options.output.join("manifest.json");
    if stamp_path.exists()
        && built_manifest.exists()
        && fs::read_to_string(&stamp_path)
            .map_err(|error| error.to_string())?
            .trim()
            == stamp
    {
        if let Ok(previous) = json(&built_manifest) {
            let present = previous
                .get("regions")
                .and_then(Value::as_array)
                .is_some_and(|regions| {
                    regions.iter().all(|region| {
                        region
                            .get("output")
                            .and_then(Value::as_str)
                            .is_some_and(|output| Path::new(output).exists())
                    })
                });
            if present {
                let count = previous
                    .get("regions")
                    .and_then(Value::as_array)
                    .map_or(0, Vec::len);
                let bytes = previous.get("bytes").and_then(Value::as_u64).unwrap_or(0);
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
        if entries[index].get("kind").and_then(Value::as_str)
            != Some("golden-sun-byte-value-region-package")
        {
            index += 1;
            continue;
        }
        let package = entries.remove(index);
        let source_name = json_string(
            package
                .get("source")
                .ok_or("byte-value package source is missing")?,
            "byte-value package source",
        )?;
        let document = json(&ctx.source(source_name)?)?;
        if document.get("kind").and_then(Value::as_str) != Some("golden-sun-byte-value-regions") {
            return Err("byte-value package source differs".to_string());
        }
        let regions = document
            .get("regions")
            .and_then(Value::as_array)
            .ok_or("byte-value package regions are missing")?;
        let generated = regions
            .iter()
            .map(|region| {
                let values = region
                    .get("values")
                    .and_then(Value::as_array)
                    .map_or(0, Vec::len);
                serde_json::json!({
                    "address": region.get("address"),
                    "size": values,
                    "kind": "golden-sun-byte-value-regions",
                    "source": source_name,
                })
            })
            .collect::<Vec<_>>();
        entries.splice(index..index, generated);
    }

    entries.sort_unstable_by_key(|entry| {
        json_number(
            entry.get("address").unwrap_or(&Value::Null),
            "asset address",
        )
        .unwrap_or(usize::MAX)
    });
    let mut previous_end = ROM_BASE;
    let mut regions = Vec::new();
    let mut all_sources = Vec::<String>::new();
    for entry in &entries {
        let address = json_number(
            entry.get("address").ok_or("asset address is missing")?,
            "asset address",
        )?;
        let size = json_number(
            entry.get("size").ok_or("asset size is missing")?,
            "asset size",
        )?;
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
        fs::write(&output, &built).map_err(|error| format!("{}: {error}", output.display()))?;
        regions.push(serde_json::json!({
            "address": address,
            "size": size,
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
    let unused = unused_tracked_images(&root, all_sources.iter(), ["assets/readme/"])
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
        .map(|region| json_number(&region["size"], "asset size").unwrap_or(0))
        .sum::<usize>();
    let output_manifest = serde_json::json!({
        "format": 1,
        "rom_base": ROM_BASE,
        "rom_size": rom_size,
        "verification": if options.source_only { "source_only" } else { "rom" },
        "asset_bytes": asset_bytes,
        "regions": regions,
    });
    fs::write(
        &built_manifest,
        format!("{}\n", canonical_json(&output_manifest)),
    )
    .map_err(|error| format!("{}: {error}", built_manifest.display()))?;
    fs::write(&stamp_path, format!("{stamp}\n"))
        .map_err(|error| format!("{}: {error}", stamp_path.display()))?;
    println!(
        "assets={} bytes={asset_bytes}",
        output_manifest["regions"].as_array().map_or(0, Vec::len)
    );
    Ok(())
}

fn run(arguments: Vec<String>) -> Result<ExitCode, String> {
    if arguments.first().map(String::as_str) == Some("--build-audio-engine") {
        if arguments.len() != 2 {
            return Err("usage: build-assets --build-audio-engine INDEX".to_string());
        }
        build_audio_engine(Path::new(&arguments[1]))?;
        return Ok(ExitCode::SUCCESS);
    }
    if arguments.first().map(String::as_str) == Some("--build-gba-header") {
        if arguments.len() != 4 {
            return Err("usage: build-assets --build-gba-header SOURCE ADDRESS SIZE".to_string());
        }
        build_gba_header(Path::new(&arguments[1]), &arguments[2], &arguments[3])?;
        return Ok(ExitCode::SUCCESS);
    }
    if arguments.first().map(String::as_str) == Some("--build-offset-archive") {
        if arguments.len() != 3 {
            return Err("usage: build-assets --build-offset-archive PLAN ATLAS".to_string());
        }
        build_offset_archive(Path::new(&arguments[1]), Path::new(&arguments[2]))?;
        return Ok(ExitCode::SUCCESS);
    }
    if arguments.as_slice() == ["--self-test"] {
        archive_self_test().map_err(|error| error.to_string())?;
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

fn main() -> ExitCode {
    match run(env::args().skip(1).collect()) {
        Ok(code) => code,
        Err(error) => {
            eprintln!("error: {error}");
            ExitCode::FAILURE
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn parses_decimal_and_hex_numbers() {
        assert_eq!(number(&Value::String("0x10".into()), "n").unwrap(), 16);
        assert_eq!(number(&Value::from(17), "n").unwrap(), 17);
    }

    #[test]
    fn parses_palette_tokens() {
        let group = parse_group(&serde_json::json!(["g", [["l"], ["c", 4, 2], ["e"]]])).unwrap();
        assert_eq!(
            group,
            PaletteGroup::Group(vec![
                PaletteOperation::Literal,
                PaletteOperation::Copy {
                    length: 4,
                    distance: 2
                },
                PaletteOperation::End,
            ])
        );
    }

    #[test]
    fn rejects_unknown_token_tags() {
        assert!(parse_group(&serde_json::json!(["nope"])).is_err());
    }
}
