//! Native implementation of `tools/make/title_resources.ts`.
//!
//! The title resources are three adjacent palette-LZ containers.  Their
//! decoded payloads are kept as ordinary tiled PNG components, while the
//! compact palette trace in each container remains part of the tracked plan.

use alignment_tail::{build_alignment_tail, inspect_alignment_tail, parse_alignment_tail};
use export_asset::tile_png;
use extract_resource::{decode_palette_trace, encode_palette, PaletteGroup, PaletteOperation};
use import_asset::{gba_graphics, Rgb};
use serde_json::{json, Value};
use std::fs;
use std::io::{self, Write};
use std::path::{Path, PathBuf};

const ROM_BASE: usize = 0x0800_0000;
const RESOURCE_TABLE: usize = 0x0832_0000;
const USAGE: &str = "usage: title_resources export ROM --directory DIR | build PLAN --output FILE | verify ROM --directory DIR | --self-test";

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Error(pub String);

impl std::fmt::Display for Error {
    fn fmt(&self, output: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        output.write_str(&self.0)
    }
}

impl std::error::Error for Error {}

fn err(message: impl Into<String>) -> Error {
    Error(message.into())
}

#[derive(Clone, Copy)]
struct FixedComponent {
    source: &'static str,
    offset: usize,
    bpp: u8,
    width: usize,
    height: usize,
    role: &'static str,
}

#[derive(Clone, Copy)]
struct FixedResource {
    id: usize,
    decoded: usize,
    palette_upload: usize,
    components: &'static [FixedComponent],
}

const R15_COMPONENTS: [FixedComponent; 9] = [
    FixedComponent {
        source: "objects/hikari_64.8bpp.png",
        offset: 0x0000,
        bpp: 8,
        width: 64,
        height: 64,
        role: "hikari",
    },
    FixedComponent {
        source: "objects/taiyou_32.8bpp.png",
        offset: 0x1000,
        bpp: 8,
        width: 32,
        height: 32,
        role: "taiyou",
    },
    FixedComponent {
        source: "objects/niji_32.8bpp.png",
        offset: 0x1400,
        bpp: 8,
        width: 32,
        height: 32,
        role: "niji",
    },
    FixedComponent {
        source: "objects/niji_16_a.8bpp.png",
        offset: 0x1800,
        bpp: 8,
        width: 16,
        height: 16,
        role: "niji_a",
    },
    FixedComponent {
        source: "objects/niji_16_b.8bpp.png",
        offset: 0x1900,
        bpp: 8,
        width: 16,
        height: 16,
        role: "niji_b",
    },
    FixedComponent {
        source: "objects/niji_16_c.8bpp.png",
        offset: 0x1a00,
        bpp: 8,
        width: 16,
        height: 16,
        role: "niji_c",
    },
    FixedComponent {
        source: "objects/niji_16_d.8bpp.png",
        offset: 0x1b00,
        bpp: 8,
        width: 16,
        height: 16,
        role: "niji_d",
    },
    FixedComponent {
        source: "objects/niji_16_e.8bpp.png",
        offset: 0x1c00,
        bpp: 8,
        width: 16,
        height: 16,
        role: "niji_e",
    },
    FixedComponent {
        source: "objects/niji_16_f.8bpp.png",
        offset: 0x1d00,
        bpp: 8,
        width: 16,
        height: 16,
        role: "niji_f",
    },
];
const R16_COMPONENTS: [FixedComponent; 1] = [FixedComponent {
    source: "title.8bpp.png",
    offset: 0,
    bpp: 8,
    width: 240,
    height: 160,
    role: "title",
}];
const R17_COMPONENTS: [FixedComponent; 2] = [
    FixedComponent {
        source: "sora.4bpp.png",
        offset: 0,
        bpp: 4,
        width: 240,
        height: 344,
        role: "sora",
    },
    FixedComponent {
        source: "iwa.8bpp.png",
        offset: 0xa140,
        bpp: 8,
        width: 240,
        height: 184,
        role: "iwa",
    },
];
const RESOURCES: [FixedResource; 3] = [
    FixedResource {
        id: 0x15,
        decoded: 0x1e00,
        palette_upload: 256,
        components: &R15_COMPONENTS,
    },
    FixedResource {
        id: 0x16,
        decoded: 0x9600,
        palette_upload: 240,
        components: &R16_COMPONENTS,
    },
    FixedResource {
        id: 0x17,
        decoded: 0x14dc0,
        palette_upload: 256,
        components: &R17_COMPONENTS,
    },
];

fn read(path: &Path) -> Result<Vec<u8>, Error> {
    fs::read(path).map_err(|e| err(format!("{}: {e}", path.display())))
}

fn write(path: &Path, data: &[u8]) -> Result<(), Error> {
    if let Some(parent) = path.parent() {
        fs::create_dir_all(parent).map_err(|e| err(format!("{}: {e}", parent.display())))?;
    }
    fs::write(path, data).map_err(|e| err(format!("{}: {e}", path.display())))
}

fn json(path: &Path) -> Result<Value, Error> {
    let data = read(path)?;
    serde_json::from_slice(&data).map_err(|e| err(format!("{}: {e}", path.display())))
}

fn field<'a>(value: &'a Value, name: &str) -> Result<&'a Value, Error> {
    value
        .get(name)
        .ok_or_else(|| err(format!("missing {name}")))
}

fn number(value: &Value, name: &str) -> Result<usize, Error> {
    value
        .as_u64()
        .and_then(|n| usize::try_from(n).ok())
        .ok_or_else(|| err(format!("{name} must be an integer")))
}

fn string<'a>(value: &'a Value, name: &str) -> Result<&'a str, Error> {
    value
        .as_str()
        .ok_or_else(|| err(format!("{name} must be a string")))
}

fn hex(value: usize) -> String {
    format!("0x{value:08x}")
}

fn component_size(bpp: u8, width: usize, height: usize) -> Result<usize, Error> {
    let bits = width
        .checked_mul(height)
        .and_then(|n| n.checked_mul(bpp as usize))
        .ok_or_else(|| err("component size overflow"))?;
    if bits % 8 != 0 {
        return Err(err("component size is fractional"));
    }
    Ok(bits / 8)
}

fn validate_components(components: &[Value], decoded_size: usize) -> Result<(), Error> {
    let mut cursor = 0;
    for component in components {
        let offset = number(field(component, "offset")?, "component offset")?;
        let bpp = number(field(component, "bpp")?, "component bpp")?;
        let width = number(field(component, "width")?, "component width")?;
        let height = number(field(component, "height")?, "component height")?;
        let size = number(field(component, "size")?, "component size")?;
        if offset != cursor || size != component_size(bpp as u8, width, height)? {
            return Err(err("title components are not a contiguous native layout"));
        }
        if width % 8 != 0 || height % 8 != 0 {
            return Err(err("title component dimensions are not tile aligned"));
        }
        cursor += size;
    }
    if cursor != decoded_size {
        return Err(err("title components do not cover decoded output"));
    }
    Ok(())
}

fn fixed_component_value(component: FixedComponent) -> Value {
    json!({
        "source": component.source,
        "offset": component.offset,
        "bpp": component.bpp,
        "width": component.width,
        "height": component.height,
        "role": component.role,
        "size": component_size(component.bpp, component.width, component.height).unwrap(),
    })
}

fn consumer(id: usize) -> Value {
    match id {
        0x15 => json!({
            "palette": "obj", "vram": "obj", "mapping": "1d", "tile_depth": 8,
            "objects": [
                {"role":"hikari","tile":0,"width":64,"height":64},
                {"role":"taiyou","tile":64,"width":32,"height":32},
                {"role":"niji","tile":80,"width":32,"height":32},
                {"role":"niji_a","tile":96,"width":16,"height":16},
                {"role":"niji_b","tile":100,"width":16,"height":16},
                {"role":"niji_c","tile":104,"width":16,"height":16},
                {"role":"niji_d","tile":108,"width":16,"height":16},
                {"role":"niji_e","tile":112,"width":16,"height":16},
                {"role":"niji_f","tile":116,"width":16,"height":16}
            ]
        }),
        0x16 => json!({
            "palette": "bg", "vram_address": "0x06004000", "tile_depth": 8,
            "tilemap": {"width":30,"height":20,"screen_base":6,"first_tile":256}
        }),
        0x17 => json!({
            "palette": "bg", "map_width": 30, "map_height": 21, "circular_rows": 32,
            "uploads": [
                {"role":"sora","source_row":11,"rows":21,"vram_address":"0x06000000","tile_depth":4},
                {"role":"iwa","source_row":0,"rows":21,"vram_address":"0x06004ec0","tile_depth":8}
            ]
        }),
        _ => Value::Null,
    }
}

fn resource_pointer(rom: &[u8], id: usize) -> Result<usize, Error> {
    let offset = RESOURCE_TABLE
        .checked_sub(ROM_BASE)
        .and_then(|base| base.checked_add(id * 4))
        .ok_or_else(|| err("resource pointer lies outside the ROM"))?;
    let bytes = rom
        .get(offset..offset + 4)
        .ok_or_else(|| err("resource pointer lies outside the ROM"))?;
    let address = u32::from_le_bytes(bytes.try_into().unwrap()) as usize;
    if address < ROM_BASE || address > ROM_BASE + rom.len() {
        return Err(err("resource pointer lies outside the ROM"));
    }
    Ok(address - ROM_BASE)
}

fn palette_colors(raw: &[u8]) -> Result<Vec<Rgb>, Error> {
    if raw.len() != 0x200 {
        return Err(err("title palette must contain 256 colors"));
    }
    Ok(raw
        .chunks_exact(2)
        .map(|pair| {
            let value = u16::from_le_bytes([pair[0], pair[1]]);
            if value & 0x8000 != 0 {
                return Err(err("title palette contains a high-bit color"));
            }
            Ok([
                ((value & 31) << 3) as u8,
                (((value >> 5) & 31) << 3) as u8,
                (((value >> 10) & 31) << 3) as u8,
            ])
        })
        .collect::<Result<Vec<_>, Error>>()?)
}

fn compact_groups(groups: &[PaletteGroup]) -> Result<Vec<Value>, Error> {
    groups
        .iter()
        .map(|group| match group {
            PaletteGroup::Zeros => Ok(json!(0)),
            PaletteGroup::Group(operations) => {
                if operations.len() > 8 {
                    return Err(err("title palette group has too many operations"));
                }
                let mut flags = 0u8;
                let mut values = vec![Value::from(0u8)];
                for (index, operation) in operations.iter().enumerate() {
                    match operation {
                        PaletteOperation::Literal => {}
                        PaletteOperation::End => {
                            flags |= 1 << (7 - index);
                            values.extend([Value::from(0u8), Value::from(0u8)])
                        }
                        PaletteOperation::Copy { length, distance } => {
                            flags |= 1 << (7 - index);
                            values.extend([Value::from(*length), Value::from(*distance)]);
                        }
                    }
                }
                if flags == 0 {
                    return Err(err("nonzero title group has no copy operation"));
                }
                values[0] = Value::from(flags);
                Ok(Value::Array(values))
            }
        })
        .collect()
}

fn expand_groups(value: &Value) -> Result<Vec<PaletteGroup>, Error> {
    let groups = value
        .as_array()
        .ok_or_else(|| err("invalid compact title groups"))?;
    let mut ended = false;
    let mut result = Vec::new();
    for group in groups {
        if ended {
            return Err(err("title groups contain data after terminator"));
        }
        if group.as_u64() == Some(0) {
            result.push(PaletteGroup::Zeros);
            continue;
        }
        let values = group
            .as_array()
            .ok_or_else(|| err("invalid compact title group"))?;
        if values.len() < 3 || values.iter().any(|v| v.as_u64().is_none()) {
            return Err(err("invalid compact title group"));
        }
        let flags = values[0].as_u64().unwrap();
        if !(1..=255).contains(&flags) {
            return Err(err("compact title flags are invalid"));
        }
        let mut cursor = 1usize;
        let mut operations = Vec::new();
        for bit in (0..8).rev() {
            if flags & (1 << bit) == 0 {
                operations.push(PaletteOperation::Literal);
                continue;
            }
            if cursor + 1 >= values.len() {
                return Err(err("compact title copy is truncated"));
            }
            let length = values[cursor].as_u64().unwrap() as u32;
            let distance = values[cursor + 1].as_u64().unwrap() as u32;
            cursor += 2;
            if length == 0 && distance == 0 {
                operations.push(PaletteOperation::End);
                ended = true;
                break;
            }
            operations.push(PaletteOperation::Copy { length, distance });
        }
        if cursor != values.len() {
            return Err(err("compact title group has unused values"));
        }
        result.push(PaletteGroup::Group(operations));
    }
    if !ended {
        return Err(err("compact title groups lack a terminator"));
    }
    Ok(result)
}

fn plan_components(plan: &Value) -> Result<Vec<Value>, Error> {
    plan.get("components")
        .and_then(Value::as_array)
        .cloned()
        .ok_or_else(|| err("title components must be an array"))
}

fn source_for(plan_path: &Path, source: &str) -> PathBuf {
    let text = plan_path.to_string_lossy();
    let base = text.strip_suffix("container.json").unwrap_or(&text);
    let flat = PathBuf::from(format!("{}{}", base, source.replace('/', "_")));
    if flat.is_file() {
        flat
    } else {
        plan_path
            .parent()
            .unwrap_or_else(|| Path::new("."))
            .join(source)
    }
}

pub fn build_title_resource(plan_path: &Path) -> Result<Vec<u8>, Error> {
    let plan = json(plan_path)?;
    if number(field(&plan, "format")?, "format")? != 1
        || string(field(&plan, "codec")?, "codec")? != "golden-sun-title-lz"
    {
        return Err(err("unsupported title-resource plan"));
    }
    let decoded_size = number(field(&plan, "decoded_size")?, "decoded_size")?;
    let source_size = number(field(&plan, "source_size")?, "source_size")?;
    let container_size = number(field(&plan, "container_size")?, "container_size")?;
    let palette_entries = number(field(&plan, "palette_entries")?, "palette_entries")?;
    let components = plan_components(&plan)?;
    validate_components(&components, decoded_size)?;
    let mut decoded = vec![0u8; decoded_size];
    let mut full_palette: Option<Vec<u8>> = None;
    let mut graphics = Vec::new();
    for component in &components {
        let source = string(field(component, "source")?, "component source")?;
        let bpp = number(field(component, "bpp")?, "component bpp")? as f64;
        let width = number(field(component, "width")?, "component width")?;
        let height = number(field(component, "height")?, "component height")?;
        let size = number(field(component, "size")?, "component size")?;
        let image = read(&source_for(plan_path, source))?;
        let (tiles, palette, report) = gba_graphics(&image, bpp).map_err(|e| err(e.0))?;
        if report.get("width") != Some(width as f64)
            || report.get("height") != Some(height as f64)
            || tiles.len() != size
        {
            return Err(err(format!("title image dimensions differ: {source}")));
        }
        if palette.len() == palette_entries * 2 && full_palette.is_none() {
            full_palette = Some(palette.clone());
        }
        graphics.push((tiles, palette));
    }
    let palette = full_palette.ok_or_else(|| err("full title palette is missing"))?;
    for (index, component) in components.iter().enumerate() {
        let (tiles, component_palette) = &graphics[index];
        if palette.get(..component_palette.len()) != Some(component_palette.as_slice()) {
            return Err(err(format!(
                "title image palette differs: {}",
                string(field(component, "source")?, "component source")?
            )));
        }
        let offset = number(field(component, "offset")?, "component offset")?;
        decoded[offset..offset + tiles.len()].copy_from_slice(tiles);
    }
    if palette.len() != palette_entries * 2 {
        return Err(err("title palette has the wrong size"));
    }
    let groups = expand_groups(field(&plan, "groups")?)?;
    let encoded_stream = encode_palette(&decoded, &groups).map_err(|e| err(e.0))?;
    let mut encoded = palette;
    encoded.extend(encoded_stream);
    if encoded.len() != source_size {
        return Err(err("title source size differs from its plan"));
    }
    let tail_value = field(&plan, "tail")?;
    let tail_size = number(field(tail_value, "size")?, "title tail size")?;
    if tail_size != container_size.saturating_sub(source_size) || container_size < source_size {
        return Err(err("title tail size differs from its plan"));
    }
    if let Some(policy) = tail_value.get("policy").and_then(Value::as_str) {
        match policy {
            "zero" => encoded.extend(std::iter::repeat_n(0, tail_size)),
            "fallback" => {}
            _ => return Err(err("unsupported title tail policy")),
        }
    } else {
        let tail =
            parse_alignment_tail(tail_value, tail_size, 3, "title tail").map_err(|e| err(e.0))?;
        encoded.extend(build_alignment_tail(&tail));
    }
    Ok(encoded)
}

fn export_resource(rom: &[u8], root: &Path, fixed: FixedResource) -> Result<(usize, usize), Error> {
    let start = resource_pointer(rom, fixed.id)?;
    let end = resource_pointer(rom, fixed.id + 1)?;
    if end <= start + 0x200 {
        return Err(err("title resource is empty"));
    }
    let palette = &rom[start..start + 0x200];
    let (decoded, cursor, tokens) =
        decode_palette_trace(rom, start + 0x200, end, fixed.decoded as u64)
            .map_err(|e| err(e.0))?;
    if decoded.len() != fixed.decoded {
        return Err(err("title decoded size differs from consumer bounds"));
    }
    let replay_stream = encode_palette(&decoded, &tokens).map_err(|e| err(e.0))?;
    let mut replay = palette.to_vec();
    replay.extend(replay_stream);
    if replay != rom[start..cursor] {
        return Err(err("title codec replay differs"));
    }
    let tail = &rom[cursor..end];
    let directory = root.join(format!("resource_{:x}", fixed.id));
    let palette_colors = palette_colors(palette)?;
    let mut component_values = Vec::new();
    for component in fixed.components {
        let size = component_size(component.bpp, component.width, component.height)?;
        let image = tile_png(
            &decoded[component.offset..component.offset + size],
            component.bpp as f64,
            (component.width / 8) as f64,
            Some(if component.bpp == 4 {
                &palette_colors[..16]
            } else {
                &palette_colors
            }),
        )
        .map_err(|e| err(e.0))?
        .0;
        write(&directory.join(component.source), &image)?;
        component_values.push(fixed_component_value(*component));
    }
    let plan = json!({
        "format": 1, "codec": "golden-sun-title-lz", "resource_id": fixed.id,
        "address": hex(ROM_BASE + start), "container_size": end - start, "source_size": cursor - start,
        "decoded_size": decoded.len(), "palette_entries": 256, "palette_upload_entries": fixed.palette_upload,
        "tail": if tail.is_empty() { json!({"size":0,"policy":"zero"}) } else { serde_json::to_value(inspect_alignment_tail(tail, 3).map_err(|e| err(e.0))?).unwrap() },
        "components": component_values, "consumer": consumer(fixed.id), "groups": compact_groups(&tokens)?
    });
    let plan_path = directory.join("container.json");
    let text = serde_json::to_string(&plan).map_err(|e| err(e.to_string()))?;
    write(&plan_path, format!("{text}\n").as_bytes())?;
    let built = build_title_resource(&plan_path)?;
    if built != rom[start..start + built.len()] {
        return Err(err("title canonical source replay differs"));
    }
    Ok((built.len(), end - start - built.len()))
}

pub fn export_title_resources(rom_path: &Path, root: &Path) -> Result<(usize, usize), Error> {
    let rom = read(rom_path)?;
    RESOURCES
        .iter()
        .try_fold((0, 0), |(source, fallback), fixed| {
            let (built, gap) = export_resource(&rom, root, *fixed)?;
            Ok((source + built, fallback + gap))
        })
}

pub fn verify_title_resources(rom_path: &Path, root: &Path) -> Result<String, Error> {
    let rom = read(rom_path)?;
    let mut source = 0;
    let mut fallback = 0;
    for fixed in RESOURCES {
        let start = resource_pointer(&rom, fixed.id)?;
        let end = resource_pointer(&rom, fixed.id + 1)?;
        let nested = root.join(format!("resource_{:x}/container.json", fixed.id));
        let flat = root.join(format!("resource_{:x}_container.json", fixed.id));
        let plan = if nested.is_file() { nested } else { flat };
        let built = build_title_resource(&plan)?;
        if end < start || built.len() > end - start || built != rom[start..start + built.len()] {
            return Err(err(format!(
                "title resource {:x} differs from ROM",
                fixed.id
            )));
        }
        source += built.len();
        fallback += end - start - built.len();
    }
    Ok(format!(
        "identical=true resources={} source={} fallback={}",
        RESOURCES.len(),
        source,
        fallback
    ))
}

pub fn self_test() -> Result<(), Error> {
    for fixed in RESOURCES {
        let mut cursor = 0;
        for component in fixed.components {
            if component.offset != cursor {
                return Err(err("title component offsets are not contiguous"));
            }
            cursor += component_size(component.bpp, component.width, component.height)?;
        }
        if cursor != fixed.decoded {
            return Err(err("title component sizes are inconsistent"));
        }
    }
    let groups = vec![
        PaletteGroup::Zeros,
        PaletteGroup::Group(vec![
            PaletteOperation::Literal,
            PaletteOperation::Copy {
                length: 2,
                distance: 1,
            },
            PaletteOperation::End,
        ]),
    ];
    let compact = compact_groups(&groups)?;
    if expand_groups(&Value::Array(compact))? != groups {
        return Err(err("compact title group round trip differs"));
    }
    if expand_groups(&json!([[128, 2]])).is_ok() {
        return Err(err("truncated compact title group was accepted"));
    }
    if inspect_alignment_tail(&[0xa5, 0xa5, 0xa5], 3).is_err()
        || inspect_alignment_tail(&[0x12, 0x34], 3).is_err()
    {
        return Err(err("alignment-tail self-test failed"));
    }
    Ok(())
}

fn option(args: &[String], name: &str) -> Result<String, Error> {
    let index = args
        .iter()
        .position(|arg| arg == name)
        .ok_or_else(|| err(format!("{name} is required")))?;
    args.get(index + 1)
        .cloned()
        .ok_or_else(|| err(format!("{name} is required")))
}

pub fn run(mut args: Vec<String>) -> Result<(), Error> {
    if args.iter().any(|arg| arg == "--self-test") {
        self_test()?;
        println!("self-test=ok");
        args.retain(|arg| arg != "--self-test");
        if args.is_empty() {
            return Ok(());
        }
    }
    if args.iter().any(|arg| arg == "-h" || arg == "--help") {
        println!("{USAGE}");
        return Ok(());
    }
    match args.first().map(String::as_str) {
        Some("export") => {
            let (source, fallback) = export_title_resources(
                Path::new(args.get(1).ok_or_else(|| err(USAGE))?),
                Path::new(&option(&args, "--directory")?),
            )?;
            println!(
                "resources={} source={} fallback={}",
                RESOURCES.len(),
                source,
                fallback
            );
            Ok(())
        }
        Some("build") => {
            let built = build_title_resource(Path::new(args.get(1).ok_or_else(|| err(USAGE))?))?;
            write(Path::new(&option(&args, "--output")?), &built)?;
            println!("bytes={}", built.len());
            Ok(())
        }
        Some("build-stdout") => {
            let built = build_title_resource(Path::new(
                args.get(1).ok_or_else(|| err(USAGE))?,
            ))?;
            io::stdout().write_all(&built).map_err(|e| err(e.to_string()))?;
            Ok(())
        }
        Some("verify") => {
            println!(
                "{}",
                verify_title_resources(
                    Path::new(args.get(1).ok_or_else(|| err(USAGE))?),
                    Path::new(&option(&args, "--directory")?)
                )?
            );
            Ok(())
        }
        _ => Err(err(USAGE)),
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn fixed_layout_is_contiguous() {
        self_test().unwrap();
    }

    #[test]
    fn compact_groups_reject_truncated_copy() {
        assert!(expand_groups(&json!([[128, 2]])).is_err());
    }

    #[test]
    fn tracked_resources_match_the_rom() {
        let root = Path::new(env!("CARGO_MANIFEST_DIR"))
            .parent()
            .and_then(Path::parent)
            .unwrap();
        let result =
            verify_title_resources(&root.join("baserom.gba"), &root.join("assets/graphics"))
                .unwrap();
        assert_eq!(result, "identical=true resources=3 source=65327 fallback=1");
    }
}
