use super::*;
const FORMAT: &str = "greedy-lz-v1";
const LZSS_FORMAT: &str = "lzss";

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
pub(super) fn restore_defaults(value: &mut Value) {
    match value {
        Value::Object(object) => {
            if object.contains_key("exceptions") {
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
            }
            for child in object.values_mut() {
                restore_defaults(child);
            }
        }
        Value::Array(rows) => {
            for row in rows {
                restore_defaults(row);
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
        self.next_with_window(
            position,
            codec,
            if codec.contains("palette") {
                4095
            } else {
                4123
            },
        )
    }
    fn next_with_window(&mut self, position: usize, codec: &str, window: usize) -> Value {
        let palette = codec.contains("palette");
        let maximum = (if palette { 272 } else { 137 }).min(self.data.len() - position);
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
/// Predict complete controls from decoded bytes. A literal is worthwhile when
/// the following copy covers at least as much as the two greedy copies.
/// General LZ uses this replacement once per stream; palette LZ repeats it.
fn lzss(decoded: &[u8], codec: &str) -> Result<Value, String> {
    if codec == "golden-sun-kind2-lz" {
        // Tile graphics use greedy copies. Only literals update the nibble
        // move-to-front table; a copied byte never enters that table.
        let mut matcher = Matcher::new(decoded);
        let mut position = 0;
        let mut result = Vec::new();
        while position < decoded.len() {
            let token = matcher.next(position, codec);
            position += length(&token, codec)?;
            result.push(token);
        }
        return Ok(json!(result));
    }
    let palette = match codec {
        "golden-sun-general-lz" => false,
        "golden-sun-palette-lz" | "golden-sun-tagged-palette-lz" => true,
        _ => return Err("LZSS compressor does not support this codec".into()),
    };
    let window = if palette { 4092 } else { 4123 };
    let mut matcher = Matcher::new(decoded);
    let mut used = false;
    let mut position = 0;
    let mut result = Vec::new();
    while position < decoded.len() {
        let mut token = matcher.next_with_window(position, codec, window);
        let count = length(&token, codec)?;
        if (!used || palette) && count > 1 && position + count < decoded.len() {
            let alternative = matcher.next_with_window(position + 1, codec, window);
            let following = matcher.next_with_window(position + count, codec, window);
            let alternative_count = length(&alternative, codec)?;
            if alternative_count > 2 && alternative_count + 1 >= count + length(&following, codec)?
            {
                token = if palette {
                    json!(["l"])
                } else {
                    json!(["l", 1])
                };
                used = true;
            }
        }
        position += length(&token, codec)?;
        result.push(token);
    }
    Ok(group(result, codec))
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
fn tokens(decoded: &[u8], plan: &Value, arena: &[u8]) -> Result<Value, String> {
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
        number(&plan["tokens"]["split"], "arena split")?
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
    while position < data.len() {
        let saved_mtf = matcher.mtf;
        let predicted = if let Some(arena) = &arena_matcher {
            arena.arena_next(&data, position)
        } else {
            matcher.next(position, codec)
        };
        let actual = if let Some(rows) = exceptions {
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
    if exceptions.is_some_and(|rows| exception != rows.len()) {
        return Err("predictor has unused records".into());
    }
    Ok(group(result, codec))
}
pub(crate) fn materialize(decoded: &[u8], plan: &Value, arena: &[u8]) -> Result<Value, String> {
    if plan.get("tokens").is_none() {
        return lzss(decoded, json_string(&plan["codec"], "codec")?);
    }
    if plan["tokens"].is_array() {
        return Ok(plan["tokens"].clone());
    }
    if plan["tokens"]["predictor"] == LZSS_FORMAT {
        if plan["tokens"]["exceptions"] != json!([]) {
            return Err("LZSS compressor does not accept exceptions".into());
        }
        return lzss(decoded, json_string(&plan["codec"], "codec")?);
    }
    if plan["tokens"]["predictor"] != FORMAT {
        return Err("unsupported LZ predictor".into());
    }
    tokens(decoded, plan, arena)
}
/// Admit only controls independently reproduced from the decoded input.
/// Never turn an encoder mismatch into an exception or explicit-token fallback.
pub(crate) fn checked_plan(decoded: &[u8], plan: &Value) -> Result<Value, String> {
    if plan.get("lookahead").is_some_and(|value| value != "") {
        return Err(
            "packing is not recovered; copying trailing reference bytes is forbidden".into(),
        );
    }
    let codec = json_string(&plan["codec"], "codec")?;
    let predicted = lzss(decoded, codec)?;
    let expected = group(flatten(&plan["tokens"], codec)?, codec);
    if predicted != expected {
        return Err(
            "compressor does not reproduce this stream; recording token exceptions is forbidden"
                .into(),
        );
    }
    let mut candidate = plan.clone();
    candidate["tokens"] = json!({"predictor":LZSS_FORMAT,"exceptions":[]});
    Ok(candidate)
}
#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn lzss_replaces_equal_coverage_once_for_general_and_repeatedly_for_palette() {
        let decoded = b"abcXbcdefYabcdefghiUhijklVghijkl";
        for (codec, second_is_literal) in [
            ("golden-sun-general-lz", false),
            ("golden-sun-tagged-palette-lz", true),
        ] {
            let controls = lzss(decoded, codec).unwrap();
            let flat = flatten(&controls, codec).unwrap();
            let mut position = 0;
            let mut checked = 0;
            for token in &flat {
                if position == 10 {
                    assert_eq!(token[0], "l");
                    checked += 1;
                }
                if position == 26 {
                    assert_eq!(token[0] == "l", second_is_literal);
                    checked += 1;
                }
                position += length(token, codec).unwrap();
            }
            assert_eq!(position, decoded.len());
            assert_eq!(checked, 2);
            let plan =
                json!({"codec":codec,"decoded_size":decoded.len(),"tokens":controls,"tag":1});
            let mut automatic = plan.clone();
            automatic.as_object_mut().unwrap().remove("tokens");
            assert_eq!(
                encode_lz_stream(decoded, &automatic, &[]).unwrap(),
                encode_lz_stream(decoded, &plan, &[]).unwrap()
            );
            let compact = checked_plan(decoded, &plan).unwrap();
            assert_eq!(compact["tokens"]["predictor"], LZSS_FORMAT);
            assert_eq!(
                encode_lz_stream(decoded, &compact, &[]).unwrap(),
                encode_lz_stream(decoded, &plan, &[]).unwrap()
            );
            let mut invalid = compact;
            invalid["tokens"]["exceptions"] = json!([[0, ["l", 1]]]);
            assert!(materialize(decoded, &invalid, &[]).is_err());
        }
        assert!(lzss(b"abc", "golden-sun-general-lz-prefill").is_err());
    }
    #[test]
    fn export_refuses_mismatches_instead_of_recording_them() {
        for padding in ["00", "aabb"] {
            let padded = json!({"codec":"golden-sun-general-lz","tokens":[["l",2],["c",6,2]],"lookahead":padding});
            assert!(checked_plan(b"ABABABAB", &padded)
                .unwrap_err()
                .contains("copying trailing reference bytes is forbidden"));
        }
        let plan = json!({"codec":"golden-sun-general-lz","tokens":[["l",8]]});
        assert!(checked_plan(b"ABABABAB", &plan)
            .unwrap_err()
            .contains("recording token exceptions is forbidden"));
        let unsupported = json!({"codec":"golden-sun-halfword-lz","tokens":[["l",1]]});
        assert!(checked_plan(&[0, 0, 0, 0], &unsupported).is_err());
        let legacy = json!({"codec":"golden-sun-kind2-lz","tokens":{"predictor":FORMAT,"exceptions":[[1,2],[2,[2,2]]]}});
        assert_eq!(
            materialize(&[0, 0, 0, 0], &legacy, &[]).unwrap(),
            json!([2, 2, [2, 2]])
        );
    }
    #[test]
    fn arena_predictor_uses_oldest_matches_and_preserves_split_and_exceptions() {
        let arena = [7, 8, 9, 7, 8, 9];
        let decoded = [7, 8, 9, 0];
        for distance in [8, 5] {
            let plan = json!({"codec":"golden-sun-arena-lz","decoded_size":4,"tokens":[["c",3,distance],["l",1]]});
            let candidate = json!({"codec":"golden-sun-arena-lz","decoded_size":4,"tokens":{"predictor":FORMAT,"split":3,"exceptions":if distance == 8 {json!([])} else {json!([[0,["c",3,5]]])}}});
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
    #[test]
    fn tile_compressor_derives_copies_and_literal_widths_from_pixels() {
        let decoded = [0x21, 0x12, 0x21, 0x12, 0x21, 0x12, 0xfe];
        let plan = json!({"codec":"golden-sun-kind2-lz"});
        assert_eq!(
            materialize(&decoded, &plan, &[]).unwrap(),
            json!([2, 2, [2, 4], 4])
        );
        let decoded = b"abcXbcdefYabcdefghiUhijklVghijkl";
        let tokens = materialize(decoded, &plan, &[]).unwrap();
        let mut position = 0;
        let mut checked = false;
        for token in tokens.as_array().unwrap() {
            if position == 10 {
                assert_eq!(token, &json!([10, 3]));
                checked = true;
            }
            position += length(token, "golden-sun-kind2-lz").unwrap();
        }
        assert_eq!(position, decoded.len());
        assert!(checked);
    }
}
