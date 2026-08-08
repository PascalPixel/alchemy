// The map graphics load table: 186 records of six u16 resource offsets each,
// stored as JSON with the offsets already biased by RESOURCE_BASE.
//
// Ported from tools/lib/map_load_table.ts. The export still round-trips through
// the builder and compares against the ROM before it returns, because that
// check is the only thing standing between a silent field-order slip and a
// wrong table.

use canonical_json::canonical_json;
use serde_json::{json, Map, Value};
use std::fs;
use std::io;
use std::path::Path;

pub const ROM_BASE: usize = 0x0800_0000;
pub const TABLE_ADDRESS: usize = 0x0801_3784;
pub const RESOURCE_BASE: u32 = 0x128;
pub const RECORDS: usize = 186;
pub const RECORD_SIZE: usize = 12;
pub const FIELDS: [&str; 6] = [
    "container",
    "palette",
    "vram_charblock1",
    "vram_charblock2",
    "vram_charblock3",
    "animation_source",
];

#[derive(Debug)]
pub enum TableError {
    Io(io::Error),
    Json(String),
    UnsupportedFormat,
    WrongResourceBase,
    WrongRecordCount(usize),
    NonSequentialIndex(usize),
    OffsetOutsideU16 { record: usize, field: &'static str },
    RomTooSmall,
    DoesNotRoundTrip,
}

impl std::fmt::Display for TableError {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self {
            Self::Io(error) => write!(f, "{error}"),
            Self::Json(message) => write!(f, "{message}"),
            Self::UnsupportedFormat => write!(f, "unsupported map graphics load table"),
            Self::WrongResourceBase => {
                write!(f, "map graphics load table has the wrong resource base")
            }
            Self::WrongRecordCount(count) => {
                write!(
                    f,
                    "map graphics load table requires {RECORDS} records, found {count}"
                )
            }
            Self::NonSequentialIndex(index) => {
                write!(
                    f,
                    "map graphics load table indices must be sequential at {index}"
                )
            }
            Self::OffsetOutsideU16 { record, field } => {
                write!(
                    f,
                    "record {record} field {field}: resource offset is outside u16"
                )
            }
            Self::RomTooSmall => write!(f, "rom does not contain the map graphics load table"),
            Self::DoesNotRoundTrip => write!(f, "map graphics load table does not round-trip"),
        }
    }
}

impl std::error::Error for TableError {}

impl From<io::Error> for TableError {
    fn from(error: io::Error) -> Self {
        Self::Io(error)
    }
}

fn table_slice(rom: &[u8]) -> Result<&[u8], TableError> {
    let start = TABLE_ADDRESS - ROM_BASE;
    rom.get(start..start + RECORDS * RECORD_SIZE)
        .ok_or(TableError::RomTooSmall)
}

/// Write the table as JSON, then rebuild it and compare against the ROM.
/// Returns the record count.
pub fn export_table(rom: &[u8], source: &Path) -> Result<usize, TableError> {
    let table = table_slice(rom)?;
    let mut records = Vec::with_capacity(RECORDS);
    for map_index in 0..RECORDS {
        let mut row = Map::new();
        row.insert("map_index".into(), json!(map_index));
        for (index, field) in FIELDS.iter().enumerate() {
            let at = map_index * RECORD_SIZE + index * 2;
            let raw = u16::from_le_bytes([table[at], table[at + 1]]) as u32;
            row.insert((*field).into(), json!(format!("{:x}", raw + RESOURCE_BASE)));
        }
        records.push(Value::Object(row));
    }
    let document = json!({
        "format": 1,
        "resource_base": format!("0x{RESOURCE_BASE:x}"),
        "records": records,
    });

    if let Some(parent) = source.parent() {
        fs::create_dir_all(parent)?;
    }
    fs::write(source, format!("{}\n", canonical_json(&document)))?;

    if build_table(source)? != table {
        return Err(TableError::DoesNotRoundTrip);
    }
    Ok(RECORDS)
}

fn read_document(source: &Path) -> Result<Value, TableError> {
    let text = fs::read_to_string(source)?;
    serde_json::from_str(&text).map_err(|error| TableError::Json(error.to_string()))
}

pub fn build_table(source: &Path) -> Result<Vec<u8>, TableError> {
    let document = read_document(source)?;
    if document.get("format").and_then(Value::as_u64) != Some(1) {
        return Err(TableError::UnsupportedFormat);
    }
    let base = document
        .get("resource_base")
        .and_then(Value::as_str)
        .and_then(|text| {
            text.strip_prefix("0x")
                .map(|digits| u32::from_str_radix(digits, 16))
                .unwrap_or_else(|| text.parse())
                .ok()
        })
        .ok_or(TableError::WrongResourceBase)?;
    if base != RESOURCE_BASE {
        return Err(TableError::WrongResourceBase);
    }
    let records = document
        .get("records")
        .and_then(Value::as_array)
        .ok_or(TableError::UnsupportedFormat)?;
    if records.len() != RECORDS {
        return Err(TableError::WrongRecordCount(records.len()));
    }

    let mut encoded = vec![0u8; RECORDS * RECORD_SIZE];
    for (map_index, row) in records.iter().enumerate() {
        if row.get("map_index").and_then(Value::as_u64) != Some(map_index as u64) {
            return Err(TableError::NonSequentialIndex(map_index));
        }
        for (index, field) in FIELDS.iter().enumerate() {
            let value = row
                .get(*field)
                .and_then(Value::as_str)
                .and_then(|text| u32::from_str_radix(text, 16).ok())
                .and_then(|value| value.checked_sub(RESOURCE_BASE))
                .filter(|value| *value <= 0xffff)
                .ok_or(TableError::OffsetOutsideU16 {
                    record: map_index,
                    field,
                })?;
            let at = map_index * RECORD_SIZE + index * 2;
            encoded[at..at + 2].copy_from_slice(&(value as u16).to_le_bytes());
        }
    }
    Ok(encoded)
}

/// Group records by their lowercased container name, preserving record order.
pub fn records_by_container(source: &Path) -> Result<Vec<(String, Vec<Value>)>, TableError> {
    let document = read_document(source)?;
    let records = document
        .get("records")
        .and_then(Value::as_array)
        .ok_or(TableError::UnsupportedFormat)?;
    let mut result: Vec<(String, Vec<Value>)> = Vec::new();
    for row in records {
        let container = row
            .get("container")
            .and_then(Value::as_str)
            .ok_or(TableError::UnsupportedFormat)?
            .to_ascii_lowercase();
        match result.iter_mut().find(|(name, _)| *name == container) {
            Some((_, rows)) => rows.push(row.clone()),
            None => result.push((container, vec![row.clone()])),
        }
    }
    Ok(result)
}

#[cfg(test)]
mod tests {
    use super::*;

    fn scratch(name: &str) -> std::path::PathBuf {
        let dir = std::env::temp_dir().join(format!(
            "alchemy-map-load-table-{name}-{}",
            std::process::id()
        ));
        let _ = fs::remove_dir_all(&dir);
        fs::create_dir_all(&dir).unwrap();
        dir
    }

    /// A ROM-shaped buffer whose table region holds a recognisable pattern.
    fn rom() -> Vec<u8> {
        let start = TABLE_ADDRESS - ROM_BASE;
        let mut rom = vec![0u8; start + RECORDS * RECORD_SIZE];
        for (index, slot) in rom[start..].chunks_exact_mut(2).enumerate() {
            slot.copy_from_slice(&((index as u16).wrapping_mul(7)).to_le_bytes());
        }
        rom
    }

    #[test]
    fn export_round_trips_against_the_rom() {
        let dir = scratch("roundtrip");
        let source = dir.join("map_load_table.json");
        let rom = rom();
        assert_eq!(export_table(&rom, &source).unwrap(), RECORDS);
        assert_eq!(build_table(&source).unwrap(), table_slice(&rom).unwrap());
        fs::remove_dir_all(&dir).unwrap();
    }

    #[test]
    fn the_written_json_is_canonical() {
        let dir = scratch("canonical");
        let source = dir.join("map_load_table.json");
        export_table(&rom(), &source).unwrap();
        let text = fs::read_to_string(&source).unwrap();
        let value: Value = serde_json::from_str(&text).unwrap();
        assert!(canonical_json::is_canonical_json_text(&text, &value));
        fs::remove_dir_all(&dir).unwrap();
    }

    #[test]
    fn a_short_rom_is_rejected_rather_than_truncated() {
        assert!(matches!(
            export_table(&[0u8; 16], Path::new("/dev/null")),
            Err(TableError::RomTooSmall)
        ));
    }

    #[test]
    fn tampered_documents_are_rejected() {
        let dir = scratch("tamper");
        let source = dir.join("map_load_table.json");
        export_table(&rom(), &source).unwrap();
        let mut document = read_document(&source).unwrap();

        let drop_last = {
            let mut document = document.clone();
            document["records"].as_array_mut().unwrap().pop();
            document
        };
        fs::write(&source, canonical_json(&drop_last)).unwrap();
        assert!(matches!(
            build_table(&source),
            Err(TableError::WrongRecordCount(185))
        ));

        // An offset below the resource base cannot be encoded and must not wrap.
        document["records"][3]["palette"] = json!("1");
        fs::write(&source, canonical_json(&document)).unwrap();
        assert!(matches!(
            build_table(&source),
            Err(TableError::OffsetOutsideU16 {
                record: 3,
                field: "palette"
            })
        ));

        document["records"][3]["palette"] = json!("128");
        document["records"][3]["map_index"] = json!(9);
        fs::write(&source, canonical_json(&document)).unwrap();
        assert!(matches!(
            build_table(&source),
            Err(TableError::NonSequentialIndex(3))
        ));

        fs::remove_dir_all(&dir).unwrap();
    }

    #[test]
    fn records_group_by_container_in_order() {
        let dir = scratch("group");
        let source = dir.join("map_load_table.json");
        export_table(&rom(), &source).unwrap();
        let grouped = records_by_container(&source).unwrap();
        assert_eq!(
            grouped.iter().map(|(_, rows)| rows.len()).sum::<usize>(),
            RECORDS
        );
        assert!(grouped
            .iter()
            .all(|(name, _)| name.chars().all(|c| !c.is_ascii_uppercase())));
        fs::remove_dir_all(&dir).unwrap();
    }
}
