//! Legacy compression-recipe reader, retained until the encoder is recovered.
//! These stored decisions are reconstruction debt, not publishable source.
use super::*;
use serde_json::json;
mod predictor;
pub(crate) use predictor::{materialize, LzMachine};

const MAGIC: &[u8; 8] = b"ALCHTOK1";
const FORMAT: &str = "alchemy-lz-controls-v1";

fn read_integer(data: &mut &[u8]) -> Result<u32, String> {
    let mut value = 0;
    for shift in (0..35).step_by(7) {
        let byte = *data.first().ok_or("truncated compression control")?;
        *data = &data[1..];
        if shift == 28 && byte > 15 {
            return Err("compression control integer overflows".into());
        }
        value |= (byte as u32 & 127) << shift;
        if byte < 128 {
            return Ok(value);
        }
    }
    Err("compression control integer overflows".into())
}
fn read_record(data: &mut &[u8], group: bool) -> Result<Value, String> {
    let tag = *data.first().ok_or("truncated compression record")?;
    *data = &data[1..];
    Ok(match tag {
        0 => json!(read_integer(data)?),
        1 => json!(["l", read_integer(data)?]),
        2 => json!(["c", read_integer(data)?, read_integer(data)?]),
        3 => json!(["e"]),
        4 => json!([read_integer(data)?, read_integer(data)?]),
        5 => json!(["z"]),
        6 if !group => {
            let count = read_integer(data)? as usize;
            if count > 8 {
                return Err("palette group exceeds eight operations".into());
            }
            json!(["g", read_sequence(data, count, true)?])
        }
        7 => json!(["l"]),
        9 => {
            let offset = read_integer(data)?;
            if !data
                .first()
                .is_some_and(|tag| matches!(tag, 0 | 1 | 2 | 4 | 7))
            {
                return Err("invalid predictor exception token".into());
            }
            json!([offset, read_record(data, group)?])
        }
        _ => return Err("invalid compression record tag".into()),
    })
}
fn read_sequence(data: &mut &[u8], count: usize, group: bool) -> Result<Vec<Value>, String> {
    if count > 16_777_216 {
        return Err("compression control count exceeds limit".into());
    }
    let mut tokens = Vec::new();
    while tokens.len() < count {
        let repeats = if data.first() == Some(&8) {
            *data = &data[1..];
            read_integer(data)? as usize
        } else {
            1
        };
        if repeats == 0 || repeats > count - tokens.len() {
            return Err("invalid compression control run".into());
        }
        let record = read_record(data, group)?;
        tokens.extend(std::iter::repeat_n(record, repeats));
    }
    Ok(tokens)
}
fn walk(
    value: &mut Value,
    action: &mut impl FnMut(&mut Value) -> Result<(), String>,
) -> Result<(), String> {
    match value {
        Value::Object(object) => {
            if object.get("codec").is_some_and(Value::is_string) {
                if let Some(tokens) = object.get_mut("tokens") {
                    if tokens.get("predictor").is_some() {
                        action(
                            tokens
                                .get_mut("exceptions")
                                .ok_or("predictor exceptions missing")?,
                        )?;
                    } else {
                        action(tokens)?;
                    }
                }
            }
            for (key, child) in object {
                if key != "tokens" {
                    walk(child, action)?;
                }
            }
        }
        Value::Array(rows) => {
            for row in rows {
                walk(row, action)?;
            }
        }
        _ => {}
    }
    Ok(())
}
// Recipe rows inherit their common codec; exceptions remain explicit.
fn restore_recipe_codec(document: &mut Value) -> Result<(), String> {
    let Some(codec) = document
        .as_object_mut()
        .and_then(|o| o.remove("recipe_codec"))
    else {
        return Ok(());
    };
    if !codec.is_string() {
        return Err("recipe codec default must be a string".into());
    }
    if let Some(recipes) = document.get_mut("recipes").and_then(Value::as_object_mut) {
        for rows in recipes.values_mut().filter_map(Value::as_array_mut) {
            for row in rows.iter_mut().filter_map(Value::as_object_mut) {
                row.entry("codec").or_insert_with(|| codec.clone());
            }
        }
    }
    Ok(())
}
/// Restore the same logical plans used by every existing codec and extractor.
pub(super) fn expand(root: &Path, document: &mut Value) -> Result<(), String> {
    restore_recipe_codec(document)?;
    let Some(table) = document.get("token_table") else {
        return Ok(());
    };
    if table["format"] != FORMAT {
        return Err("unsupported compression control table".into());
    }
    let bytes = fs::read(root_path(
        root,
        json_string(&table["source"], "control table source")?,
    )?)
    .map_err(|e| e.to_string())?;
    if !bytes.starts_with(MAGIC)
        || sha256::hex(&bytes) != json_string(&table["sha256"], "control table digest")?
    {
        return Err("compression control table digest or header differs".into());
    }
    if table.get("predictor").is_some_and(|p| p != "greedy-lz-v1") {
        return Err("unsupported default LZ predictor".into());
    }
    predictor::restore_defaults(document);
    walk(document, &mut |tokens| {
        if tokens.get("predictor").is_some() || tokens.is_array() {
            return Ok(());
        }
        let start = number(&tokens["offset"], "control offset")?;
        let size = number(&tokens["size"], "control size")?;
        let count = number(&tokens["count"], "control count")?;
        if start < MAGIC.len() {
            return Err("control stream overlaps header".into());
        }
        let mut data = bytes
            .get(start..start.checked_add(size).ok_or("control extent overflows")?)
            .ok_or("control stream exceeds table")?;
        let records = read_sequence(&mut data, count, false)?;
        if !data.is_empty() {
            return Err("compression control stream has trailing data".into());
        }
        *tokens = json!(records);
        Ok(())
    })
}
pub(super) fn table_source(document: &Value) -> Option<&str> {
    document["token_table"]["source"].as_str()
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn legacy_reader_checks_records_without_a_recipe_writer() {
        // Synthetic controls only: literal run, copy, and a repeated literal.
        let bytes = [1, 3, 2, 6, 3, 8, 2, 7];
        let mut input = bytes.as_slice();
        assert_eq!(
            read_sequence(&mut input, 4, false).unwrap(),
            vec![
                json!(["l", 3]),
                json!(["c", 6, 3]),
                json!(["l"]),
                json!(["l"])
            ]
        );
        assert!(input.is_empty());
        for end in 0..bytes.len() {
            assert!(read_sequence(&mut &bytes[..end], 4, false).is_err());
        }
        for bad in [
            &[8, 0, 7][..],
            &[8, 5, 7],
            &[9],
            &[0, 255, 255, 255, 255, 16],
        ] {
            assert!(read_sequence(&mut bad.as_ref(), 1, false).is_err());
        }
    }
}
