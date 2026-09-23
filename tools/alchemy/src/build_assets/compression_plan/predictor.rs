use super::*;
const FORMAT: &str = "greedy-lz-v1";
const LZSS_FORMAT: &str = "lzss";
/// Palette LZ searches this many bytes back from the byte it encodes.
const PALETTE_WINDOW: usize = 4092;
/// The palette format's largest copy distance, which its look-ahead reaches.
const PALETTE_REACH: usize = 4095;

/// How an LZSS compressor streams its input. It keeps `read_ahead` bytes of
/// input ahead of the byte it encodes in a ring of `window + read_ahead`
/// bytes and reads one more byte for each byte it consumes, so a copy may
/// start at any byte still in the ring, at most `max_distance` back. At the
/// end of the input reading stops and the ring's oldest byte stops
/// advancing: over the final `read_ahead` bytes the history grows by one
/// byte for each byte encoded, until `max_distance` limits it.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
struct Ring {
    window: usize,
    read_ahead: usize,
    max_distance: usize,
}
impl Ring {
    /// The oldest byte the ring holds while it encodes `position` of `size`
    /// input bytes: `read_ahead` bytes are read ahead unless the input ends.
    fn oldest(&self, position: usize, size: usize) -> usize {
        (position + self.read_ahead)
            .min(size)
            .saturating_sub(self.window + self.read_ahead)
    }
}

/// The LZSS compressors of a reference machine. Their settings were observed
/// in the shipped streams and are recorded, each with its evidence and no
/// credit, in the machine definition's `compressors`; the streams bound them
/// but do not derive them. General LZ records its window, read-ahead and
/// maximum distance; palette LZ its read-ahead, beside its fixed 4,092-byte
/// window and the format's largest distance, 4,095.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub(crate) struct LzMachine {
    general: Ring,
    palette: Ring,
}
impl LzMachine {
    /// The compressors a machine definition records. Each setting must be
    /// one observed, uncredited value with its evidence, inside the range
    /// its evidence allows.
    pub(crate) fn of(definition: &Value) -> Result<Self, String> {
        if definition["format"] != 1 {
            return Err("unsupported machine definition".into());
        }
        let setting = |codec: &str, name: &str| -> Result<usize, String> {
            let record = &definition["compressors"][codec][name];
            let label = format!("{} {name}", codec.replace("_lz", "-LZ"));
            let value = number(&record["value"], &label)?;
            let lowest = number(&record["allowed"]["lowest"], &label)?;
            let highest = number(&record["allowed"]["highest"], &label)?;
            if !(lowest..=highest).contains(&value)
                || record["confidence"] != "observed"
                || record["credit"] != "none"
                || !record["evidence"]
                    .as_array()
                    .is_some_and(|rows| !rows.is_empty() && rows.iter().all(Value::is_string))
            {
                return Err(format!(
                    "{label} must be one observed, uncredited value inside its allowed range, with its evidence"
                ));
            }
            Ok(value)
        };
        let general = Ring {
            window: setting("general_lz", "window")?,
            read_ahead: setting("general_lz", "read_ahead")?,
            max_distance: setting("general_lz", "max_distance")?,
        };
        if general.window == 0 || general.max_distance < general.window {
            return Err("general-LZ maximum distance must reach at least the window".into());
        }
        let palette = Ring {
            window: PALETTE_WINDOW,
            read_ahead: setting("palette_lz", "read_ahead")?,
            max_distance: PALETTE_REACH,
        };
        Ok(Self { general, palette })
    }
}

fn supported(codec: &str) -> bool {
    matches!(
        codec,
        "golden-sun-kind2-lz"
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
    /// The greedy token of tag-2 tile graphics and the legacy predictor.
    fn next(&mut self, position: usize, codec: &str) -> Value {
        let window = if codec.contains("palette") {
            4095
        } else {
            4123
        };
        self.search(position, codec, position.saturating_sub(window))
    }
    /// The nearest longest copy at `position` whose source is at least
    /// `lowest`, else a literal.
    fn search(&mut self, position: usize, codec: &str, lowest: usize) -> Value {
        let palette = codec.contains("palette");
        let maximum = (if palette { 272 } else { 137 }).min(self.data.len() - position);
        let mut length = 1;
        let mut distance = 0;
        if let Some(pair) = self.data.get(position..position + 2) {
            if let Some(positions) = self.pairs.get(&u16::from_le_bytes([pair[0], pair[1]])) {
                let start = positions.partition_point(|p| *p < lowest);
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
/// Predict complete controls from decoded bytes with nearest-longest matches
/// and one-byte lazy evaluation. The encoder defers a copy to a literal when
/// the copy at the next byte (longer than two bytes) covers at least as far
/// as the copy and the token after it. Both look-ahead searches copy only
/// from the history of the current position, so their distance limit grows
/// by the look-ahead offset, up to the largest distance.
///
/// Each codec's history is its ring in the reference machine ([`LzMachine`]),
/// which general and palette LZ both require.
///
/// A deferral suppresses lazy evaluation of the match that follows it. The
/// palette encoder then resumes lazy evaluation; the general encoder never
/// does, so a general stream defers at most once. Across the twelve ROMs the
/// first general opportunity deferred in 5,123 of 5,123 streams and none of
/// 1,316,283 later opportunities did.
fn lzss(decoded: &[u8], codec: &str, machine: Option<&LzMachine>) -> Result<Value, String> {
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
    let machine =
        machine.ok_or("LZSS compression needs the target's reference machine definition")?;
    let ring = if palette {
        machine.palette
    } else {
        machine.general
    };
    // The oldest byte in the history of `position`, and the largest distance.
    let history = |position: usize| ring.oldest(position, decoded.len());
    let reach = ring.max_distance;
    // A search at `at` from the history whose oldest byte is `oldest`.
    let lowest = |oldest: usize, at: usize| oldest.max(at.saturating_sub(reach));
    let mut matcher = Matcher::new(decoded);
    let mut deferred = false;
    let mut position = 0;
    let mut result = Vec::new();
    while position < decoded.len() {
        let oldest = history(position);
        let mut token = matcher.search(position, codec, lowest(oldest, position));
        let count = length(&token, codec)?;
        let lazy = !deferred;
        if palette {
            deferred = false;
        }
        if lazy && count > 1 && position + count < decoded.len() {
            let alternative = matcher.search(position + 1, codec, lowest(oldest, position + 1));
            let following =
                matcher.search(position + count, codec, lowest(oldest, position + count));
            let alternative_count = length(&alternative, codec)?;
            if alternative_count > 2 && alternative_count + 1 >= count + length(&following, codec)?
            {
                token = if palette {
                    json!(["l"])
                } else {
                    json!(["l", 1])
                };
                deferred = true;
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
#[cfg(test)]
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
fn tokens(decoded: &[u8], plan: &Value) -> Result<Value, String> {
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
    let exceptions = plan["tokens"]["exceptions"].as_array();
    let mut exception = 0;
    let mut position = prefill;
    let mut result = Vec::new();
    while position < data.len() {
        let saved_mtf = matcher.mtf;
        let predicted = matcher.next(position, codec);
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
/// A plan's controls: compressed from `decoded` on `machine`, which general
/// and palette LZ require, unless the plan still carries legacy recorded
/// controls.
pub(crate) fn materialize(
    decoded: &[u8],
    plan: &Value,
    machine: Option<&LzMachine>,
) -> Result<Value, String> {
    if plan.get("tokens").is_none() {
        return lzss(decoded, json_string(&plan["codec"], "codec")?, machine);
    }
    if plan["tokens"].is_array() {
        return Ok(plan["tokens"].clone());
    }
    if plan["tokens"]["predictor"] == LZSS_FORMAT {
        if plan["tokens"]["exceptions"] != json!([]) {
            return Err("LZSS compressor does not accept exceptions".into());
        }
        return lzss(decoded, json_string(&plan["codec"], "codec")?, machine);
    }
    if plan["tokens"]["predictor"] != FORMAT {
        return Err("unsupported LZ predictor".into());
    }
    tokens(decoded, plan)
}
#[cfg(test)]
impl LzMachine {
    /// A synthetic machine for tests of the compressor's rules: its general
    /// ring, and a palette ring that reads nothing ahead.
    pub(crate) const fn synthetic(window: usize, read_ahead: usize, max_distance: usize) -> Self {
        Self {
            general: Ring {
                window,
                read_ahead,
                max_distance,
            },
            palette: Ring {
                window: PALETTE_WINDOW,
                read_ahead: 0,
                max_distance: PALETTE_REACH,
            },
        }
    }
    /// This machine with a palette ring reading `read_ahead` bytes ahead.
    pub(crate) const fn with_palette_read_ahead(mut self, read_ahead: usize) -> Self {
        self.palette.read_ahead = read_ahead;
        self
    }
}
#[cfg(test)]
mod tests {
    use super::*;
    /// A synthetic machine whose history spans every short test input.
    const WIDE: LzMachine = LzMachine::synthetic(64, 8, 72);
    /// `size` distinct bytes, except that the pair at `copy` repeats the pair
    /// at `source`.
    fn distinct_with_copy(size: usize, source: usize, copy: usize) -> Vec<u8> {
        let mut decoded = (0..size as u8).collect::<Vec<_>>();
        decoded[copy] = decoded[source];
        decoded[copy + 1] = decoded[source + 1];
        decoded
    }
    /// The general-LZ token that `general` emits at `at`.
    fn general_token(decoded: &[u8], general: &LzMachine, at: usize) -> Value {
        let codec = "golden-sun-general-lz";
        let controls = lzss(decoded, codec, Some(general)).unwrap();
        let mut position = 0;
        for token in flatten(&controls, codec).unwrap() {
            if position == at {
                return token;
            }
            position += length(&token, codec).unwrap();
        }
        unreachable!()
    }
    #[test]
    fn general_history_grows_over_the_final_read_ahead_bytes() {
        // A 16-byte window and 4 bytes read ahead: a 20-byte ring.
        let general = LzMachine::synthetic(16, 4, 32);
        // The ring's oldest byte trails the window until reading stops.
        assert_eq!(general.general.oldest(10, 40), 0);
        assert_eq!(general.general.oldest(30, 40), 14);
        assert_eq!(general.general.oldest(36, 40), 20);
        assert_eq!(general.general.oldest(39, 40), 20);
        // A pair 17 bytes back is out of reach with four bytes left and in
        // reach with three; with two left the history reaches 18 bytes.
        for (left, distance, expected) in [
            (4, 17, json!(["l", 1])),
            (3, 17, json!(["c", 2, 17])),
            (2, 18, json!(["c", 2, 18])),
            (2, 19, json!(["l", 1])),
        ] {
            let at = 40 - left;
            let decoded = distinct_with_copy(40, at - distance, at);
            assert_eq!(general_token(&decoded, &general, at), expected);
        }
        // Mid-stream the same pair 17 bytes back stays out of reach.
        let decoded = distinct_with_copy(60, 13, 30);
        assert_eq!(general_token(&decoded, &general, 30), json!(["l", 1]));
    }
    #[test]
    fn general_history_growth_stops_at_the_maximum_distance() {
        // Eight bytes read ahead open 22 bytes of history two bytes before
        // the end, but the machine copies at most 18 bytes back.
        let general = LzMachine::synthetic(16, 8, 18);
        assert_eq!(general.general.oldest(38, 40), 16);
        for (distance, expected) in [(18, json!(["c", 2, 18])), (19, json!(["l", 1]))] {
            let decoded = distinct_with_copy(40, 38 - distance, 38);
            assert_eq!(general_token(&decoded, &general, 38), expected);
        }
    }
    #[test]
    fn general_lazy_look_ahead_searches_the_current_history() {
        // At 30 a two-byte copy (distance 5); at 31 a three-byte copy from
        // 14, the ring's oldest byte while 30 is encoded (distance 17); at
        // 32 a two-byte copy. Distinct bytes elsewhere.
        let mut decoded = (0..50u8).collect::<Vec<_>>();
        decoded[26] = 14;
        decoded[30] = 25;
        decoded[31..34].copy_from_slice(&[14, 15, 16]);
        let general = LzMachine::synthetic(16, 4, 18);
        assert_eq!(general.general.oldest(30, 50), 14);
        // Both look-aheads search from 30's oldest byte, so the copy at 31
        // covers as far as the two greedy copies and 30 defers. Once 31 is
        // encoded, byte 14 has left the ring: the copy is gone.
        for (at, expected) in [(30, json!(["l", 1])), (31, json!(["l", 1]))] {
            assert_eq!(general_token(&decoded, &general, at), expected);
        }
        // The look-ahead also copies at most the maximum distance back.
        let general = LzMachine::synthetic(16, 4, 16);
        assert_eq!(general_token(&decoded, &general, 30), json!(["c", 2, 5]));
    }
    #[test]
    fn machine_settings_are_observed_uncredited_and_inside_their_evidence() {
        let setting = |value: usize, lowest: usize, highest: usize| {
            json!({"value": value, "allowed": {"lowest": lowest, "highest": highest},
                "confidence": "observed", "evidence": ["test"], "credit": "none"})
        };
        let definition = json!({"format": 1, "compressors": {
            "general_lz": {
                "window": setting(16, 16, 16),
                "read_ahead": setting(4, 2, 6),
                "max_distance": setting(18, 17, 18),
            },
            "palette_lz": {"read_ahead": setting(5, 3, 9)},
        }});
        assert_eq!(
            LzMachine::of(&definition),
            Ok(LzMachine::synthetic(16, 4, 18).with_palette_read_ahead(5))
        );
        for (codec, name, key, value) in [
            ("general_lz", "window", "confidence", json!("inferred")),
            ("general_lz", "read_ahead", "credit", json!("exact")),
            ("general_lz", "read_ahead", "evidence", json!([])),
            ("general_lz", "read_ahead", "value", json!(7)),
            (
                "general_lz",
                "max_distance",
                "allowed",
                json!({"lowest": 17}),
            ),
            ("general_lz", "max_distance", "value", json!(15)),
            ("palette_lz", "read_ahead", "confidence", json!("assumed")),
            ("palette_lz", "read_ahead", "evidence", json!([])),
            ("palette_lz", "read_ahead", "value", json!(10)),
        ] {
            let mut invalid = definition.clone();
            invalid["compressors"][codec][name][key] = value;
            assert!(LzMachine::of(&invalid).is_err(), "{codec} {name} {key}");
        }
        // Palette LZ's read-ahead is required.
        let mut invalid = definition.clone();
        invalid["compressors"]["palette_lz"] = json!({});
        assert!(LzMachine::of(&invalid).is_err());
        // A maximum distance below the window cannot be.
        let mut invalid = definition;
        invalid["compressors"]["general_lz"]["max_distance"] = setting(15, 15, 15);
        assert!(LzMachine::of(&invalid).is_err());
    }
    #[test]
    fn both_games_record_one_lz_machine() {
        use crate::targets::{target_for, DecompTargetId};
        let root = repository_root();
        let tbs = crate::build_assets::target_lz_machine(&root, &target_for(DecompTargetId::TbsEn));
        let tla = crate::build_assets::target_lz_machine(&root, &target_for(DecompTargetId::TlaEn));
        assert!(tbs.is_ok(), "{tbs:?}");
        assert_eq!(tbs, tla);
    }
    /// Palette LZ at `at` of a sequence with distinct byte pairs, except for a
    /// two-byte copy at `at` (distance 100), a three-byte copy at `at + 1`
    /// (distance 201) and a three-byte copy at `at + 2` from `far` bytes back.
    fn planted_palette_token(far: usize) -> Value {
        let mut decoded = Vec::new();
        let mut value = 0u8;
        for index in 0..4400usize {
            decoded.push(value);
            value = value.wrapping_add((2 * (index / 256) + 1) as u8);
        }
        let at = 4300;
        let (near, alternative, following) = (at - 100, at - 200, at + 2 - far);
        decoded[near + 1] = decoded[alternative];
        decoded[following] = decoded[alternative + 1];
        decoded[following + 1] = decoded[alternative + 2];
        decoded[at] = decoded[near];
        decoded[at + 1] = decoded[alternative];
        decoded[at + 2] = decoded[alternative + 1];
        decoded[at + 3] = decoded[alternative + 2];
        decoded[at + 4] = decoded[following + 2];
        let flat = flatten(
            &lzss(&decoded, "golden-sun-palette-lz", Some(&WIDE)).unwrap(),
            "golden-sun-palette-lz",
        )
        .unwrap();
        let mut position = 0;
        for token in flat {
            if position == at {
                return token;
            }
            position += length(&token, "golden-sun-palette-lz").unwrap();
        }
        unreachable!()
    }
    #[test]
    fn lookahead_matches_reach_past_the_window_by_their_offset() {
        // The following copy at `at + 2` is visible from `at` up to distance
        // 4092 + 2, so the two greedy copies cover more and the copy stays.
        assert_eq!(planted_palette_token(4094), json!(["c", 2, 100]));
        // One byte further it is out of reach and the copy is deferred.
        assert_eq!(planted_palette_token(4095), json!(["l"]));
    }
    /// The palette-LZ token that a machine reading `read_ahead` bytes ahead
    /// emits `left` bytes before the end of a 4,400-byte sequence with
    /// distinct byte pairs, except that the pair there repeats the pair
    /// `distance` bytes back.
    fn palette_end_token(read_ahead: usize, left: usize, distance: usize) -> Value {
        let codec = "golden-sun-palette-lz";
        let mut decoded = Vec::new();
        let mut value = 0u8;
        for index in 0..4400usize {
            decoded.push(value);
            value = value.wrapping_add((2 * (index / 256) + 1) as u8);
        }
        let at = decoded.len() - left;
        decoded[at] = decoded[at - distance];
        decoded[at + 1] = decoded[at - distance + 1];
        let machine = WIDE.with_palette_read_ahead(read_ahead);
        let mut position = 0;
        for token in flatten(&lzss(&decoded, codec, Some(&machine)).unwrap(), codec).unwrap() {
            if position == at {
                return token;
            }
            position += length(&token, codec).unwrap();
        }
        unreachable!()
    }
    #[test]
    fn palette_history_grows_over_the_final_read_ahead_bytes() {
        // Mid-stream and without read-ahead the window is 4,092 bytes.
        assert_eq!(palette_end_token(0, 8, 4092), json!(["c", 2, 4092]));
        assert_eq!(palette_end_token(0, 8, 4093), json!(["l"]));
        assert_eq!(palette_end_token(8, 9, 4093), json!(["l"]));
        // Reading 8 bytes ahead, the history grows by one byte for each of
        // the last 8 bytes encoded, up to the format's distance of 4,095.
        assert_eq!(palette_end_token(8, 7, 4093), json!(["c", 2, 4093]));
        assert_eq!(palette_end_token(8, 6, 4094), json!(["c", 2, 4094]));
        assert_eq!(palette_end_token(8, 6, 4095), json!(["l"]));
        assert_eq!(palette_end_token(8, 2, 4095), json!(["c", 2, 4095]));
        assert_eq!(palette_end_token(8, 2, 4096), json!(["l"]));
    }
    #[test]
    fn lzss_replaces_equal_coverage_once_for_general_and_repeatedly_for_palette() {
        let decoded = b"abcXbcdefYabcdefghiUhijklVghijkl";
        for (codec, second_is_literal) in [
            ("golden-sun-general-lz", false),
            ("golden-sun-tagged-palette-lz", true),
        ] {
            let controls = lzss(decoded, codec, Some(&WIDE)).unwrap();
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
                encode_lz_stream(decoded, &automatic, &[], Some(&WIDE)).unwrap(),
                encode_lz_stream(decoded, &plan, &[], Some(&WIDE)).unwrap()
            );
            let mut invalid = plan.clone();
            invalid["tokens"] = json!({"predictor":LZSS_FORMAT,"exceptions":[[0, ["l", 1]]]});
            assert!(materialize(decoded, &invalid, Some(&WIDE)).is_err());
        }
        assert!(lzss(b"abc", "golden-sun-general-lz-prefill", Some(&WIDE)).is_err());
        // General and palette LZ both need their machine.
        for codec in ["golden-sun-general-lz", "golden-sun-palette-lz"] {
            assert!(lzss(decoded, codec, None)
                .unwrap_err()
                .contains("reference machine definition"));
        }
    }
    #[test]
    fn legacy_predictor_exceptions_still_materialize() {
        let legacy = json!({"codec":"golden-sun-kind2-lz","tokens":{"predictor":FORMAT,"exceptions":[[1,2],[2,[2,2]]]}});
        assert_eq!(
            materialize(&[0, 0, 0, 0], &legacy, None).unwrap(),
            json!([2, 2, [2, 2]])
        );
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
            assert!(materialize(&[0, 0, 0, 0], &invalid, None).is_err());
        }
        let mut invalid = base;
        invalid["tokens"]["predictor"] = json!("unknown");
        assert!(materialize(&[0], &invalid, None).is_err());
    }
    #[test]
    fn tile_compressor_derives_copies_and_literal_widths_from_pixels() {
        let decoded = [0x21, 0x12, 0x21, 0x12, 0x21, 0x12, 0xfe];
        let plan = json!({"codec":"golden-sun-kind2-lz"});
        assert_eq!(
            materialize(&decoded, &plan, None).unwrap(),
            json!([2, 2, [2, 4], 4])
        );
        let decoded = b"abcXbcdefYabcdefghiUhijklVghijkl";
        let tokens = materialize(decoded, &plan, None).unwrap();
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
