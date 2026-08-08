//! Native build and ROM verification for the two Golden Sun map-container series.
//!
//! The map grid and six container components are deliberately delegated to
//! their native crates. This crate only owns the audited series catalog,
//! component extents, and composition of a resource from its tracked sources.
//! It is build-facing: export is intentionally not part of this port.

use canonical_json::is_canonical_json_text;
use kind1_map_grid::{build_grid, self_test as grid_self_test};
use map_container_components::{
    build_blend_animation, build_descriptors, build_header, build_metatiles, build_queues,
    build_sparse, self_test as components_self_test,
};
use serde_json::{Map, Value};
use std::fs;
use std::io::Write;
use std::path::{Path, PathBuf};

pub type Result<T> = std::result::Result<T, String>;

pub const ROM_BASE: usize = 0x0800_0000;
pub const RESOURCE_TABLE: usize = 0x0832_0000;
const HEADER_SIZE: usize = 0x3c;

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum SeriesKind {
    Tokushu,
    Chiiki,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
struct ResourceSpec {
    id: usize,
    address: usize,
    size: usize,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct BuiltMapResource {
    pub id: usize,
    pub address: usize,
    pub data: Vec<u8>,
    pub sources: Vec<PathBuf>,
}

#[derive(Debug, Clone)]
struct IndexEntry {
    id: usize,
    address: usize,
    directory: String,
}

const TOKUSHU: [ResourceSpec; 6] = [
    ResourceSpec {
        id: 0x1f3,
        address: 0x085c_d0d8,
        size: 0x1e00,
    },
    ResourceSpec {
        id: 0x1f6,
        address: 0x085d_0188,
        size: 0x1d5c,
    },
    ResourceSpec {
        id: 0x28c,
        address: 0x086a_bcc0,
        size: 0x1f68,
    },
    ResourceSpec {
        id: 0x2d4,
        address: 0x086f_4ae8,
        size: 0x1b04,
    },
    ResourceSpec {
        id: 0x307,
        address: 0x0873_5e84,
        size: 0x1b68,
    },
    ResourceSpec {
        id: 0x331,
        address: 0x0876_a00c,
        size: 0x0a1c,
    },
];

const CHIIKI: [ResourceSpec; 2] = [
    ResourceSpec {
        id: 0x1e5,
        address: 0x085b_b860,
        size: 0x182c,
    },
    ResourceSpec {
        id: 0x320,
        address: 0x0875_3b18,
        size: 0x2040,
    },
];

impl SeriesKind {
    fn kind_name(self) -> &'static str {
        match self {
            Self::Tokushu => "golden-sun-tokushu-map-series",
            Self::Chiiki => "golden-sun-chiiki-map-series",
        }
    }

    fn prefix(self) -> &'static str {
        match self {
            Self::Tokushu => "tokushu_",
            Self::Chiiki => "chiiki_",
        }
    }

    fn specs(self) -> &'static [ResourceSpec] {
        match self {
            Self::Tokushu => &TOKUSHU,
            Self::Chiiki => &CHIIKI,
        }
    }

    fn label(self) -> &'static str {
        match self {
            Self::Tokushu => "special-map",
            Self::Chiiki => "regional-map",
        }
    }

    fn required_offsets(self) -> [bool; 6] {
        match self {
            Self::Tokushu => [true, true, true, true, false, true],
            Self::Chiiki => [true, true, true, false, false, true],
        }
    }
}

fn read(path: &Path) -> Result<Vec<u8>> {
    fs::read(path).map_err(|error| format!("{}: {error}", path.display()))
}

fn json(path: &Path, label: &str) -> Result<Value> {
    let text = String::from_utf8(read(path)?).map_err(|error| error.to_string())?;
    let value: Value = serde_json::from_str(&text).map_err(|error| format!("{label}: {error}"))?;
    if !is_canonical_json_text(&text, &value) {
        return Err(format!("{label} is not canonical JSON"));
    }
    Ok(value)
}

fn object<'a>(value: &'a Value, label: &str) -> Result<&'a Map<String, Value>> {
    value
        .as_object()
        .ok_or_else(|| format!("{label} must be an object"))
}

fn field<'a>(value: &'a Map<String, Value>, name: &str) -> Result<&'a Value> {
    value.get(name).ok_or_else(|| format!("missing {name}"))
}

fn number(value: &Value, label: &str) -> Result<usize> {
    if let Some(value) = value.as_u64() {
        return usize::try_from(value).map_err(|_| format!("{label} is too large"));
    }
    let text = value
        .as_str()
        .ok_or_else(|| format!("{label} must be an integer"))?;
    let parsed = text
        .strip_prefix("0x")
        .map_or_else(
            || text.parse::<usize>(),
            |digits| usize::from_str_radix(digits, 16),
        )
        .map_err(|_| format!("{label} must be an integer"))?;
    Ok(parsed)
}

fn text<'a>(value: &'a Value, label: &str) -> Result<&'a str> {
    value
        .as_str()
        .ok_or_else(|| format!("{label} must be a string"))
}

fn id_text(id: usize) -> String {
    format!("{id:03x}")
}

fn parse_id(value: &str, label: &str) -> Result<usize> {
    usize::from_str_radix(value.strip_prefix("0x").unwrap_or(value), 16)
        .map_err(|_| format!("{label} must be hexadecimal"))
}

fn parse_index(index_path: &Path, kind: SeriesKind) -> Result<Vec<IndexEntry>> {
    let value = json(index_path, &format!("{} index", kind.label()))?;
    let index = object(&value, &format!("{} index", kind.label()))?;
    if number(field(index, "format")?, "format")? != 1
        || text(field(index, "kind")?, "kind")? != kind.kind_name()
    {
        return Err(format!("unsupported {} index", kind.label()));
    }
    let resources = field(index, "resources")?
        .as_array()
        .ok_or_else(|| format!("{} resources must be an array", kind.label()))?;
    let specs = kind.specs();
    if resources.len() != specs.len() {
        return Err(format!(
            "{} index has a different resource count",
            kind.label()
        ));
    }
    resources
        .iter()
        .enumerate()
        .map(|(position, value)| {
            let entry = object(value, &format!("{} resource entry", kind.label()))?;
            let id = parse_id(text(field(entry, "id")?, "resource id")?, "resource id")?;
            let address = number(field(entry, "address")?, "resource address")?;
            let size = number(field(entry, "size")?, "resource size")?;
            let directory = text(field(entry, "directory")?, "resource directory")?.to_string();
            let spec = specs[position];
            if id != spec.id
                || address != spec.address
                || size != spec.size
                || directory != format!("resource_{:03x}", spec.id)
            {
                return Err(format!(
                    "{} resource entry differs from the audited catalog",
                    kind.label()
                ));
            }
            Ok(IndexEntry {
                id,
                address,
                directory,
            })
        })
        .collect()
}

fn component_offsets(header: &[u8], size: usize, kind: SeriesKind) -> Result<[usize; 6]> {
    if header.len() != HEADER_SIZE || size <= HEADER_SIZE {
        return Err(format!("invalid {} container extent", kind.label()));
    }
    let mut offsets = [0usize; 6];
    for (slot, offset) in offsets.iter_mut().enumerate() {
        let start = 0x24 + slot * 4;
        *offset = u32::from_le_bytes(header[start..start + 4].try_into().unwrap()) as usize;
    }
    for (slot, required) in kind.required_offsets().into_iter().enumerate() {
        if required && offsets[slot] == 0 {
            return Err(format!("{} container lacks component {slot}", kind.label()));
        }
    }
    if offsets[0] != HEADER_SIZE {
        return Err(format!(
            "{} container has an invalid header offset",
            kind.label()
        ));
    }
    let mut previous = HEADER_SIZE - 1;
    for offset in offsets {
        if offset == 0 {
            continue;
        }
        if offset <= previous || offset >= size {
            return Err(format!(
                "{} component offsets are not ordered",
                kind.label()
            ));
        }
        previous = offset;
    }
    Ok(offsets)
}

fn component_end(offsets: &[usize; 6], slot: usize, size: usize) -> Result<usize> {
    let start = offsets[slot];
    if start == 0 {
        return Err("map component is absent".into());
    }
    Ok(offsets[slot + 1..]
        .iter()
        .copied()
        .find(|offset| *offset != 0)
        .unwrap_or(size))
}

fn component_sources(directory: &Path, slot: usize) -> Result<Vec<PathBuf>> {
    let stem = directory.display().to_string();
    Ok(match slot {
        0 => vec![
            PathBuf::from(format!("{stem}_metatiles.tilemap")),
            PathBuf::from(format!("{stem}_metatiles.lz.json")),
        ],
        1 => vec![
            PathBuf::from(format!("{stem}_descriptors.json")),
            PathBuf::from(format!("{stem}_descriptors.lz.json")),
        ],
        2 => vec![
            PathBuf::from(format!("{stem}_grid_grid.kind1.json")),
            PathBuf::from(format!("{stem}_grid_value_low.png")),
            PathBuf::from(format!("{stem}_grid_value_high.png")),
            PathBuf::from(format!("{stem}_grid_attribute_a.png")),
            PathBuf::from(format!("{stem}_grid_attribute_b.png")),
            PathBuf::from(format!("{stem}_grid_sentinels.png")),
        ],
        3 => vec![
            PathBuf::from(format!("{stem}_animation_queues.json")),
            PathBuf::from(format!("{stem}_animation_queues.lz.json")),
        ],
        4 => vec![
            PathBuf::from(format!("{stem}_blend_animation.json")),
            PathBuf::from(format!("{stem}_blend_animation.lz.json")),
        ],
        5 => vec![PathBuf::from(format!("{stem}_sparse_cells.json"))],
        _ => return Err("unsupported map component slot".into()),
    })
}

fn build_component(directory: &Path, slot: usize, expected: usize) -> Result<Vec<u8>> {
    let sources = component_sources(directory, slot)?;
    let built = match slot {
        0 => build_metatiles(&sources[0], &sources[1]),
        1 => build_descriptors(&sources[0], &sources[1]),
        2 => build_grid(&json(&sources[0], "map grid plan")?, directory),
        3 => build_queues(&sources[0], &sources[1]),
        4 => build_blend_animation(&sources[0], &sources[1]),
        5 => build_sparse(&sources[0]),
        _ => return Err("unsupported map component slot".into()),
    }
    .map_err(|error| format!("map component {slot}: {error}"))?;
    if built.len() != expected {
        return Err(format!("map component {slot} has the wrong size"));
    }
    Ok(built)
}

fn build_resource(
    index_path: &Path,
    entry: &IndexEntry,
    spec: ResourceSpec,
    kind: SeriesKind,
) -> Result<BuiltMapResource> {
    let root = index_path
        .parent()
        .ok_or_else(|| "map index has no parent".to_string())?;
    let directory = root.join(format!("{}{}", kind.prefix(), entry.directory));
    let header_path = PathBuf::from(format!("{}_header.json", directory.display()));
    let header = build_header(&header_path, None).map_err(|error| error.to_string())?;
    let offsets = component_offsets(&header, spec.size, kind)?;
    let mut data = header;
    let mut sources = vec![header_path];
    for slot in 0..6 {
        if offsets[slot] == 0 {
            continue;
        }
        let end = component_end(&offsets, slot, spec.size)?;
        data.extend(build_component(&directory, slot, end - offsets[slot])?);
        sources.extend(component_sources(&directory, slot)?);
    }
    if data.len() != spec.size {
        return Err(format!(
            "resource {} has the wrong rebuilt size",
            id_text(entry.id)
        ));
    }
    Ok(BuiltMapResource {
        id: entry.id,
        address: entry.address,
        data,
        sources,
    })
}

pub fn build_series(index_path: &Path, kind: SeriesKind) -> Result<Vec<BuiltMapResource>> {
    let entries = parse_index(index_path, kind)?;
    entries
        .iter()
        .enumerate()
        .map(|(position, entry)| build_resource(index_path, entry, kind.specs()[position], kind))
        .collect()
}

pub fn build_resource_by_id(
    index_path: &Path,
    kind: SeriesKind,
    id: usize,
) -> Result<BuiltMapResource> {
    build_series(index_path, kind)?
        .into_iter()
        .find(|resource| resource.id == id)
        .ok_or_else(|| format!("resource {id:03x} is absent"))
}

pub fn verify_series(rom_path: &Path, index_path: &Path, kind: SeriesKind) -> Result<String> {
    let rom = read(rom_path)?;
    let resources = build_series(index_path, kind)?;
    let mut bytes = 0usize;
    for resource in &resources {
        let pointer = RESOURCE_TABLE
            .checked_sub(ROM_BASE)
            .and_then(|offset| offset.checked_add(resource.id * 4))
            .ok_or_else(|| "resource table is outside the ROM".to_string())?;
        if pointer + 8 > rom.len() {
            return Err("resource table is outside the ROM".into());
        }
        let actual = u32::from_le_bytes(rom[pointer..pointer + 4].try_into().unwrap()) as usize;
        let next = u32::from_le_bytes(rom[pointer + 4..pointer + 8].try_into().unwrap()) as usize;
        if actual != resource.address || next != resource.address + resource.data.len() {
            return Err(format!(
                "resource {} directory bounds differ",
                id_text(resource.id)
            ));
        }
        let start = resource
            .address
            .checked_sub(ROM_BASE)
            .ok_or_else(|| "resource lies before ROM base".to_string())?;
        let end = start + resource.data.len();
        if end > rom.len() || rom[start..end] != resource.data {
            return Err(format!(
                "resource {} differs from ROM",
                id_text(resource.id)
            ));
        }
        bytes += resource.data.len();
    }
    Ok(format!(
        "identical=true resources={} source_bytes={bytes}",
        resources.len()
    ))
}

pub fn self_test() -> Result<()> {
    components_self_test().map_err(|error| error.to_string())?;
    grid_self_test().map_err(|error| error.to_string())?;
    let mut header = vec![0u8; HEADER_SIZE];
    for (slot, offset) in [0x3c_u32, 0x80, 0xa0, 0x120, 0, 0x140]
        .into_iter()
        .enumerate()
    {
        header[0x24 + slot * 4..0x28 + slot * 4].copy_from_slice(&offset.to_le_bytes());
    }
    let offsets = component_offsets(&header, 0x148, SeriesKind::Tokushu)?;
    if component_end(&offsets, 3, 0x148)? != 0x140 || component_end(&offsets, 5, 0x148)? != 0x148 {
        return Err("map component extent self-test failed".into());
    }
    if component_offsets(&[0u8; HEADER_SIZE], 0x100, SeriesKind::Tokushu).is_ok() {
        return Err("empty map header was accepted".into());
    }
    Ok(())
}

pub fn run(args: Vec<String>, kind: SeriesKind) -> Result<()> {
    if args.len() == 1 && args[0] == "--self-test" {
        self_test()?;
        println!("self-test=ok");
        return Ok(());
    }
    let usage = format!(
        "usage: {} build-stdout INDEX RESOURCE_ID | series-stdout INDEX | build INDEX --output FILE | verify ROM --index INDEX | --self-test",
        kind.label()
    );
    if args.len() == 1 && matches!(args[0].as_str(), "-h" | "--help") {
        println!("{usage}");
        return Ok(());
    }
    match args.first().map(String::as_str) {
        Some("build-stdout") if args.len() == 3 => {
            let resource = build_resource_by_id(
                Path::new(&args[1]),
                kind,
                parse_id(&args[2], "resource id")?,
            )?;
            std::io::stdout()
                .write_all(&resource.data)
                .map_err(|error| error.to_string())?;
            Ok(())
        }
        Some("series-stdout") if args.len() == 2 => {
            for resource in build_series(Path::new(&args[1]), kind)? {
                std::io::stdout()
                    .write_all(&resource.data)
                    .map_err(|error| error.to_string())?;
            }
            Ok(())
        }
        Some("build") if args.len() == 4 && args[2] == "--output" => {
            let resources = build_series(Path::new(&args[1]), kind)?;
            let bytes: Vec<u8> = resources
                .iter()
                .flat_map(|resource| resource.data.iter().copied())
                .collect();
            fs::write(&args[3], &bytes).map_err(|error| error.to_string())?;
            println!("resources={} source_bytes={}", resources.len(), bytes.len());
            Ok(())
        }
        Some("verify") if args.len() == 4 && args[2] == "--index" => {
            println!(
                "{}",
                verify_series(Path::new(&args[1]), Path::new(&args[3]), kind)?
            );
            Ok(())
        }
        _ => Err(usage),
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn native_self_test() {
        self_test().unwrap();
    }

    #[test]
    fn catalogs_are_stable() {
        assert_eq!(TOKUSHU.len(), 6);
        assert_eq!(CHIIKI.len(), 2);
        assert_eq!(TOKUSHU.iter().map(|spec| spec.size).sum::<usize>(), 0x9b4c);
        assert_eq!(CHIIKI.iter().map(|spec| spec.size).sum::<usize>(), 0x386c);
    }
}
