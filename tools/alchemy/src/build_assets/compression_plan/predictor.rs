use super::*;
const FORMAT: &str = "greedy-lz-v1";

fn supported(codec: &str) -> bool {
    matches!(
        codec,
        "golden-sun-arena-lz"
            | "golden-sun-kind2-lz"
            | "golden-sun-general-lz"
            | "golden-sun-general-lz-prefill"
            | "golden-sun-palette-lz"
            | "golden-sun-tagged-palette-lz"
    )
}
pub(super) fn defaults(value: &mut Value, restore: bool) {
    match value {
        Value::Object(object) => {
            if object.contains_key("exceptions") {
                if restore {
                    if let Some(reference) = object["exceptions"]
                        .as_array()
                        .filter(|rows| rows.len() == 3 && rows.iter().all(Value::is_u64))
                    {
                        object.insert(
                            "exceptions".into(),
                            json!({"offset":reference[0],"size":reference[1],"count":reference[2]}),
                        );
                    }
                    object.entry("predictor").or_insert_with(|| json!(FORMAT));
                } else if object.get("predictor").and_then(Value::as_str) == Some(FORMAT) {
                    object.remove("predictor");
                    if object["exceptions"].is_object() {
                        let r = &object["exceptions"];
                        let reference = if r["count"] == 0 {
                            json!([])
                        } else {
                            json!([r["offset"], r["size"], r["count"]])
                        };
                        object.insert("exceptions".into(), reference);
                    }
                }
            }
            for child in object.values_mut() {
                defaults(child, restore);
            }
        }
        Value::Array(rows) => {
            for row in rows {
                defaults(row, restore);
            }
        }
        _ => {}
    }
}
struct Matcher<'a> {
    data: &'a [u8],
    pairs: HashMap<u16, Vec<usize>>,
    mtf: [u8; 16],
}
impl<'a> Matcher<'a> {
    fn new(data: &'a [u8]) -> Self {
        let mut pairs = HashMap::<u16, Vec<usize>>::new();
        for (position, pair) in data.windows(2).enumerate() {
            pairs
                .entry(u16::from_le_bytes([pair[0], pair[1]]))
                .or_default()
                .push(position);
        }
        Self {
            data,
            pairs,
            mtf: std::array::from_fn(|i| i as u8),
        }
    }
    fn next(&mut self, position: usize, codec: &str) -> Value {
        let palette = codec.contains("palette");
        let maximum = (if palette { 272 } else { 137 }).min(self.data.len() - position);
        let window = if palette { 4095 } else { 4123 };
        let mut length = 1;
        let mut distance = 0;
        if let Some(pair) = self.data.get(position..position + 2) {
            if let Some(positions) = self.pairs.get(&u16::from_le_bytes([pair[0], pair[1]])) {
                let start = positions.partition_point(|p| *p < position.saturating_sub(window));
                let end = positions.partition_point(|p| *p < position);
                for source in positions[start..end].iter().rev() {
                    if self.data[*source + length] != self.data[position + length] {
                        continue;
                    }
                    let mut count = 2;
                    while count < maximum
                        && position + count < self.data.len()
                        && self.data[*source + count] == self.data[position + count]
                    {
                        count += 1;
                    }
                    if count > length {
                        length = count;
                        distance = position - source;
                    }
                    if length == maximum {
                        break;
                    }
                }
            }
        }
        if length >= 2 {
            if codec == "golden-sun-kind2-lz" {
                json!([distance, length])
            } else {
                json!(["c", length, distance])
            }
        } else if codec == "golden-sun-kind2-lz" {
            self.literal(position)
        } else if palette {
            json!(["l"])
        } else {
            json!(["l", 1])
        }
    }
    fn arena_next(&self, decoded: &[u8], position: usize) -> Value {
        let maximum = 273.min(decoded.len() - position);
        let mut length = 1;
        let mut distance = 0;
        if let Some(pair) = decoded.get(position..position + 2) {
            if let Some(positions) = self.pairs.get(&u16::from_le_bytes([pair[0], pair[1]])) {
                let start =
                    positions.partition_point(|p| *p < self.data.len().saturating_sub(4095));
                for source in &positions[start..] {
                    if source + length >= self.data.len()
                        || self.data[source + length] != decoded[position + length]
                    {
                        continue;
                    }
                    let mut count = 2;
                    while count < maximum
                        && source + count < self.data.len()
                        && self.data[source + count] == decoded[position + count]
                    {
                        count += 1;
                    }
                    if count > length {
                        length = count;
                        distance = self.data.len() - source;
                    }
                    if length == maximum {
                        break;
                    }
                }
            }
        }
        if length >= 3 {
            json!(["c", length, distance])
        } else {
            json!(["l", 1])
        }
    }
    fn literal(&mut self, position: usize) -> Value {
        let byte = self.data[position];
        let mut largest = 0usize;
        for nibble in [byte & 15, byte >> 4] {
            let index = self.mtf.iter().position(|n| *n == nibble).unwrap();
            largest = largest.max(index);
            self.mtf[..=index].rotate_right(1);
        }
        json!((usize::BITS - largest.leading_zeros()).max(2))
    }
}
fn length(token: &Value, codec: &str) -> Result<usize, String> {
    if token.is_u64() || token[0] == "l" {
        return Ok(if codec.contains("palette") || token.is_u64() {
            1
        } else {
            number(&token[1], "literal count")?
        });
    }
    number(&token[1], "copy length")
}
fn flatten(tokens: &Value, codec: &str) -> Result<Vec<Value>, String> {
    let rows = tokens
        .as_array()
        .ok_or("explicit tokens must be an array")?;
    let mut flat = Vec::new();
    for row in rows {
        if codec.contains("palette") {
            if row[0] == "z" {
                flat.extend(std::iter::repeat_n(json!(["l"]), 8));
            } else if row[0] == "g" {
                for op in row[1].as_array().ok_or("palette group must be an array")? {
                    if op[0] != "e" {
                        flat.push(op.clone());
                    }
                }
            } else {
                return Err("invalid palette group".into());
            }
        } else if row[0] == "l" {
            flat.extend(std::iter::repeat_n(
                json!(["l", 1]),
                number(&row[1], "literal count")?,
            ));
        } else {
            flat.push(row.clone());
        }
    }
    Ok(flat)
}
fn group(flat: Vec<Value>, codec: &str) -> Value {
    if !codec.contains("palette") {
        return json!(flat);
    }
    let mut flat = flat;
    flat.push(json!(["e"]));
    json!(flat
        .chunks(8)
        .map(|ops| if ops.len() == 8 && ops.iter().all(|o| o[0] == "l") {
            json!(["z"])
        } else {
            json!(["g", ops])
        })
        .collect::<Vec<_>>())
}
fn tokens(
    decoded: &[u8],
    plan: &Value,
    explicit: Option<&[Value]>,
    arena: &[u8],
) -> Result<Value, String> {
    let codec = json_string(&plan["codec"], "predictor codec")?;
    if !supported(codec) {
        return Err("unsupported predictor codec".into());
    }
    let prefill = if codec.ends_with("prefill") {
        number(&plan["prefill"], "prefill")?
    } else {
        0
    };
    let mut data = vec![0; prefill];
    data.extend_from_slice(decoded);
    let mut matcher = Matcher::new(&data);
    let arena_split = if codec == "golden-sun-arena-lz" {
        if let Some(rows) = explicit {
            2 + rows.iter().filter(|r| r[0] == "l").count()
        } else {
            number(&plan["tokens"]["split"], "arena split")?
        }
    } else {
        0
    };
    let mut prefix = arena.to_vec();
    if codec == "golden-sun-arena-lz" {
        prefix.extend(
            u16::try_from(arena_split)
                .map_err(|_| "arena split exceeds halfword")?
                .to_le_bytes(),
        );
    }
    let arena_matcher = if codec == "golden-sun-arena-lz" {
        Some(Matcher::new(&prefix))
    } else {
        None
    };
    let exceptions = plan["tokens"]["exceptions"].as_array();
    let mut exception = 0;
    let mut position = prefill;
    let mut result = Vec::new();
    let mut overrides = Vec::new();
    while position < data.len() {
        let saved_mtf = matcher.mtf;
        let predicted = if let Some(arena) = &arena_matcher {
            arena.arena_next(&data, position)
        } else {
            matcher.next(position, codec)
        };
        let actual = if let Some(rows) = explicit {
            rows.get(result.len())
                .ok_or("explicit plan ends early")?
                .clone()
        } else if let Some(rows) = exceptions {
            if let Some(row) = rows.get(exception) {
                let at = number(&row[0], "exception offset")?;
                if at < position - prefill {
                    return Err("predictor exception is out of order or inside a copy".into());
                }
                if at == position - prefill {
                    exception += 1;
                    row[1].clone()
                } else {
                    predicted.clone()
                }
            } else {
                predicted.clone()
            }
        } else {
            return Err("predictor exceptions must be an array".into());
        };
        if actual != predicted {
            overrides.push(json!([position - prefill, actual]));
        }
        if codec == "golden-sun-kind2-lz" {
            matcher.mtf = saved_mtf;
            if actual.is_u64() {
                matcher.literal(position);
            }
        }
        let consumed = length(&actual, codec)?;
        if consumed == 0 || consumed > data.len() - position {
            return Err("predictor token exceeds decoded input".into());
        }
        position += consumed;
        result.push(actual);
    }
    if explicit.is_some_and(|rows| rows.len() != result.len())
        || exceptions.is_some_and(|rows| exception != rows.len())
    {
        return Err("predictor has unused records".into());
    }
    Ok(if explicit.is_some() {
        if arena_matcher.is_some() {
            json!({"predictor":FORMAT,"split":arena_split,"exceptions":overrides})
        } else {
            json!({"predictor":FORMAT,"exceptions":overrides})
        }
    } else {
        group(result, codec)
    })
}
pub(in crate::build_assets) fn materialize(
    decoded: &[u8],
    plan: &Value,
    arena: &[u8],
) -> Result<Value, String> {
    if plan["tokens"].is_array() {
        return Ok(plan["tokens"].clone());
    }
    if plan["tokens"]["predictor"] != FORMAT {
        return Err("unsupported LZ predictor".into());
    }
    tokens(decoded, plan, None, arena)
}
/// The predictor form of one explicit plan, or the plan itself when its codec
/// has no predictor; the logical controls are checked to be unchanged.
pub(in crate::build_assets) fn compact_plan(decoded: &[u8], plan: &Value) -> Result<Value, String> {
    Ok(derive_stream(decoded, plan, &[])?.unwrap_or_else(|| plan.clone()))
}
fn derive_stream(decoded: &[u8], plan: &Value, arena: &[u8]) -> Result<Option<Value>, String> {
    let codec = json_string(&plan["codec"], "codec")?;
    if !supported(codec) || !plan["tokens"].is_array() {
        return Ok(None);
    }
    let flat = flatten(&plan["tokens"], codec)?;
    let compact = tokens(decoded, plan, Some(&flat), arena)?;
    let mut candidate = plan.clone();
    candidate["tokens"] = compact;
    if materialize(decoded, &candidate, arena)? != group(flat, codec) {
        return Err("derived predictor changed logical controls".into());
    }
    Ok(Some(candidate))
}
fn collect(
    ctx: &Context,
    value: &Value,
    name: &str,
    entries: &mut Vec<Value>,
    seen: &mut BTreeSet<(String, String)>,
) -> Result<(), String> {
    if value.get("plan").and_then(Value::as_str) == Some(name) {
        let mut entry = value.clone();
        if entry.get("address").is_none() {
            entry["address"] = json!(0);
        }
        entries.push(entry);
    }
    if value.get("kind").and_then(Value::as_str) == Some("components")
        && !value["components"].is_array()
    {
        if let (Some(source), Some(pointer)) = (value["source"].as_str(), value["pointer"].as_str())
        {
            if seen.insert((source.into(), pointer.into())) {
                let doc = ctx.document(&root_path(&ctx.root, source)?)?;
                collect(
                    ctx,
                    doc.pointer(pointer)
                        .ok_or("native component pointer missing")?,
                    name,
                    entries,
                    seen,
                )?;
            }
        }
    }
    match value {
        Value::Array(rows) => {
            for row in rows {
                collect(ctx, row, name, entries, seen)?;
            }
        }
        Value::Object(object) => {
            for row in object.values() {
                collect(ctx, row, name, entries, seen)?;
            }
        }
        _ => {}
    }
    Ok(())
}
/// Derive from native source components, then compare every whole asset before writing.
pub(in crate::build_assets) fn derive(root: &Path, path: &Path) -> Result<(), String> {
    let name = root_relative(root, path)?;
    let manifest = json(&root.join("games/THE BROKEN SEAL/recon/assets.json"))?;
    let mut original_ctx = Context::new(root);
    let mut entries = manifest["regions"].as_array().cloned().unwrap_or_default();
    expand_closure_packages(&mut original_ctx, &manifest, &mut entries)?;
    expand_series(&mut original_ctx, &manifest, &mut entries)?;
    let mut compressors = Vec::new();
    collect(
        &original_ctx,
        &json!(entries),
        &name,
        &mut compressors,
        &mut BTreeSet::new(),
    )?;
    let entries = compressors;
    let mut candidate = (*original_ctx.document(path)?).clone();
    let mut changed = 0;
    for (asset, entry) in entries.iter().enumerate() {
        if asset % 64 == 0 {
            println!("derive assets={asset}/{}", entries.len());
        }
        if entry["plan"] != name {
            continue;
        }
        let plan = select_plan(&candidate, entry)?.clone();
        let component_doc = entry
            .get("components_source")
            .and_then(Value::as_str)
            .map(|p| original_ctx.document(&root_path(root, p)?))
            .transpose()?;
        let components = entry
            .get("components")
            .or_else(|| {
                component_doc
                    .as_ref()?
                    .pointer(entry["components_pointer"].as_str()?)
            })
            .and_then(Value::as_array)
            .ok_or("native components missing")?;
        let rows = plan
            .as_array()
            .cloned()
            .unwrap_or_else(|| vec![plan.clone()]);
        let mut replacement = rows.clone();
        let mut arena = Vec::new();
        let alignment = entry
            .get("stream_alignment")
            .map(|v| number(v, "stream alignment"))
            .transpose()?
            .unwrap_or(1)
            .max(1);
        for (index, row) in rows.iter().enumerate() {
            let mut decoded = Vec::new();
            for component in components {
                let mut component = component.clone();
                if plan.is_array() {
                    component["frame"] = json!(index);
                }
                decoded.extend(build_component_cached(&original_ctx, &component)?.data);
            }
            if let Some(new) = derive_stream(&decoded, row, &arena)? {
                replacement[index] = new;
                changed += 1;
            }
            if plan.is_array() {
                let mut stream = encode_lz_stream(&decoded, row, &arena)?;
                stream.resize(stream.len().div_ceil(alignment) * alignment, 0);
                arena.extend(stream);
            }
        }
        let section = json_string(&entry["plan_section"], "plan section")?;
        let target = if section.starts_with('/') {
            candidate
                .pointer_mut(section)
                .ok_or("plan pointer missing")?
        } else {
            candidate.get_mut(section).ok_or("plan section missing")?
        };
        *target = if plan.is_array() {
            json!(replacement)
        } else {
            replacement.remove(0)
        };
    }
    let mut candidate_ctx = Context::new(root);
    candidate_ctx
        .documents
        .borrow_mut()
        .insert(path.to_path_buf(), std::rc::Rc::new(candidate.clone()));
    let mut verified = 0;
    for entry in &entries {
        if entry["plan"] != name {
            continue;
        }
        if build_entry(&mut original_ctx, entry)?.0 != build_entry(&mut candidate_ctx, entry)?.0 {
            return Err(format!(
                "derived compressor changed asset {}",
                entry["address"]
            ));
        }
        verified += 1;
        if verified % 64 == 0 {
            println!("byte-exact assets={verified}/{}", entries.len());
        }
    }
    super::store(root, path, candidate)?;
    println!("derived streams={changed} byte-exact assets={verified}");
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn predictors_preserve_literals_copies_ties_and_palette_groups() {
        for (decoded, mut plan) in [
            (
                vec![0, 0, 0, 0],
                json!({"codec":"golden-sun-kind2-lz","tokens":[2,[1,3]]}),
            ),
            (
                vec![0, 0, 0, 0],
                json!({"codec":"golden-sun-kind2-lz","tokens":[2,2,[2,2]]}),
            ),
            (
                b"ABCABCABC".to_vec(),
                json!({"codec":"golden-sun-general-lz","tokens":[["l",3],["c",6,3]]}),
            ),
            (
                (1..=9).collect(),
                json!({"codec":"golden-sun-palette-lz","tokens":[["z"],["g",[["l"],["e"]]]]}),
            ),
            (
                vec![0, 0, 0],
                json!({"codec":"golden-sun-general-lz-prefill","prefill":32,"tokens":[["c",3,1]]}),
            ),
        ] {
            plan["decoded_size"] = json!(decoded.len());
            let candidate = derive_stream(&decoded, &plan, &[]).unwrap().unwrap();
            let restored = materialize(&decoded, &candidate, &[]).unwrap();
            assert_eq!(
                restored,
                group(
                    flatten(&plan["tokens"], plan["codec"].as_str().unwrap()).unwrap(),
                    plan["codec"].as_str().unwrap()
                )
            );
            if plan["codec"] != "golden-sun-kind2-lz" {
                assert_eq!(
                    encode_lz_stream(&decoded, &plan, &[]).unwrap(),
                    encode_lz_stream(&decoded, &candidate, &[]).unwrap()
                );
            }
        }
        let plan = json!({"codec":"golden-sun-kind2-lz","tokens":[2,2,[2,2]]});
        let candidate = derive_stream(&[0, 0, 0, 0], &plan, &[]).unwrap().unwrap();
        assert_eq!(
            candidate["tokens"]["exceptions"],
            json!([[1, 2], [2, [2, 2]]])
        );
    }
    #[test]
    fn arena_predictor_uses_oldest_matches_and_preserves_split_and_exceptions() {
        let arena = [7, 8, 9, 7, 8, 9];
        let decoded = [7, 8, 9, 0];
        for distance in [8, 5] {
            let plan = json!({"codec":"golden-sun-arena-lz","decoded_size":4,"tokens":[["c",3,distance],["l",1]]});
            let candidate = derive_stream(&decoded, &plan, &arena).unwrap().unwrap();
            assert_eq!(candidate["tokens"]["split"], json!(3));
            assert_eq!(
                candidate["tokens"]["exceptions"].as_array().unwrap().len(),
                usize::from(distance == 5)
            );
            assert_eq!(
                encode_lz_stream(&decoded, &plan, &arena).unwrap(),
                encode_lz_stream(&decoded, &candidate, &arena).unwrap()
            );
        }
    }
    #[test]
    fn predictors_reject_unreachable_and_invalid_exception_records() {
        let base =
            json!({"codec":"golden-sun-kind2-lz","tokens":{"predictor":FORMAT,"exceptions":[]}});
        for exceptions in [
            json!([[2, 2]]),
            json!([[4, 2]]),
            json!([[1, [1, 0]]]),
            json!([[1, [1, 5]]]),
            json!([[1, 2], [0, 2]]),
        ] {
            let mut invalid = base.clone();
            invalid["tokens"]["exceptions"] = exceptions;
            assert!(materialize(&[0, 0, 0, 0], &invalid, &[]).is_err());
        }
        let mut invalid = base;
        invalid["tokens"]["predictor"] = json!("unknown");
        assert!(materialize(&[0], &invalid, &[]).is_err());
    }
}
