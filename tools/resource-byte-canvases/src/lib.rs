//! Native codec for tools/make/resource_byte_canvases.ts.
//!
//! Each pixel is one ROM byte and the canvas is padded with zeroes to a
//! multiple of eight rows. The TypeScript module remains available as a
//! library for the native asset builder; this crate provides standalone commands.

use std::fs;
use std::path::{Path, PathBuf};

use canonical_json::{canonical_json, is_canonical_json_text};
use export_asset::byte_png;
use import_asset::indexed_png;
use serde_json::{json, Map, Value};

pub const ROM_BASE: u64 = 0x0800_0000;
pub const SOURCE_BYTES: usize = 8528;

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct ResourceSpec {
    pub id: &'static str,
    pub address: u64,
    pub size: usize,
    pub source: &'static str,
}

pub const RESOURCES: [ResourceSpec; 8] = [
    ResourceSpec {
        id: "049",
        address: 0x083c_d090,
        size: 0x488,
        source: "resource_049.png",
    },
    ResourceSpec {
        id: "04a",
        address: 0x083c_d518,
        size: 0x5ec,
        source: "resource_04a.png",
    },
    ResourceSpec {
        id: "061",
        address: 0x083d_85e0,
        size: 0x444,
        source: "resource_061.png",
    },
    ResourceSpec {
        id: "064",
        address: 0x083d_c1f8,
        size: 0x220,
        source: "resource_064.png",
    },
    ResourceSpec {
        id: "066",
        address: 0x083d_c854,
        size: 0x114,
        source: "resource_066.png",
    },
    ResourceSpec {
        id: "073",
        address: 0x083e_2ae4,
        size: 0x29c,
        source: "resource_073.png",
    },
    ResourceSpec {
        id: "0a5",
        address: 0x083f_5f4c,
        size: 0x7b0,
        source: "resource_0a5.png",
    },
    ResourceSpec {
        id: "0a6",
        address: 0x083f_66fc,
        size: 0x518,
        source: "resource_0a6.png",
    },
];

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct BuiltResource {
    pub id: String,
    pub address: u64,
    pub data: Vec<u8>,
    pub source: String,
}

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

fn width(_: usize) -> usize {
    32
}

pub fn height(size: usize) -> usize {
    let rows = size.div_ceil(width(size));
    rows.div_ceil(8) * 8
}

fn read(path: &Path) -> Result<Vec<u8>, Error> {
    fs::read(path).map_err(|error| err(format!("{}: {error}", path.display())))
}

fn write(path: &Path, data: &[u8]) -> Result<(), Error> {
    fs::write(path, data).map_err(|error| err(format!("{}: {error}", path.display())))
}

fn resource_json(resource: ResourceSpec) -> Value {
    json!({
        "id": resource.id,
        "address": resource.address,
        "size": resource.size,
        "source": resource.source,
    })
}

pub fn index_json() -> Value {
    let mut index = Map::new();
    index.insert("format".into(), json!(1));
    index.insert(
        "kind".into(),
        json!("golden-sun-resource-byte-canvas-series"),
    );
    index.insert(
        "resources".into(),
        Value::Array(RESOURCES.iter().copied().map(resource_json).collect()),
    );
    Value::Object(index)
}

fn number(value: &Value) -> Option<u64> {
    value.as_u64().or_else(|| {
        value.as_str().and_then(|text| {
            text.strip_prefix("0x")
                .or_else(|| text.strip_prefix("0X"))
                .map_or_else(
                    || text.parse().ok(),
                    |hex| u64::from_str_radix(hex, 16).ok(),
                )
        })
    })
}

fn parse_index(path: &Path) -> Result<(), Error> {
    let text = String::from_utf8(read(path)?).map_err(|_| err("resource canvas index differs"))?;
    let value: Value =
        serde_json::from_str(&text).map_err(|_| err("resource canvas index differs"))?;
    if !is_canonical_json_text(&text, &value) {
        return Err(err("resource canvas index differs"));
    }
    let Some(object) = value.as_object() else {
        return Err(err("resource canvas index differs"));
    };
    if object.get("format").and_then(number) != Some(1)
        || object.get("kind").and_then(Value::as_str)
            != Some("golden-sun-resource-byte-canvas-series")
    {
        return Err(err("resource canvas index identity differs"));
    }
    let Some(resources) = object.get("resources").and_then(Value::as_array) else {
        return Err(err("resource canvas index identity differs"));
    };
    if resources.len() != RESOURCES.len() {
        return Err(err("resource canvas index identity differs"));
    }
    for (index, value) in resources.iter().enumerate() {
        let Some(item) = value.as_object() else {
            return Err(err(format!("resource canvas {index} layout differs")));
        };
        let expected = RESOURCES[index];
        let matches = item.get("id").and_then(Value::as_str) == Some(expected.id)
            && item.get("address").and_then(number) == Some(expected.address)
            && item.get("size").and_then(number) == Some(expected.size as u64)
            && item.get("source").and_then(Value::as_str) == Some(expected.source);
        if !matches {
            return Err(err(format!("resource canvas {index} layout differs")));
        }
    }
    Ok(())
}

fn image_path(index_path: &Path, source: &str) -> PathBuf {
    let directory = index_path.parent().unwrap_or_else(|| Path::new("."));
    let basename = index_path
        .file_name()
        .and_then(|name| name.to_str())
        .unwrap_or_default();
    let prefix = basename.strip_suffix("index.json").unwrap_or(basename);
    directory.join(format!("{prefix}{source}"))
}

pub fn build_resource_byte_canvases(index_path: &Path) -> Result<Vec<BuiltResource>, Error> {
    parse_index(index_path)?;
    RESOURCES
        .iter()
        .map(|resource| {
            let path = image_path(index_path, resource.source);
            let encoded = read(&path)?;
            let image = indexed_png(&encoded).map_err(|error| err(error.0))?;
            let expected_width = width(resource.size);
            let expected_height = height(resource.size);
            if image.width as usize != expected_width
                || image.height as usize != expected_height
                || image.pixels.len() != expected_width * expected_height
                || image.pixels[resource.size..]
                    .iter()
                    .any(|pixel| *pixel != 0)
            {
                return Err(err(format!("resource {} canvas differs", resource.id)));
            }
            let canvas: Vec<u8> = image.pixels.iter().map(|pixel| *pixel as u8).collect();
            let canonical = byte_png(&canvas, expected_width as f64)
                .map_err(|error| err(error.0))?
                .0;
            if canonical != encoded {
                return Err(err(format!(
                    "resource {} canvas is not canonical",
                    resource.id
                )));
            }
            Ok(BuiltResource {
                id: resource.id.to_string(),
                address: resource.address,
                data: canvas[..resource.size].to_vec(),
                source: resource.source.to_string(),
            })
        })
        .collect()
}

fn rom_region<'a>(rom: &'a [u8], resource: ResourceSpec) -> Result<&'a [u8], Error> {
    let start = resource
        .address
        .checked_sub(ROM_BASE)
        .ok_or_else(|| err(format!("resource {} lies outside the ROM", resource.id)))?
        as usize;
    let end = start
        .checked_add(resource.size)
        .ok_or_else(|| err(format!("resource {} lies outside the ROM", resource.id)))?;
    rom.get(start..end)
        .ok_or_else(|| err(format!("resource {} lies outside the ROM", resource.id)))
}

fn spec_for_id(id: &str) -> Result<ResourceSpec, Error> {
    RESOURCES
        .iter()
        .copied()
        .find(|candidate| candidate.id == id)
        .ok_or_else(|| err("resource canvas index identity differs"))
}

fn directory_index(directory: &Path) -> Result<PathBuf, Error> {
    let conventional = directory.join("index.json");
    if conventional.is_file() && parse_index(&conventional).is_ok() {
        return Ok(conventional);
    }
    let candidates = fs::read_dir(directory)
        .map_err(|error| err(format!("{}: {error}", directory.display())))?
        .filter_map(Result::ok)
        .map(|entry| entry.path())
        .filter(|path| {
            path.is_file()
                && path
                    .file_name()
                    .and_then(|name| name.to_str())
                    .is_some_and(|name| name.ends_with("_index.json"))
        })
        .filter(|path| parse_index(path).is_ok())
        .collect::<Vec<_>>();
    if candidates.len() == 1 {
        return Ok(candidates[0].clone());
    }
    Err(err("resource canvas index differs"))
}

pub fn export_resource_byte_canvases(rom_path: &Path, directory: &Path) -> Result<(), Error> {
    let rom = read(rom_path)?;
    fs::create_dir_all(directory)
        .map_err(|error| err(format!("{}: {error}", directory.display())))?;
    write(
        &directory.join("index.json"),
        format!("{}\n", canonical_json(&index_json())).as_bytes(),
    )?;
    for resource in RESOURCES {
        let data = rom_region(&rom, resource)?;
        let mut canvas = vec![0; width(resource.size) * height(resource.size)];
        canvas[..resource.size].copy_from_slice(data);
        let encoded = byte_png(&canvas, width(resource.size) as f64)
            .map_err(|error| err(error.0))?
            .0;
        write(&directory.join(resource.source), &encoded)?;
    }
    for resource in build_resource_byte_canvases(&directory.join("index.json"))? {
        if resource.data != rom_region(&rom, spec_for_id(&resource.id)?)? {
            return Err(err(format!("resource {} does not round-trip", resource.id)));
        }
    }
    Ok(())
}

pub fn verify_resource_byte_canvases(rom_path: &Path, directory: &Path) -> Result<(), Error> {
    let rom = read(rom_path)?;
    let built = build_resource_byte_canvases(&directory_index(directory)?)?;
    if built.is_empty() {
        return Err(err("no resource canvases were verified"));
    }
    for resource in &built {
        if resource.data != rom_region(&rom, spec_for_id(&resource.id)?)? {
            return Err(err(format!("resource {} differs from ROM", resource.id)));
        }
    }
    Ok(())
}

pub fn self_test() -> Result<(), Error> {
    if RESOURCES
        .iter()
        .map(|resource| resource.size)
        .sum::<usize>()
        != SOURCE_BYTES
        || RESOURCES
            .iter()
            .any(|resource| height(resource.size) % 8 != 0)
    {
        return Err(err("resource canvas layout differs"));
    }
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;
    use std::fs;
    use std::time::{SystemTime, UNIX_EPOCH};

    #[test]
    fn audited_layout_and_canonical_index_match() {
        self_test().expect("layout");
        let text = format!("{}\n", canonical_json(&index_json()));
        let value: Value = serde_json::from_str(&text).expect("index JSON");
        assert!(is_canonical_json_text(&text, &value));
        assert_eq!(RESOURCES.len(), 8);
        assert_eq!(SOURCE_BYTES, 8528);
    }

    #[test]
    fn dimensions_are_padded_to_eight_rows() {
        assert_eq!(height(0x488), 40);
        assert_eq!(height(0x5ec), 48);
        assert_eq!(height(0x114), 16);
        assert_eq!(height(0x7b0), 64);
    }

    #[test]
    fn export_and_verify_round_trip_a_synthetic_rom() {
        let stamp = SystemTime::now()
            .duration_since(UNIX_EPOCH)
            .expect("clock")
            .as_nanos();
        let root = std::env::temp_dir().join(format!("alchemy-resource-byte-canvases-{stamp}"));
        let rom_path = root.join("baserom.gba");
        let directory = root.join("assets");
        let mut rom = vec![0u8; 0x0080_0000];
        for resource in RESOURCES {
            let start = (resource.address - ROM_BASE) as usize;
            for (offset, byte) in rom[start..start + resource.size].iter_mut().enumerate() {
                *byte = (offset as u8)
                    .wrapping_mul(37)
                    .wrapping_add(resource.id.as_bytes()[0]);
            }
        }
        fs::create_dir_all(&root).expect("root");
        fs::write(&rom_path, rom).expect("ROM");
        export_resource_byte_canvases(&rom_path, &directory).expect("export");
        verify_resource_byte_canvases(&rom_path, &directory).expect("verify");
        let _ = fs::remove_dir_all(root);
    }
}
