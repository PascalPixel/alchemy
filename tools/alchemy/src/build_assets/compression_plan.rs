//! Lossless storage of LZ control choices. Literal records contain lengths or
//! bit widths, never image bytes. JSON retains codecs, sizes and stream order.
use super::*;
use serde_json::json;
mod predictor;
pub(super) use predictor::{compact_plan, derive, materialize};

const MAGIC: &[u8; 8] = b"ALCHTOK1";
const FORMAT: &str = "alchemy-lz-controls-v1";

fn integer(out: &mut Vec<u8>, mut value: u32) {
    while value >= 128 {
        out.push(value as u8 | 128);
        value >>= 7;
    }
    out.push(value as u8);
}
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
fn write_record(out: &mut Vec<u8>, token: &Value) -> Result<(), String> {
    if let Some(value) = token.as_u64() {
        out.push(0);
        integer(out, u32::try_from(value).map_err(|_| "token exceeds u32")?);
        return Ok(());
    }
    let row = token.as_array().ok_or("invalid compression control")?;
    let (tag, values) = match row.as_slice() {
        [a, b] if a.is_u64() && b.is_u64() => (4, &row[..]),
        [a, b] if a.is_u64() && b.is_array() => {
            out.push(9);
            integer(
                out,
                u32::try_from(number(a, "exception offset")?)
                    .map_err(|_| "exception offset exceeds u32")?,
            );
            write_record(out, b)?;
            return Ok(());
        }
        [a] if a == "e" => (3, &row[1..]),
        [a] if a == "z" => (5, &row[1..]),
        [a] if a == "l" => (7, &row[1..]),
        [a, _] if a == "l" => (1, &row[1..]),
        [a, _, _] if a == "c" => (2, &row[1..]),
        [a, operations] if a == "g" => {
            let operations = operations.as_array().ok_or("invalid palette operations")?;
            if operations.len() > 8 {
                return Err("palette group exceeds eight operations".into());
            }
            out.push(6);
            integer(out, operations.len() as u32);
            write_sequence(out, operations)?;
            return Ok(());
        }
        _ => return Err("unsupported compression control".into()),
    };
    out.push(tag);
    for value in values {
        integer(
            out,
            u32::try_from(number(value, "token value")?).map_err(|_| "token exceeds u32")?,
        );
    }
    Ok(())
}
fn write_sequence(out: &mut Vec<u8>, tokens: &[Value]) -> Result<(), String> {
    let mut at = 0;
    while at < tokens.len() {
        let mut end = at + 1;
        while end < tokens.len() && tokens[end] == tokens[at] {
            end += 1;
        }
        if end - at >= 3 {
            out.push(8);
            integer(
                out,
                u32::try_from(end - at).map_err(|_| "too many controls")?,
            );
            write_record(out, &tokens[at])?;
        } else {
            for token in &tokens[at..end] {
                write_record(out, token)?;
            }
        }
        at = end;
    }
    Ok(())
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
    predictor::defaults(document, true);
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

fn compact(document: &mut Value) -> Result<Vec<u8>, String> {
    let mut table = MAGIC.to_vec();
    let mut streams = BTreeMap::<Vec<u8>, (usize, usize)>::new();
    walk(document, &mut |tokens| {
        if tokens.get("predictor").is_some() {
            return Ok(());
        }
        let rows = tokens
            .as_array()
            .ok_or("expand controls before compacting")?;
        let count = rows.len();
        let mut bytes = Vec::new();
        write_sequence(&mut bytes, rows)?;
        let (offset, size) = *streams.entry(bytes.clone()).or_insert_with(|| {
            let offset = table.len();
            table.extend(&bytes);
            (offset, bytes.len())
        });
        *tokens = json!({"offset":offset,"size":size,"count":count});
        Ok(())
    })?;
    let mut inherited = false;
    if let Some(recipes) = document.get_mut("recipes").and_then(Value::as_object_mut) {
        for rows in recipes.values_mut().filter_map(Value::as_array_mut) {
            for row in rows.iter_mut().filter_map(Value::as_object_mut) {
                if row.get("codec").and_then(Value::as_str) == Some("golden-sun-arena-lz") {
                    row.remove("codec");
                    inherited = true;
                }
            }
        }
    }
    if inherited {
        document["recipe_codec"] = json!("golden-sun-arena-lz");
    }
    Ok(table)
}
/// Repack generated readable plans; already compact plans also round-trip.
pub(super) fn repack(root: &Path, path: &Path) -> Result<(), String> {
    let mut document = json(path)?;
    expand(root, &mut document)?;
    store(root, path, document)
}
fn store(root: &Path, path: &Path, mut document: Value) -> Result<(), String> {
    let mut original = document.clone();
    document
        .as_object_mut()
        .ok_or("compression plans must be an object")?
        .remove("token_table");
    let table = compact(&mut document)?;
    let table_path = path.with_extension("TOKENS");
    let name = table_path
        .strip_prefix(root)
        .map_err(|_| "compression plans must be in the repository")?
        .to_string_lossy();
    document["token_table"] = json!({"format":FORMAT,"source":name,"sha256":sha256::hex(&table),"predictor":"greedy-lz-v1"});
    // Check the exact inverse before either maintained file is written.
    let mut restored = document.clone();
    restore_recipe_codec(&mut restored)?;
    walk(&mut restored, &mut |tokens| {
        if tokens.get("predictor").is_some() {
            return Ok(());
        }
        let start = number(&tokens["offset"], "offset")?;
        let size = number(&tokens["size"], "size")?;
        let mut data = &table[start..start + size];
        *tokens = json!(read_sequence(
            &mut data,
            number(&tokens["count"], "count")?,
            false
        )?);
        Ok(())
    })?;

    original.as_object_mut().unwrap().remove("token_table");
    restored.as_object_mut().unwrap().remove("token_table");
    if original != restored {
        return Err("compression controls did not round-trip".into());
    }
    fs::write(&table_path, &table).map_err(|e| e.to_string())?;
    predictor::defaults(&mut document, false);
    fs::write(path, format!("{}\n", canonical_json(&document))).map_err(|e| e.to_string())?;
    println!(
        "compression metadata={} controls={} bytes",
        path.display(),
        table.len()
    );
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn compact_plans_roundtrip_deterministically_and_reject_corruption() {
        let directory = tempfile::tempdir().unwrap();
        let root = directory.path();
        let path = root.join("COMPRESSION.JSON");
        let original = json!({"recipes":{"bank":[
            {"codec":"golden-sun-general-lz","tokens":[["l",1],["c",2,1],["e"]]},
            {"codec":"golden-sun-general-lz","tokens":[["l",1],["c",2,1],["e"]]},
            {"codec":"golden-sun-arena-lz","decoded_size":4,"encoded_size":6}
        ]},"palette":{"codec":"golden-sun-palette-lz","tokens":[["z"],["g",[["l"],["l"],["l"],["c",2,1],["e"]]]]},
        "tile":{"codec":"golden-sun-kind2-lz","tokens":[2,2,2,[1,2]]},
        "derived":{"codec":"golden-sun-general-lz","tokens":{"predictor":"greedy-lz-v1","exceptions":[[1,["l",1]]]}}});
        fs::write(&path, original.to_string()).unwrap();
        repack(root, &path).unwrap();
        let metadata = fs::read(&path).unwrap();
        let table_path = path.with_extension("TOKENS");
        let bytes = fs::read(&table_path).unwrap();
        let mut restored = json(&path).unwrap();
        expand(root, &mut restored).unwrap();
        restored.as_object_mut().unwrap().remove("token_table");
        assert_eq!(restored, original);
        repack(root, &path).unwrap();
        assert_eq!(fs::read(&path).unwrap(), metadata);
        assert_eq!(fs::read(&table_path).unwrap(), bytes);
        let document = json(&path).unwrap();
        let mut invalid = document.clone();
        invalid["tile"]["tokens"]["offset"] = json!(7);
        assert!(expand(root, &mut invalid).is_err());
        let mut invalid = document.clone();
        invalid["tile"]["tokens"]["size"] = json!(bytes.len());
        assert!(expand(root, &mut invalid).is_err());
        let mut invalid = document.clone();
        invalid["tile"]["tokens"]["count"] = json!(1);
        assert!(expand(root, &mut invalid).is_err());
        let mut corrupt = bytes;
        corrupt[8] ^= 1;
        fs::write(table_path, corrupt).unwrap();
        assert!(expand(root, &mut document.clone()).is_err());
    }

    #[test]
    fn hash_keyed_default_plans_without_recipes_roundtrip() {
        let directory = tempfile::tempdir().unwrap();
        let root = directory.path();
        let path = root.join("COMPRESSION.JSON");
        let original = json!({
            "aa":{"format":1,"codec":"golden-sun-kind2-lz","decoded_size":4,"tokens":{"predictor":"greedy-lz-v1","exceptions":[]}},
            "bb":{"format":1,"codec":"golden-sun-tagged-palette-lz","decoded_size":8,"tokens":{"predictor":"greedy-lz-v1","exceptions":[[3,["l"]],[5,["c",2,1]]]}}
        });
        fs::write(&path, original.to_string()).unwrap();
        repack(root, &path).unwrap();
        let metadata = fs::read(&path).unwrap();
        assert!(json(&path).unwrap().get("recipes").is_none());
        let mut restored = json(&path).unwrap();
        expand(root, &mut restored).unwrap();
        restored.as_object_mut().unwrap().remove("token_table");
        assert_eq!(restored["aa"]["tokens"], original["aa"]["tokens"]);
        assert_eq!(restored["bb"]["tokens"], original["bb"]["tokens"]);
        assert_eq!(restored, original);
        repack(root, &path).unwrap();
        assert_eq!(fs::read(&path).unwrap(), metadata);
    }

    #[test]
    fn control_records_preserve_every_shape_and_run_boundary() {
        let rows = json!([
            2,
            2,
            2,
            [1, 2],
            [14, ["c", 3, 2]],
            ["l", 128],
            ["c", 256, 65536],
            ["e"],
            ["z"],
            ["g", [["l"], ["l"], ["l"], ["c", 2, 3], ["e"]]]
        ]);
        let rows = rows.as_array().unwrap();
        let mut bytes = Vec::new();
        write_sequence(&mut bytes, rows).unwrap();
        let mut input = bytes.as_slice();
        assert_eq!(read_sequence(&mut input, rows.len(), false).unwrap(), *rows);
        assert!(input.is_empty());
        for end in 0..bytes.len() {
            assert!(read_sequence(&mut &bytes[..end], rows.len(), false).is_err());
        }
        for bad in [
            &[8, 0, 7][..],
            &[8, 4, 7],
            &[9],
            &[0, 255, 255, 255, 255, 16],
        ] {
            let mut input = bad;
            assert!(read_sequence(&mut input, 1, false).is_err());
        }
    }
}
