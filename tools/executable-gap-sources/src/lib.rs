//! Native executable-gap package builder.
//!
//! The asset build consumes the typed portions of the executable-gap package.
//! Assembly export is owned by the overlay/discovery tools; this crate keeps
//! the deterministic build and ROM verification API.

mod armsrc;
mod discover;
mod js;
mod json;
mod toolchain;

use json::Value;
use std::path::Path;

pub const ROM_BASE: u64 = 0x0800_0000;
pub const PLAN_LIMIT: u64 = 0x080f_b792;

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct BuiltSection {
    pub address: u64,
    pub data: Vec<u8>,
}

fn object<'a>(value: &'a Value, name: &str) -> Result<&'a [(String, Value)], String> {
    match value {
        Value::Obj(members) => Ok(members),
        _ => Err(format!("{name} must be an object")),
    }
}

fn field<'a>(value: &'a Value, key: &str, name: &str) -> Result<&'a Value, String> {
    value
        .get(key)
        .ok_or_else(|| format!("{name} is missing {key}"))
}

fn number(value: &Value, name: &str) -> Result<u64, String> {
    let raw = value
        .as_f64()
        .ok_or_else(|| format!("{name} must be an integer"))?;
    if !raw.is_finite() || raw < 0.0 || raw.fract() != 0.0 {
        return Err(format!("{name} must be an integer"));
    }
    Ok(raw as u64)
}

fn text<'a>(value: &'a Value, name: &str) -> Result<&'a str, String> {
    value
        .as_str()
        .ok_or_else(|| format!("{name} must be a string"))
}

fn address(value: &Value, name: &str) -> Result<u64, String> {
    let raw = text(value, name)?;
    if raw.len() != 10
        || !raw.starts_with("0x")
        || !raw[2..].bytes().all(|byte| byte.is_ascii_hexdigit())
    {
        return Err(format!("invalid {name}"));
    }
    u64::from_str_radix(&raw[2..], 16).map_err(|_| format!("invalid {name}"))
}

fn hex_address(value: u64) -> String {
    format!("0x{value:08x}")
}

fn canonical_document(path: &Path) -> Result<Value, String> {
    let bytes = std::fs::read(path).map_err(|e| format!("{}: {e}", path.display()))?;
    let source = String::from_utf8_lossy(&bytes);
    let value = json::parse(&source).map_err(|e| format!("{}: {e}", path.display()))?;
    if !json::is_canonical_json_text(&source, &value) {
        return Err(format!("{}: source is not canonical JSON", path.display()));
    }
    Ok(value)
}

fn exact_keys(value: &Value, expected: &[&str], name: &str) -> Result<(), String> {
    let members = object(value, name)?;
    let mut actual: Vec<&str> = members.iter().map(|(key, _)| key.as_str()).collect();
    let mut wanted = expected.to_vec();
    actual.sort_unstable();
    wanted.sort_unstable();
    if actual != wanted {
        return Err(format!("{name} has unknown or missing fields"));
    }
    Ok(())
}

fn word(value: &Value, width: usize, name: &str) -> Result<u32, String> {
    let raw = text(value, name)?;
    let digits = width * 2;
    if raw.len() != digits + 2
        || !raw.starts_with("0x")
        || !raw[2..].bytes().all(|byte| byte.is_ascii_hexdigit())
    {
        return Err(format!("{name} differs"));
    }
    u32::from_str_radix(&raw[2..], 16).map_err(|_| format!("{name} differs"))
}

fn decode_package(path: &Path) -> Result<Vec<BuiltSection>, String> {
    let package = canonical_document(path)?;
    exact_keys(
        &package,
        &[
            "format",
            "kind",
            "rom_base",
            "limit",
            "planned_gaps",
            "planned_gap_bytes",
            "gaps",
        ],
        "executable gap package",
    )?;
    if number(field(&package, "format", "package")?, "package format")? != 1
        || text(field(&package, "kind", "package")?, "package kind")?
            != "golden-sun-executable-gap-source-package"
        || address(field(&package, "rom_base", "package")?, "package ROM base")? != ROM_BASE
        || address(field(&package, "limit", "package")?, "package limit")? != PLAN_LIMIT
    {
        return Err("executable gap package identity differs".into());
    }
    let gaps = field(&package, "gaps", "package")?
        .as_array()
        .ok_or_else(|| "executable gap package gaps differ".to_string())?;
    let mut result = Vec::new();
    for (gap_index, gap) in gaps.iter().enumerate() {
        exact_keys(
            gap,
            &["address", "size", "sections"],
            &format!("gap {gap_index}"),
        )?;
        let sections = field(gap, "sections", &format!("gap {gap_index}"))?
            .as_array()
            .ok_or_else(|| format!("gap {gap_index} sections differ"))?;
        for (section_index, section) in sections.iter().enumerate() {
            let section_name = format!("gap {gap_index} section {section_index}");
            let kind = text(field(section, "kind", &section_name)?, "section kind")?;
            let section_address =
                address(field(section, "address", &section_name)?, "section address")?;
            let mut data = Vec::new();
            match kind {
                "u32-records" => {
                    exact_keys(
                        section,
                        &[
                            "address",
                            "size",
                            "kind",
                            "name",
                            "value_type",
                            "record_count",
                            "words_per_record",
                            "records",
                        ],
                        &section_name,
                    )?;
                    let records = field(section, "records", &section_name)?
                        .as_array()
                        .ok_or_else(|| "u32 records differ".to_string())?;
                    for (record_index, record) in records.iter().enumerate() {
                        let words = field(record, "words", &format!("record {record_index}"))?
                            .as_array()
                            .ok_or_else(|| "u32 record differs".to_string())?;
                        for (word_index, value) in words.iter().enumerate() {
                            data.extend_from_slice(
                                &word(value, 4, &format!("u32 word {word_index}"))?.to_le_bytes(),
                            );
                        }
                    }
                }
                "u32-values" => {
                    exact_keys(
                        section,
                        &["address", "size", "kind", "name", "value_type", "values"],
                        &section_name,
                    )?;
                    let values = field(section, "values", &section_name)?
                        .as_array()
                        .ok_or_else(|| "u32 values differ".to_string())?;
                    for (index, value) in values.iter().enumerate() {
                        data.extend_from_slice(
                            &word(value, 4, &format!("u32 value {index}"))?.to_le_bytes(),
                        );
                    }
                }
                "u16-tables" => {
                    exact_keys(
                        section,
                        &["address", "size", "kind", "name", "value_type", "tables"],
                        &section_name,
                    )?;
                    let tables = field(section, "tables", &section_name)?
                        .as_array()
                        .ok_or_else(|| "u16 tables differ".to_string())?;
                    for (table_index, table) in tables.iter().enumerate() {
                        let values = field(table, "values", &format!("table {table_index}"))?
                            .as_array()
                            .ok_or_else(|| "u16 table differs".to_string())?;
                        for (index, value) in values.iter().enumerate() {
                            let word = word(value, 2, &format!("u16 value {index}"))?;
                            data.extend_from_slice(&(word as u16).to_le_bytes());
                        }
                    }
                }
                "arm" | "thumb" | "veneer" | "fill" => continue,
                other => return Err(format!("unexpected package section kind: {other}")),
            }
            let declared = number(field(section, "size", &section_name)?, "section size")?;
            if declared != data.len() as u64 {
                return Err(format!("{section_name}: decoded size differs"));
            }
            result.push(BuiltSection {
                address: section_address,
                data,
            });
        }
    }
    result.sort_by_key(|section| section.address);
    Ok(result)
}

pub fn build_executable_gap_data(path: &Path) -> Result<Vec<BuiltSection>, String> {
    decode_package(path)
}

pub fn build_section(path: &Path, address_value: u64) -> Result<Vec<u8>, String> {
    build_executable_gap_data(path)?
        .into_iter()
        .find(|section| section.address == address_value)
        .map(|section| section.data)
        .ok_or_else(|| {
            format!(
                "executable gap section {} is not present",
                hex_address(address_value)
            )
        })
}

pub fn verify_rom(rom_path: &Path, package_path: &Path) -> Result<(usize, usize), String> {
    let rom = std::fs::read(rom_path).map_err(|e| format!("{}: {e}", rom_path.display()))?;
    let sections = build_executable_gap_data(package_path)?;
    let mut bytes = 0usize;
    for section in &sections {
        let offset =
            section.address.checked_sub(ROM_BASE).ok_or_else(|| {
                format!("section {} precedes ROM base", hex_address(section.address))
            })? as usize;
        let end = offset + section.data.len();
        if end > rom.len() || rom[offset..end] != section.data {
            let mismatch = (0..section.data.len())
                .find(|index| {
                    offset + index >= rom.len() || rom[offset + index] != section.data[*index]
                })
                .unwrap_or(section.data.len());
            return Err(format!(
                "executable gap data differs at {}",
                hex_address(section.address + mismatch as u64)
            ));
        }
        bytes += section.data.len();
    }
    Ok((sections.len(), bytes))
}

pub fn self_test(plan_path: &Path) -> Result<String, String> {
    let plan = canonical_document(plan_path)?;
    exact_keys(
        &plan,
        &[
            "format",
            "kind",
            "rom_base",
            "limit",
            "gaps",
            "fragments",
            "exclusions",
        ],
        "executable gap plan",
    )?;
    if number(field(&plan, "format", "plan")?, "plan format")? != 1
        || text(field(&plan, "kind", "plan")?, "plan kind")? != "golden-sun-executable-gap-plan"
        || address(field(&plan, "rom_base", "plan")?, "plan ROM base")? != ROM_BASE
        || address(field(&plan, "limit", "plan")?, "plan limit")? != PLAN_LIMIT
    {
        return Err("executable gap plan identity differs".into());
    }
    let gaps = field(&plan, "gaps", "plan")?
        .as_array()
        .ok_or("plan gaps differ")?;
    let mut gap_bytes = 0u64;
    let mut code_bytes = 0u64;
    let mut veneer_bytes = 0u64;
    for gap in gaps {
        gap_bytes += number(field(gap, "size", "gap")?, "gap size")?;
        if text(field(gap, "classification", "gap")?, "classification")? == "veneer-slots" {
            veneer_bytes += field(gap, "veneer_addresses", "veneer gap")?
                .as_array()
                .ok_or("veneer addresses differ")?
                .len() as u64
                * 8;
        } else {
            for section in field(gap, "sections", "gap")?
                .as_array()
                .ok_or("sections differ")?
            {
                let kind = text(field(section, "kind", "section")?, "section kind")?;
                if kind == "arm" || kind == "thumb" {
                    code_bytes += number(field(section, "size", "section")?, "section size")?;
                }
            }
        }
    }
    let fragments = field(&plan, "fragments", "plan")?
        .as_array()
        .ok_or("plan fragments differ")?;
    for fragment in fragments {
        gap_bytes += number(field(fragment, "size", "fragment")?, "fragment size")?;
        code_bytes += number(field(fragment, "size", "fragment")?, "fragment size")?;
    }
    let total_gaps = gaps.len() + fragments.len();
    if total_gaps != 78 || gap_bytes != 6156 || code_bytes != 3948 || veneer_bytes != 136 {
        return Err("executable gap plan totals differ".into());
    }
    let deferred = gap_bytes - code_bytes - veneer_bytes;
    if deferred != 2072 {
        return Err("executable gap deferred total differs".into());
    }
    Ok(format!(
        "self-test=ok gaps={} executable_bytes={} deferred_bytes={deferred}",
        total_gaps,
        code_bytes + veneer_bytes
    ))
}
