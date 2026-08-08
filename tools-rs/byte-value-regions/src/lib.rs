// Regions of the ROM kept as literal byte values in JSON, for data no better
// representation has been found for yet.
//
// Ported from tools/make/byte_value_regions.ts. Every identity check in the
// TypeScript survives, because this reader is the thing that decides whether a
// tracked JSON file still describes the ROM: the canonical-text check, the
// exact field set per region, the name and address shapes, and the requirement
// that regions be nonempty and strictly ascending with no overlap.

use canonical_json::{canonical_json, is_canonical_json_text};
use serde_json::{json, Value};
use std::fs;
use std::io;
use std::path::Path;

pub const ROM_BASE: u32 = 0x0800_0000;

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct ByteValueRegion {
    pub address: u32,
    pub data: Vec<u8>,
}

#[derive(Debug)]
pub enum RegionError {
    Io(io::Error),
    Json(String),
    SourceIdentityDiffers,
    RegionFieldsDiffer(usize),
    RegionDiffers(usize),
    RegionAddressDiffers(usize),
    RegionByteDiffers { region: usize, offset: usize },
    RegionOrderingDiffers(usize),
    OutsideRom(String),
    DoesNotRoundTrip(usize),
}

impl std::fmt::Display for RegionError {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self {
            Self::Io(error) => write!(f, "{error}"),
            Self::Json(message) => write!(f, "{message}"),
            Self::SourceIdentityDiffers => write!(f, "byte-value source identity differs"),
            Self::RegionFieldsDiffer(index) => write!(f, "region {index} fields differ"),
            Self::RegionDiffers(index) => write!(f, "region {index} differs"),
            Self::RegionAddressDiffers(index) => write!(f, "region {index} address differs"),
            Self::RegionByteDiffers { region, offset } => {
                write!(f, "region {region} byte {offset} differs")
            }
            Self::RegionOrderingDiffers(index) => write!(f, "region {index} ordering differs"),
            Self::OutsideRom(name) => write!(f, "{name} lies outside ROM"),
            Self::DoesNotRoundTrip(index) => write!(f, "region {index} differs"),
        }
    }
}

impl std::error::Error for RegionError {}

impl From<io::Error> for RegionError {
    fn from(error: io::Error) -> Self {
        Self::Io(error)
    }
}

/// The TypeScript matched `/^0x08[0-7][0-9a-f]{5}$/`: a lowercase eight-digit
/// cartridge address inside the 8 MiB window. Nothing looser is accepted,
/// because a differently-spelled address would round-trip to a different file.
fn parse_address(value: &Value) -> Option<u32> {
    let text = value.as_str()?;
    let digits = text.strip_prefix("0x")?;
    if digits.len() != 8 || !digits.starts_with("08") {
        return None;
    }
    if !matches!(digits.as_bytes()[2], b'0'..=b'7') {
        return None;
    }
    if !digits[3..].bytes().all(|byte| byte.is_ascii_digit() || (b'a'..=b'f').contains(&byte)) {
        return None;
    }
    u32::from_str_radix(digits, 16).ok()
}

fn is_region_name(name: &str) -> bool {
    !name.is_empty()
        && name.bytes().all(|byte| byte.is_ascii_lowercase() || byte.is_ascii_digit() || byte == b'_')
}

fn hex(value: u32) -> String {
    format!("0x{value:08x}")
}

pub fn build_byte_value_regions(path: &Path) -> Result<Vec<ByteValueRegion>, RegionError> {
    let text = fs::read_to_string(path)?;
    let value: Value = serde_json::from_str(&text).map_err(|e| RegionError::Json(e.to_string()))?;
    if !value.is_object()
        || !is_canonical_json_text(&text, &value)
        || value.get("format").and_then(Value::as_u64) != Some(1)
        || value.get("kind").and_then(Value::as_str) != Some("golden-sun-byte-value-regions")
    {
        return Err(RegionError::SourceIdentityDiffers);
    }
    let regions = value
        .get("regions")
        .and_then(Value::as_array)
        .ok_or(RegionError::SourceIdentityDiffers)?;

    // `previous` is the last byte already claimed, so the first region only has
    // to start at or after the ROM base.
    let mut previous = ROM_BASE.wrapping_sub(1);
    let mut built = Vec::with_capacity(regions.len());
    for (index, raw) in regions.iter().enumerate() {
        let item = raw.as_object().ok_or(RegionError::RegionDiffers(index))?;
        let mut fields: Vec<&str> = item.keys().map(String::as_str).collect();
        fields.sort_unstable();
        if fields != ["address", "name", "representation", "values"] {
            return Err(RegionError::RegionFieldsDiffer(index));
        }
        let named = item.get("name").and_then(Value::as_str).is_some_and(is_region_name);
        let representation = item.get("representation").and_then(Value::as_str) == Some("byte_values");
        let values = item.get("values").and_then(Value::as_array);
        let (true, true, Some(values)) = (named, representation, values) else {
            return Err(RegionError::RegionDiffers(index));
        };
        let address = parse_address(&item["address"])
            .ok_or(RegionError::RegionAddressDiffers(index))?;
        if address <= previous || values.is_empty() {
            return Err(RegionError::RegionOrderingDiffers(index));
        }
        let mut data = Vec::with_capacity(values.len());
        for (offset, entry) in values.iter().enumerate() {
            let byte = entry
                .as_u64()
                .filter(|value| *value <= 0xff)
                .ok_or(RegionError::RegionByteDiffers { region: index, offset })?;
            data.push(byte as u8);
        }
        previous = address + data.len() as u32 - 1;
        built.push(ByteValueRegion { address, data });
    }
    Ok(built)
}

pub struct RegionSpec {
    pub name: String,
    pub address: u32,
    pub size: usize,
}

fn rom_slice(rom: &[u8], address: u32, size: usize) -> Option<&[u8]> {
    let start = address.checked_sub(ROM_BASE)? as usize;
    rom.get(start..start.checked_add(size)?)
}

/// Write `index.json` for the given regions, then read it back and compare
/// every byte against the ROM before returning.
pub fn export_byte_value_regions(
    rom: &[u8],
    directory: &Path,
    regions: &[RegionSpec],
) -> Result<(), RegionError> {
    let mut described = Vec::with_capacity(regions.len());
    for region in regions {
        let data = rom_slice(rom, region.address, region.size)
            .ok_or_else(|| RegionError::OutsideRom(region.name.clone()))?;
        described.push(json!({
            "name": region.name,
            "address": hex(region.address),
            "representation": "byte_values",
            "values": data,
        }));
    }
    let source = json!({
        "format": 1,
        "kind": "golden-sun-byte-value-regions",
        "regions": described,
    });

    fs::create_dir_all(directory)?;
    let output = directory.join("index.json");
    fs::write(&output, format!("{}\n", canonical_json(&source)))?;

    for (index, region) in build_byte_value_regions(&output)?.iter().enumerate() {
        if rom_slice(rom, region.address, region.data.len()) != Some(region.data.as_slice()) {
            return Err(RegionError::DoesNotRoundTrip(index));
        }
    }
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;

    fn scratch(name: &str) -> std::path::PathBuf {
        let dir = std::env::temp_dir()
            .join(format!("alchemy-byte-value-regions-{name}-{}", std::process::id()));
        let _ = fs::remove_dir_all(&dir);
        fs::create_dir_all(&dir).unwrap();
        dir
    }

    fn rom() -> Vec<u8> {
        (0..0x1000u32).map(|index| (index.wrapping_mul(31) & 0xff) as u8).collect()
    }

    fn specs() -> Vec<RegionSpec> {
        vec![
            RegionSpec { name: "residual_000".into(), address: ROM_BASE + 0x10, size: 8 },
            RegionSpec { name: "residual_001".into(), address: ROM_BASE + 0x40, size: 16 },
        ]
    }

    #[test]
    fn export_round_trips() {
        let dir = scratch("roundtrip");
        export_byte_value_regions(&rom(), &dir, &specs()).unwrap();
        let built = build_byte_value_regions(&dir.join("index.json")).unwrap();
        assert_eq!(built.len(), 2);
        assert_eq!(built[0].address, ROM_BASE + 0x10);
        assert_eq!(built[0].data, rom()[0x10..0x18]);
        assert_eq!(built[1].data, rom()[0x40..0x50]);
        fs::remove_dir_all(&dir).unwrap();
    }

    #[test]
    fn a_region_outside_the_rom_is_refused() {
        let dir = scratch("outside");
        let specs = vec![RegionSpec { name: "far".into(), address: ROM_BASE + 0xffff, size: 4 }];
        assert!(matches!(
            export_byte_value_regions(&rom(), &dir, &specs),
            Err(RegionError::OutsideRom(name)) if name == "far"
        ));
        fs::remove_dir_all(&dir).unwrap();
    }

    #[test]
    fn addresses_must_be_spelled_exactly() {
        assert_eq!(parse_address(&json!("0x08000010")), Some(ROM_BASE + 0x10));
        assert_eq!(parse_address(&json!("0x0800001")), None, "seven digits");
        assert_eq!(parse_address(&json!("0x08000010 ")), None, "trailing space");
        assert_eq!(parse_address(&json!("0x0800001A")), None, "uppercase");
        assert_eq!(parse_address(&json!("0x09000010")), None, "wrong region");
        assert_eq!(parse_address(&json!("0x08800010")), None, "past the 8 MiB window");
        assert_eq!(parse_address(&json!(134217744)), None, "not a string");
    }

    #[test]
    fn a_non_canonical_file_is_refused() {
        let dir = scratch("noncanonical");
        export_byte_value_regions(&rom(), &dir, &specs()).unwrap();
        let path = dir.join("index.json");
        let text = fs::read_to_string(&path).unwrap();
        // Four-space indentation is none of the three accepted spellings.
        let loosened = text.replace('\n', "\n    ");
        assert_ne!(loosened, text);
        fs::write(&path, loosened).unwrap();
        assert!(matches!(
            build_byte_value_regions(&path),
            Err(RegionError::SourceIdentityDiffers)
        ));
        fs::remove_dir_all(&dir).unwrap();
    }

    #[test]
    fn overlapping_and_malformed_regions_are_refused() {
        let dir = scratch("tamper");
        let path = dir.join("index.json");
        let write = |value: &Value| fs::write(&path, format!("{}\n", canonical_json(value))).unwrap();
        let base = || {
            json!({
                "format": 1,
                "kind": "golden-sun-byte-value-regions",
                "regions": [
                    { "name": "a", "address": hex(ROM_BASE + 0x10), "representation": "byte_values", "values": [1, 2, 3, 4] },
                    { "name": "b", "address": hex(ROM_BASE + 0x20), "representation": "byte_values", "values": [5] },
                ],
            })
        };
        write(&base());
        assert_eq!(build_byte_value_regions(&path).unwrap().len(), 2);

        // The second region now starts inside the first.
        let mut overlapping = base();
        overlapping["regions"][1]["address"] = json!(hex(ROM_BASE + 0x12));
        write(&overlapping);
        assert!(matches!(
            build_byte_value_regions(&path),
            Err(RegionError::RegionOrderingDiffers(1))
        ));

        let mut empty = base();
        empty["regions"][1]["values"] = json!([]);
        write(&empty);
        assert!(matches!(
            build_byte_value_regions(&path),
            Err(RegionError::RegionOrderingDiffers(1))
        ));

        let mut wide = base();
        wide["regions"][0]["values"] = json!([1, 256, 3, 4]);
        write(&wide);
        assert!(matches!(
            build_byte_value_regions(&path),
            Err(RegionError::RegionByteDiffers { region: 0, offset: 1 })
        ));

        let mut extra = base();
        extra["regions"][0]["note"] = json!("hello");
        write(&extra);
        assert!(matches!(build_byte_value_regions(&path), Err(RegionError::RegionFieldsDiffer(0))));

        let mut renamed = base();
        renamed["regions"][0]["name"] = json!("Not A Name");
        write(&renamed);
        assert!(matches!(build_byte_value_regions(&path), Err(RegionError::RegionDiffers(0))));

        fs::remove_dir_all(&dir).unwrap();
    }
}
