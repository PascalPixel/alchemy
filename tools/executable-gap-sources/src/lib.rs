//! Decode the typed executable-gap package used by the asset builder.

pub mod cli;

use serde_json::Value;
use std::path::Path;

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct BuiltSection {
    pub address: u64,
    pub data: Vec<u8>,
}

fn field<'a>(value: &'a Value, key: &str) -> Result<&'a Value, String> {
    value.get(key).ok_or_else(|| format!("missing {key}"))
}

fn address(value: &Value) -> Result<u64, String> {
    let text = value
        .as_str()
        .ok_or_else(|| "invalid address".to_string())?;
    u64::from_str_radix(text.strip_prefix("0x").ok_or("invalid address")?, 16)
        .map_err(|_| "invalid address".to_string())
}

fn words(values: &Value, width: usize) -> Result<Vec<u8>, String> {
    let values = values
        .as_array()
        .ok_or_else(|| "values are not an array".to_string())?;
    let mut data = Vec::with_capacity(values.len() * width);
    for value in values {
        let text = value.as_str().ok_or_else(|| "invalid word".to_string())?;
        if text.len() != width * 2 + 2 || !text.starts_with("0x") {
            return Err("invalid word".to_string());
        }
        let word = u32::from_str_radix(&text[2..], 16).map_err(|_| "invalid word".to_string())?;
        data.extend_from_slice(&word.to_le_bytes()[..width]);
    }
    Ok(data)
}

fn section_data(section: &Value) -> Result<Option<Vec<u8>>, String> {
    match field(section, "kind")?.as_str().unwrap_or("") {
        "u32-values" => Ok(Some(words(field(section, "values")?, 4)?)),
        "u32-records" => {
            let mut data = Vec::new();
            for record in field(section, "records")?
                .as_array()
                .ok_or("records are not an array")?
            {
                data.extend(words(field(record, "words")?, 4)?);
            }
            Ok(Some(data))
        }
        "u16-tables" => {
            let mut data = Vec::new();
            for table in field(section, "tables")?
                .as_array()
                .ok_or("tables are not an array")?
            {
                data.extend(words(field(table, "values")?, 2)?);
            }
            Ok(Some(data))
        }
        "arm" | "thumb" | "veneer" | "fill" => Ok(None),
        kind => Err(format!("unexpected section kind: {kind}")),
    }
}

pub fn build_executable_gap_data(path: &Path) -> Result<Vec<BuiltSection>, String> {
    let package: Value = serde_json::from_slice(&std::fs::read(path).map_err(|e| e.to_string())?)
        .map_err(|e| e.to_string())?;
    let mut sections = Vec::new();
    for gap in field(&package, "gaps")?
        .as_array()
        .ok_or("gaps are not an array")?
    {
        for section in field(gap, "sections")?
            .as_array()
            .ok_or("sections are not an array")?
        {
            if let Some(data) = section_data(section)? {
                sections.push(BuiltSection {
                    address: address(field(section, "address")?)?,
                    data,
                });
            }
        }
    }
    sections.sort_by_key(|section| section.address);
    Ok(sections)
}

pub fn build_section(path: &Path, wanted: u64) -> Result<Vec<u8>, String> {
    build_executable_gap_data(path)?
        .into_iter()
        .find(|section| section.address == wanted)
        .map(|section| section.data)
        .ok_or_else(|| format!("executable gap section 0x{wanted:08x} is not present"))
}
