//! Builder for the Golden Sun byte conversion ("byte henkan") tables.
//!
//! Port of `tools/make/byte_henkan.ts`. The JSON source describes nine tables of
//! fourteen sixteen-byte rows; each table is followed by two reserved zero rows,
//! so every table occupies exactly 256 bytes of the ROM region.

pub mod cli;

use std::fs;
use std::path::Path;

use serde_json::Value;

pub const BYTE_HENKAN_ADDRESS: u32 = 0x0800_92b8;
pub const BYTE_HENKAN_END: u32 = 0x0800_9bb8;
pub const BYTE_HENKAN_SIZE: usize = (BYTE_HENKAN_END - BYTE_HENKAN_ADDRESS) as usize;

const TABLE_COUNT: usize = 9;
const PERMUTATION_TABLES: usize = 5;

#[derive(Debug, Clone, PartialEq, Eq)]
pub enum TableKind {
    Permutation,
    Mapping,
}

impl TableKind {
    fn as_str(&self) -> &'static str {
        match self {
            TableKind::Permutation => "permutation",
            TableKind::Mapping => "mapping",
        }
    }
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct TableSource {
    pub name: String,
    pub kind: TableKind,
    pub rows: Vec<Vec<u8>>,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct SourceDocument {
    pub table_width: usize,
    pub source_rows: usize,
    pub reserved_rows: usize,
    pub tables: Vec<TableSource>,
}

/// Mirrors the TypeScript `exactKeys`: the value must be a plain object whose
/// key set is exactly `keys` (no extras, no omissions).
fn exact_keys<'a>(value: &'a Value, keys: &[&str], label: &str) -> Result<&'a serde_json::Map<String, Value>, String> {
    let object = match value {
        Value::Object(map) => map,
        _ => return Err(format!("{label} must be an object")),
    };
    let mut actual: Vec<&str> = object.keys().map(|key| key.as_str()).collect();
    actual.sort_unstable();
    let mut expected: Vec<&str> = keys.to_vec();
    expected.sort_unstable();
    if actual != expected {
        return Err(format!("{label} has unexpected fields"));
    }
    Ok(object)
}

/// JavaScript has one number type, so `1` and `1.0` are the same value and both
/// satisfy `Number.isSafeInteger`. Accept an integral JSON float the same way.
fn as_js_safe_integer(value: &Value) -> Option<i64> {
    const MAX_SAFE: f64 = 9_007_199_254_740_991.0;
    let number = value.as_f64()?;
    if !value.is_number() || !number.is_finite() || number.fract() != 0.0 || number.abs() > MAX_SAFE {
        return None;
    }
    Some(number as i64)
}

fn integer(value: &Value, minimum: i64, maximum: i64, label: &str) -> Result<i64, String> {
    match as_js_safe_integer(value) {
        Some(number) if number >= minimum && number <= maximum => Ok(number),
        _ => Err(format!("{label} is outside its range")),
    }
}

fn number_is(value: Option<&Value>, expected: i64) -> bool {
    value.and_then(as_js_safe_integer) == Some(expected)
}

fn string_is(value: Option<&Value>, expected: &str) -> bool {
    value.and_then(Value::as_str) == Some(expected)
}

pub fn parse_document(value: &Value) -> Result<SourceDocument, String> {
    let object = exact_keys(value, &["format", "kind", "address", "end", "table_width", "source_rows", "reserved_rows", "tables"], "byte conversion source")?;
    let tables_value = object.get("tables").and_then(Value::as_array);
    if !number_is(object.get("format"), 1)
        || !string_is(object.get("kind"), "golden-sun-byte-henkan-tables")
        || !string_is(object.get("address"), "0x080092b8")
        || !string_is(object.get("end"), "0x08009bb8")
        || !number_is(object.get("table_width"), 16)
        || !number_is(object.get("source_rows"), 14)
        || !number_is(object.get("reserved_rows"), 2)
        || tables_value.map(|tables| tables.len()) != Some(TABLE_COUNT)
    {
        return Err("byte conversion layout differs".to_string());
    }
    let tables_value = tables_value.expect("table count already checked");

    let mut tables = Vec::with_capacity(TABLE_COUNT);
    for (table_index, item) in tables_value.iter().enumerate() {
        let table = exact_keys(item, &["name", "kind", "rows"], &format!("byte conversion table {table_index}"))?;
        let expected_name = format!("hyou_{table_index:02}");
        let expected_kind = if table_index < PERMUTATION_TABLES { TableKind::Permutation } else { TableKind::Mapping };
        let rows_value = table.get("rows").and_then(Value::as_array);
        if !string_is(table.get("name"), &expected_name) || !string_is(table.get("kind"), expected_kind.as_str()) || rows_value.map(|rows| rows.len()) != Some(14) {
            return Err(format!("byte conversion table {table_index} differs"));
        }
        let rows_value = rows_value.expect("row count already checked");

        let mut rows: Vec<Vec<u8>> = Vec::with_capacity(14);
        for (row_index, row) in rows_value.iter().enumerate() {
            let entries = match row.as_array() {
                Some(entries) if entries.len() == 16 => entries,
                _ => return Err(format!("byte conversion table {table_index} row {row_index} differs")),
            };
            let mut parsed = Vec::with_capacity(16);
            for (column_index, entry) in entries.iter().enumerate() {
                let label = format!("byte conversion table {table_index} row {row_index} column {column_index}");
                parsed.push(integer(entry, 0, 223, &label)? as u8);
            }
            rows.push(parsed);
        }

        if expected_kind == TableKind::Permutation {
            let mut ordered: Vec<u8> = rows.iter().flatten().copied().collect();
            ordered.sort_unstable();
            if ordered.iter().enumerate().any(|(index, entry)| usize::from(*entry) != index) {
                return Err(format!("byte conversion table {table_index} is not a permutation"));
            }
        }

        tables.push(TableSource { name: expected_name, kind: expected_kind, rows });
    }

    Ok(SourceDocument { table_width: 16, source_rows: 14, reserved_rows: 2, tables })
}

pub fn build_from_text(text: &str) -> Result<Vec<u8>, String> {
    if text.contains('\r') || !text.ends_with('\n') {
        return Err("byte conversion source is not canonical text".to_string());
    }
    let value: Value = serde_json::from_str(text).map_err(|error| error.to_string())?;
    let source = parse_document(&value)?;
    let mut result = Vec::with_capacity(BYTE_HENKAN_SIZE);
    for table in &source.tables {
        for row in &table.rows {
            result.extend_from_slice(row);
        }
        result.resize(result.len() + source.reserved_rows * source.table_width, 0);
    }
    if result.len() != BYTE_HENKAN_SIZE {
        return Err("byte conversion output size differs".to_string());
    }
    Ok(result)
}

pub fn build_byte_henkan_tables(path: &Path) -> Result<Vec<u8>, String> {
    let text = fs::read_to_string(path).map_err(|error| format!("{}: {error}", path.display()))?;
    build_from_text(&text)
}

fn test_document() -> Value {
    let permutation: Vec<Value> = (0..14).map(|row| Value::Array((0..16).map(|column| Value::from(row * 16 + column)).collect())).collect();
    let mapping: Vec<Value> = (0..14).map(|_| Value::Array((0..16).map(|_| Value::from(7)).collect())).collect();
    let tables: Vec<Value> = (0..TABLE_COUNT)
        .map(|index| {
            let kind = if index < PERMUTATION_TABLES { "permutation" } else { "mapping" };
            let rows = if index < PERMUTATION_TABLES { &permutation } else { &mapping };
            serde_json::json!({
                "name": format!("hyou_{index:02}"),
                "kind": kind,
                "rows": Value::Array(rows.clone()),
            })
        })
        .collect();
    serde_json::json!({
        "format": 1,
        "kind": "golden-sun-byte-henkan-tables",
        "address": "0x080092b8",
        "end": "0x08009bb8",
        "table_width": 16,
        "source_rows": 14,
        "reserved_rows": 2,
        "tables": Value::Array(tables),
    })
}

/// Port of the TypeScript `selfTest`, including the rejection of a source whose
/// first permutation table has a duplicated entry.
pub fn self_test() -> Result<(), String> {
    let parsed = parse_document(&test_document())?;
    let mut result = Vec::new();
    for table in &parsed.tables {
        for row in &table.rows {
            result.extend_from_slice(row);
        }
        result.resize(result.len() + 32, 0);
    }
    if result.len() != BYTE_HENKAN_SIZE || result[223] != 223 || result[224] != 0 || result[5 * 256] != 7 || *result.last().expect("result is not empty") != 0 {
        return Err("byte conversion build self-test failed".to_string());
    }

    let mut duplicate = test_document();
    duplicate["tables"][0]["rows"][0][0] = Value::from(1);
    if parse_document(&duplicate).is_ok() {
        return Err("invalid byte conversion permutation was accepted".to_string());
    }
    println!("self-test=ok");
    Ok(())
}
