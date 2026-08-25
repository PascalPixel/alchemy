// The one-to-three padding bytes that sit between a resource and the next
// alignment boundary. They are recorded either as a fill value or as literal
// bytes, and either form must rebuild the original bytes exactly.
//
// Ported from tools/lib/alignment_tail.ts. Two differences are worth naming.
// The byte-range check is gone from the constructors because a u8 cannot leave
// its range; it survives in the JSON parser, where the input is still
// untrusted. And serialization is written by hand rather than derived, because
// serde's internally-tagged representation would emit "encoding" first while
// the tracked files carry "size" first, and reordering keys would rewrite every
// file holding a tail.

use serde::ser::{Serialize, SerializeMap, Serializer};
use serde_json::Value;

pub const DEFAULT_MAXIMUM: usize = 3;

#[derive(Debug, Clone, PartialEq, Eq)]
pub enum AlignmentTail {
    Fill { size: usize, value: u8 },
    Bytes { values: Vec<u8> },
}

impl AlignmentTail {
    pub fn size(&self) -> usize {
        match self {
            Self::Fill { size, .. } => *size,
            Self::Bytes { values } => values.len(),
        }
    }
}

impl Serialize for AlignmentTail {
    fn serialize<S: Serializer>(&self, serializer: S) -> Result<S::Ok, S::Error> {
        let mut map = serializer.serialize_map(Some(3))?;
        map.serialize_entry("size", &self.size())?;
        match self {
            Self::Fill { value, .. } => {
                map.serialize_entry("encoding", "fill")?;
                map.serialize_entry("value", value)?;
            }
            Self::Bytes { values } => {
                map.serialize_entry("encoding", "bytes")?;
                map.serialize_entry("values", values)?;
            }
        }
        map.end()
    }
}

#[derive(Debug, PartialEq, Eq)]
pub struct AlignmentTailError(pub String);

impl std::fmt::Display for AlignmentTailError {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "{}", self.0)
    }
}

impl std::error::Error for AlignmentTailError {}

fn fail<T>(message: impl Into<String>) -> Result<T, AlignmentTailError> {
    Err(AlignmentTailError(message.into()))
}

pub fn inspect_alignment_tail(data: &[u8], maximum: usize) -> Result<AlignmentTail, AlignmentTailError> {
    if maximum < 1 {
        return fail("alignment-tail maximum must be positive");
    }
    if data.is_empty() || data.len() > maximum {
        return fail("alignment tail is outside its bounded extent");
    }
    if data.iter().all(|byte| *byte == data[0]) {
        return Ok(AlignmentTail::Fill { size: data.len(), value: data[0] });
    }
    Ok(AlignmentTail::Bytes { values: data.to_vec() })
}

fn exact_keys(object: &serde_json::Map<String, Value>, keys: &[&str], label: &str) -> Result<(), AlignmentTailError> {
    let mut actual: Vec<&str> = object.keys().map(String::as_str).collect();
    actual.sort_unstable();
    let mut expected = keys.to_vec();
    expected.sort_unstable();
    if actual != expected {
        return fail(format!("{label} has unexpected fields"));
    }
    Ok(())
}

fn byte(value: Option<&Value>, label: &str) -> Result<u8, AlignmentTailError> {
    match value.and_then(Value::as_u64) {
        Some(number) if number <= 0xff => Ok(number as u8),
        _ => fail(format!("{label} must be a byte")),
    }
}

pub fn parse_alignment_tail(value: &Value, expected_size: usize, maximum: usize, label: &str) -> Result<AlignmentTail, AlignmentTailError> {
    if expected_size < 1 || expected_size > maximum {
        return fail(format!("{label} has an invalid expected size"));
    }
    let Some(tail) = value.as_object() else {
        return fail(format!("{label} must be an object"));
    };
    if tail.get("size").and_then(Value::as_u64) != Some(expected_size as u64) {
        return fail(format!("{label} size differs from its boundary"));
    }
    match tail.get("encoding").and_then(Value::as_str) {
        Some("fill") => {
            exact_keys(tail, &["size", "encoding", "value"], label)?;
            Ok(AlignmentTail::Fill { size: expected_size, value: byte(tail.get("value"), &format!("{label} fill"))? })
        }
        Some("bytes") => {
            exact_keys(tail, &["size", "encoding", "values"], label)?;
            let Some(items) = tail.get("values").and_then(Value::as_array) else {
                return fail(format!("{label} byte count differs from its boundary"));
            };
            if items.len() != expected_size {
                return fail(format!("{label} byte count differs from its boundary"));
            }
            let mut values = Vec::with_capacity(items.len());
            for (index, item) in items.iter().enumerate() {
                values.push(byte(Some(item), &format!("{label} byte {index}"))?);
            }
            Ok(AlignmentTail::Bytes { values })
        }
        _ => fail(format!("{label} has an unsupported encoding")),
    }
}

pub fn build_alignment_tail(tail: &AlignmentTail) -> Vec<u8> {
    match tail {
        AlignmentTail::Fill { size, value } => vec![*value; *size],
        AlignmentTail::Bytes { values } => values.clone(),
    }
}
