//! Rebuild the static character sprite series from maintained images and plans.
//!
//! The JSON plans deliberately stay dynamic: the tracked plans contain the
//! compiler's structural traces (including tuple-shaped compression tokens).
//! Keeping the representation as `serde_json::Value` preserves their order.

use std::fs;
use std::path::{Path, PathBuf};

use extract_resource::{GeneralToken, PaletteGroup, PaletteOperation};
use import_asset::{indexed_png, Rgb};
use serde_json::{json, Map, Value};

const MAX_PIXELS: usize = 0x1000000;

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Error(pub String);
impl std::fmt::Display for Error {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        f.write_str(&self.0)
    }
}
impl std::error::Error for Error {}
type Result<T> = std::result::Result<T, Error>;
fn fail<T>(message: impl Into<String>) -> Result<T> {
    Err(Error(message.into()))
}

pub fn parse_integer(text: &str, name: &str) -> Result<i64> {
    let parsed = if let Some(digits) = text.strip_prefix("0x").or_else(|| text.strip_prefix("0X")) {
        i128::from_str_radix(digits, 16)
            .ok()
            .and_then(|v| i64::try_from(v).ok())
    } else {
        text.parse::<f64>()
            .ok()
            .filter(|v| v.is_finite() && v.fract() == 0.0 && v.abs() <= 9_007_199_254_740_991.0)
            .map(|v| v as i64)
    };
    parsed.ok_or_else(|| Error(format!("invalid {name}")))
}
fn int(value: &Value, name: &str) -> Result<i64> {
    match value {
        Value::String(s) => parse_integer(s, name),
        Value::Number(n) => n.as_i64().ok_or_else(|| Error(format!("invalid {name}"))),
        _ => fail(format!("invalid {name}")),
    }
}
fn required<'a>(value: &'a Value, name: &str) -> Result<&'a Value> {
    value
        .get(name)
        .ok_or_else(|| Error(format!("missing {name}")))
}
fn array<'a>(value: &'a Value, name: &str) -> Result<&'a Vec<Value>> {
    value
        .get(name)
        .and_then(Value::as_array)
        .ok_or_else(|| Error(format!("invalid {name}")))
}
pub fn read_json(path: &Path) -> Result<Value> {
    serde_json::from_slice(&fs::read(path).map_err(|e| Error(format!("{}: {e}", path.display())))?)
        .map_err(|e| Error(format!("{}: {e}", path.display())))
}

fn bounded(value: i64, min: i64, max: i64, name: &str) -> Result<i64> {
    if value < min || value > max {
        fail(format!("invalid {name}"))
    } else {
        Ok(value)
    }
}

fn palette(path: &Path, offset: i64, entries: i64) -> Result<Vec<Rgb>> {
    let image =
        indexed_png(&fs::read(path).map_err(|e| Error(e.to_string()))?).map_err(|e| Error(e.0))?;
    bounded(offset, 0, 255, "static-sprite palette offset")?;
    bounded(entries, 1, 256 - offset, "static-sprite palette entries")?;
    let offset = offset as usize;
    let entries = entries as usize;
    if offset + entries > image.pixels.len() {
        return fail("shared palette PNG is too small");
    }
    image.pixels[offset..offset + entries]
        .iter()
        .map(|pixel| {
            image
                .palette
                .get(*pixel as usize)
                .copied()
                .ok_or_else(|| Error("shared palette PNG references a missing color".into()))
        })
        .collect()
}

fn atlas_frames(
    data: &[u8],
    width: usize,
    height: usize,
    count: usize,
    columns: usize,
    colors: &[Rgb],
) -> Result<Vec<Vec<u8>>> {
    let image = indexed_png(data).map_err(|e| Error(e.0))?;
    if image.palette != colors
        || image.width as usize != columns * width
        || image.height as usize != count.div_ceil(columns) * height
    {
        return fail("sprite atlas dimensions or palette differ from its plan");
    }
    let mut output = Vec::with_capacity(count);
    let pixels: Vec<u8> = image.pixels.into_iter().map(|p| p as u8).collect();
    for index in 0..count {
        let mut frame = vec![0; width * height];
        let left = index % columns * width;
        let top = index / columns * height;
        for y in 0..height {
            frame[y * width..(y + 1) * width].copy_from_slice(
                &pixels[(top + y) * image.width as usize + left
                    ..(top + y) * image.width as usize + left + width],
            );
        }
        output.push(frame);
    }
    for index in count..(count.div_ceil(columns) * columns) {
        let left = index % columns * width;
        let top = index / columns * height;
        for y in 0..height {
            if pixels[(top + y) * image.width as usize + left
                ..(top + y) * image.width as usize + left + width]
                .iter()
                .any(|p| *p != 0)
            {
                return fail("sprite atlas padding must be transparent");
            }
        }
    }
    Ok(output)
}

fn zero_decode(data: &[u8], offset: usize, max: usize) -> Result<(Vec<u8>, usize)> {
    let mut out = Vec::new();
    let mut cursor = offset;
    while cursor < data.len() {
        let value = data[cursor];
        cursor += 1;
        if value == 0 {
            return Ok((out, cursor - offset));
        } else if value <= 0xdf {
            if out.len() >= max {
                return fail("zero-skip pixel limit exceeded");
            }
            out.push(value);
        } else {
            let count = value as usize - 0xdf;
            if out.len() + count > max {
                return fail("zero-skip pixel limit exceeded");
            }
            out.extend(std::iter::repeat_n(0, count));
        }
    }
    fail("zero-skip stream has no terminator")
}
fn zero_encode(pixels: &[u8]) -> Result<Vec<u8>> {
    let mut out = Vec::new();
    let mut cursor = 0;
    while cursor < pixels.len() {
        if pixels[cursor] != 0 {
            if pixels[cursor] > 0xdf {
                return fail("zero-skip literal is outside 1..223");
            }
            out.push(pixels[cursor]);
            cursor += 1;
        } else {
            let mut end = cursor + 1;
            while end < pixels.len() && pixels[end] == 0 {
                end += 1;
            }
            let mut count = end - cursor;
            while count > 0 {
                let n = count.min(32);
                out.push(0xdf + n as u8);
                count -= n;
            }
            cursor = end;
        }
    }
    out.push(0);
    Ok(out)
}

fn mode3_decode(data: &[u8], offset: usize) -> Result<(Vec<u8>, usize, Value)> {
    if offset + 2 > data.len() {
        return fail("mode-3 frame offset is truncated");
    }
    let split = u16::from_le_bytes([data[offset], data[offset + 1]]) as usize;
    let base = offset + 2;
    if split == 0 {
        let (_pixels, size) = zero_decode(data, base, MAX_PIXELS)?;
        return Ok((
            data[base..base + size].to_vec(),
            2 + size,
            json!({"split":0}),
        ));
    }
    if split < 2 || offset + split >= data.len() {
        return fail("mode-3 split is outside the encoded arena");
    }
    let control_start = offset + split;
    let mut literal = base;
    let mut cursor = control_start;
    let mut output = Vec::new();
    let mut operations = Vec::new();
    let final_flags;
    'groups: loop {
        if cursor >= data.len() {
            return fail("mode-3 flag group is truncated");
        }
        let flags = data[cursor];
        cursor += 1;
        for bit in 0..8 {
            if flags & (1 << bit) != 0 {
                if literal >= control_start {
                    return fail("mode-3 literal prefix is exhausted");
                }
                output.push(data[literal]);
                literal += 1;
                if let Some(last) = operations.last_mut().and_then(Value::as_array_mut) {
                    if last.first().and_then(Value::as_str) == Some("l") {
                        let n = last[1].as_u64().unwrap() + 1;
                        last[1] = json!(n);
                        continue;
                    }
                }
                operations.push(json!(["l", 1]));
            } else if cursor + 2 > data.len() {
                return fail("mode-3 copy token is truncated");
            } else {
                let word = u16::from_be_bytes([data[cursor], data[cursor + 1]]);
                cursor += 2;
                if word == 0 {
                    final_flags = (u16::from(flags) & !((1u16 << (bit + 1)) - 1)) as u64;
                    break 'groups;
                }
                let distance = (word & 0x0fff) as usize;
                if distance == 0 {
                    return fail("mode-3 copy has zero distance");
                }
                let nibble = word >> 12;
                let length = if nibble == 0 {
                    if cursor >= data.len() {
                        return fail("mode-3 extended length is truncated");
                    }
                    data[cursor] as usize + 18
                } else {
                    nibble as usize + 2
                };
                if nibble == 0 {
                    cursor += 1;
                }
                let source = base
                    .checked_sub(distance)
                    .ok_or_else(|| Error("mode-3 copy is outside the encoded arena".into()))?;
                if source + length > data.len() {
                    return fail("mode-3 copy is outside the encoded arena");
                }
                for i in 0..length {
                    output.push(data[source + i]);
                }
                operations.push(json!(["c", distance, length]));
            }
        }
    }
    if literal != control_start {
        return fail("mode-3 literal prefix was not consumed exactly");
    }
    let mut wrapper = Map::new();
    wrapper.insert("split".into(), json!(split));
    wrapper.insert("operations".into(), Value::Array(operations));
    if final_flags != 0 {
        wrapper.insert("final_flags".into(), json!(final_flags));
    }
    Ok((output, cursor - offset, Value::Object(wrapper)))
}

fn copy_word(distance: usize, length: usize) -> Result<(u16, Option<u8>)> {
    if !(1..=0xfff).contains(&distance) || !(3..=273).contains(&length) {
        fail("mode-3 copy token is invalid")
    } else if length <= 17 {
        Ok((((length - 2) << 12 | distance) as u16, None))
    } else {
        Ok((distance as u16, Some((length - 18) as u8)))
    }
}
fn mode3_encode(stream: &[u8], wrapper: &Value, preceding: &[u8]) -> Result<Vec<u8>> {
    let split = int(required(wrapper, "split")?, "mode-3 split")? as usize;
    if split == 0 {
        let mut out = vec![0, 0];
        out.extend_from_slice(stream);
        return Ok(out);
    }
    let operations = array(wrapper, "operations")?;
    let mut literals = Vec::new();
    let mut bits = Vec::new();
    let mut payloads: Vec<Option<Vec<u8>>> = Vec::new();
    let mut output = 0;
    for op in operations {
        let values = op
            .as_array()
            .ok_or_else(|| Error("unknown mode-3 operation".into()))?;
        match values.first().and_then(Value::as_str) {
            Some("l") => {
                let count = int(
                    values
                        .get(1)
                        .ok_or_else(|| Error("mode-3 literal run is invalid".into()))?,
                    "mode-3 literal run",
                )? as usize;
                if output + count > stream.len() {
                    return fail("mode-3 literal exceeds decoded stream");
                }
                literals.extend_from_slice(&stream[output..output + count]);
                output += count;
                for _ in 0..count {
                    bits.push(true);
                    payloads.push(None);
                }
            }
            Some("c") => {
                let distance = int(
                    values
                        .get(1)
                        .ok_or_else(|| Error("mode-3 copy operation is invalid".into()))?,
                    "mode-3 copy distance",
                )? as usize;
                let length = int(
                    values
                        .get(2)
                        .ok_or_else(|| Error("mode-3 copy operation is invalid".into()))?,
                    "mode-3 copy length",
                )? as usize;
                let (word, extra) = copy_word(distance, length)?;
                if output + length > stream.len() {
                    return fail("mode-3 copy exceeds decoded stream");
                }
                output += length;
                bits.push(false);
                let mut payload = vec![(word >> 8) as u8, word as u8];
                if let Some(extra) = extra {
                    payload.push(extra);
                }
                payloads.push(Some(payload));
            }
            _ => return fail("unknown mode-3 operation"),
        }
    }
    if output != stream.len() || literals.len() + 2 != split {
        return fail("mode-3 plan does not cover decoded stream");
    }
    bits.push(false);
    payloads.push(Some(vec![0, 0]));
    let final_flags = int(
        wrapper.get("final_flags").unwrap_or(&json!(0)),
        "mode-3 final flags",
    )? as u8;
    let final_used = bits.len() % 8;
    let final_used = if final_used == 0 { 8 } else { final_used };
    let mask = !((1u16 << final_used) - 1) as u8;
    if final_flags & !mask != 0 {
        return fail("mode-3 final flags overlap planned operations");
    }
    let mut control = Vec::new();
    for group in (0..bits.len()).step_by(8) {
        let used = (bits.len() - group).min(8);
        let last = group + used == bits.len();
        let mut flags = if last { final_flags } else { 0 };
        for bit in 0..used {
            if bits[group + bit] {
                flags |= 1 << bit;
            }
        }
        control.push(flags);
        for bit in 0..used {
            if let Some(payload) = &payloads[group + bit] {
                control.extend_from_slice(payload);
            }
        }
    }
    let mut frame = Vec::new();
    frame.extend_from_slice(&(split as u16).to_le_bytes());
    frame.extend_from_slice(&literals);
    frame.extend_from_slice(&control);
    let mut combined = preceding.to_vec();
    combined.extend_from_slice(&frame);
    let base = preceding.len() + 2;
    let mut outpos = 0;
    for op in operations {
        let values = op.as_array().unwrap();
        if values[0].as_str() == Some("l") {
            outpos += int(&values[1], "mode-3 literal run")? as usize;
        } else {
            let distance = int(&values[1], "mode-3 copy distance")? as usize;
            let length = int(&values[2], "mode-3 copy length")? as usize;
            let source = base
                .checked_sub(distance)
                .ok_or_else(|| Error("mode-3 copy source has not been reconstructed".into()))?;
            if source + length > combined.len() {
                return fail("mode-3 copy source has not been reconstructed");
            }
            for i in 0..length {
                if stream[outpos + i] != combined[source + i] {
                    return fail("mode-3 copy differs from the encoded arena dictionary");
                }
            }
            outpos += length;
        }
    }
    Ok(frame)
}

fn parse_general(value: &Value) -> Result<Vec<GeneralToken>> {
    array_value(value)?
        .iter()
        .map(|item| {
            let a = item
                .as_array()
                .ok_or_else(|| Error("invalid general-stream token".into()))?;
            match a.first().and_then(Value::as_str) {
                Some("l") => Ok(GeneralToken::Literal(int(&a[1], "literal run")? as u32)),
                Some("c") => Ok(GeneralToken::Copy {
                    length: int(&a[1], "copy length")? as u32,
                    distance: int(&a[2], "copy distance")? as u32,
                }),
                _ => fail("invalid general-stream token"),
            }
        })
        .collect()
}
fn parse_palette(value: &Value) -> Result<Vec<PaletteGroup>> {
    array_value(value)?
        .iter()
        .map(|item| {
            let a = item
                .as_array()
                .ok_or_else(|| Error("invalid palette token group".into()))?;
            match a.first().and_then(Value::as_str) {
                Some("z") => Ok(PaletteGroup::Zeros),
                Some("g") => {
                    let ops = a
                        .get(1)
                        .and_then(Value::as_array)
                        .ok_or_else(|| Error("invalid palette token group".into()))?;
                    Ok(PaletteGroup::Group(
                        ops.iter()
                            .map(|op| {
                                let x = op
                                    .as_array()
                                    .ok_or_else(|| Error("invalid palette token".into()))?;
                                match x.first().and_then(Value::as_str) {
                                    Some("l") => Ok(PaletteOperation::Literal),
                                    Some("e") => Ok(PaletteOperation::End),
                                    Some("c") => Ok(PaletteOperation::Copy {
                                        length: int(&x[1], "copy length")? as u32,
                                        distance: int(&x[2], "copy distance")? as u32,
                                    }),
                                    _ => fail("invalid palette token"),
                                }
                            })
                            .collect::<Result<Vec<_>>>()?,
                    ))
                }
                _ => fail("invalid palette token group"),
            }
        })
        .collect()
}
fn array_value(value: &Value) -> Result<&Vec<Value>> {
    value
        .as_array()
        .ok_or_else(|| Error("invalid token list".into()))
}

fn encode_frame(pixels: &[u8], plan: &Value, preceding: &[u8]) -> Result<Vec<u8>> {
    let codec = required(plan, "codec")?
        .as_str()
        .ok_or_else(|| Error("unsupported static-sprite plan".into()))?;
    let expected = int(required(plan, "encoded_bytes")?, "encoded_bytes")? as usize;
    let frame = match codec {
        "golden-sun-static-sprite-mode0" => zero_encode(pixels)?,
        "golden-sun-static-sprite-mode1" => {
            let compression = required(plan, "compression")?;
            let kind = int(required(compression, "kind")?, "kind")?;
            if kind == 0 {
                let tokens = parse_general(required(compression, "tokens")?)?;
                extract_resource::encode_general(pixels, &tokens).map_err(|e| Error(e.0))?
            } else {
                let groups = parse_palette(required(compression, "groups")?)?;
                let mut out = vec![1];
                out.extend(
                    extract_resource::encode_palette(pixels, &groups).map_err(|e| Error(e.0))?,
                );
                out
            }
        }
        "golden-sun-static-sprite-mode3" => {
            let stream = zero_encode(pixels)?;
            if stream.len() != int(required(plan, "stream_bytes")?, "stream_bytes")? as usize {
                return fail("static-sprite stream size differs from plan");
            }
            mode3_encode(&stream, required(plan, "wrapper")?, preceding)?
        }
        _ => return fail("unsupported static-sprite plan"),
    };
    if frame.len() != expected {
        return fail(format!(
            "{codec} encoded size differs from plan (got={} expected={expected})",
            frame.len()
        ));
    }
    Ok(frame)
}

fn resolve_plan(root: &Path, plan: &str) -> PathBuf {
    let flat = plan.replace('/', "_");
    for family in ["battle_characters_", "field_characters_"] {
        let p = root.join(format!("{family}{flat}"));
        if p.exists() {
            return p;
        }
    }
    let local = root.join(&flat);
    if local.exists() {
        local
    } else {
        root.join(plan)
    }
}
fn frame_path(plan_path: &Path, index: usize) -> PathBuf {
    let text = plan_path.to_string_lossy();
    PathBuf::from(format!(
        "{}koma_{index:03}.png",
        text.strip_suffix("bank.json").unwrap_or(&text)
    ))
}
fn read_frames(
    root: &Path,
    item: &Value,
    plan_path: &Path,
    plan: &Value,
    colors: &[Rgb],
) -> Result<Vec<Vec<u8>>> {
    let width = int(required(plan, "width")?, "width")? as usize;
    let height = int(required(plan, "height")?, "height")? as usize;
    let count = array(plan, "frames")?.len();
    if plan.get("atlases").is_none() && plan.get("atlas_columns").is_none() {
        return (0..count)
            .map(|i| {
                let data = fs::read(frame_path(plan_path, i)).map_err(|e| Error(e.to_string()))?;
                let image = indexed_png(&data).map_err(|e| Error(e.0))?;
                if image.width as usize != width
                    || image.height as usize != height
                    || image.palette != colors
                {
                    return fail("sprite frame image differs from its plan");
                }
                Ok(image.pixels.into_iter().map(|p| p as u8).collect())
            })
            .collect();
    }
    let source = item
        .get("source")
        .and_then(Value::as_str)
        .ok_or_else(|| Error("sprite package lacks its atlas source".into()))?;
    let columns = int(required(plan, "atlas_columns")?, "atlas_columns")? as usize;
    atlas_frames(
        &fs::read(root.join(source)).map_err(|e| Error(e.to_string()))?,
        width,
        height,
        count,
        columns,
        colors,
    )
}

pub fn build_series(index: &Value, index_path: &Path, palette_path: &Path) -> Result<Vec<u8>> {
    if index.get("format") != Some(&json!(1))
        || index.get("codec").and_then(Value::as_str) != Some("golden-sun-static-sprite-series")
    {
        return fail("unsupported static-sprite series index");
    }
    let base = int(required(index, "address")?, "address")?;
    let size = int(required(index, "size")?, "size")? as usize;
    let prefix = int(required(index, "prefix_zeros")?, "prefix_zeros")? as usize;
    let suffix = int(
        index.get("suffix_zeros").unwrap_or(&json!(0)),
        "suffix_zeros",
    )? as usize;
    let colors = palette(
        palette_path,
        int(required(index, "palette_offset")?, "palette_offset")?,
        int(required(index, "palette_entries")?, "palette_entries")?,
    )?;
    let root = index_path.parent().unwrap_or(Path::new("."));
    let mut result = vec![0; prefix];
    let packages = array(index, "packages")?;
    for item in packages {
        let id = item.get("id").and_then(Value::as_str).unwrap_or("?");
        let address = int(required(item, "address")?, "address")?;
        let end = address + int(required(item, "size")?, "package size")?;
        if base + result.len() as i64 != address {
            return fail(format!("sprite package {id} is not contiguous"));
        }
        let plan_path = resolve_plan(root, item["plan"].as_str().unwrap());
        let plan = read_json(&plan_path)?;
        let mode = int(plan.get("mode").unwrap_or(&json!(3)), "mode")?;
        if let Some(index_mode) = item.get("mode") {
            if int(index_mode, "mode")? != mode {
                return fail("sprite package mode differs from its index");
            }
        }
        let frames = read_frames(root, item, &plan_path, &plan, &colors)?;
        let frame_plans = array(&plan, "frames")?;
        let mut offsets = Vec::new();
        for (i, frame) in frames.iter().enumerate() {
            offsets.push(result.len());
            let encoded = encode_frame(frame, &frame_plans[i], &result)
                .map_err(|e| Error(format!("{id} frame {i}: {}", e.0)))?;
            result.extend_from_slice(&encoded);
        }
        let alignment = int(
            plan.get("alignment_zeros").unwrap_or(&json!(0)),
            "alignment",
        )? as usize;
        result.extend(std::iter::repeat_n(0, alignment));
        if base + result.len() as i64 != int(required(item, "directory")?, "directory")? {
            return fail(format!("{id} stream extent differs from its index"));
        }
        let directory = array(&plan, "directory")?;
        for entry in directory {
            let selected = int(entry, "directory entry")? as usize;
            if selected >= offsets.len() {
                return fail(format!("{id} has an invalid alias"));
            }
            let ptr = base + offsets[selected] as i64;
            result.extend_from_slice(&(ptr as u32).to_le_bytes());
        }
        result.extend_from_slice(&[0, 0, 0, 0]);
        if base + result.len() as i64 != end {
            return fail(format!("{id} directory extent differs from its index"));
        }
    }
    result.extend(std::iter::repeat_n(0, suffix));
    if result.len() != size {
        return fail("static-sprite series size differs");
    } else if suffix > result.len() || result[result.len() - suffix..].iter().any(|b| *b != 0) {
        return fail("static-sprite suffix alignment is not zero");
    }
    Ok(result)
}

pub fn self_test() -> Result<()> {
    let pixels = vec![1, 0, 0, 0, 2, 0, 0];
    let encoded = zero_encode(&pixels)?;
    let (decoded, n) = zero_decode(&encoded, 0, MAX_PIXELS)?;
    if decoded != pixels || n != encoded.len() {
        return fail("zero-skip self-test failed");
    }
    let preceding = vec![0x55; 96];
    let wrapper = json!({"split":0});
    let stream = zero_encode(&pixels)?;
    let frame = mode3_encode(&stream, &wrapper, &preceding)?;
    let mut arena = preceding.clone();
    arena.extend_from_slice(&frame);
    let (decoded, n, _) = mode3_decode(&arena, preceding.len())?;
    if decoded != stream || n != frame.len() {
        return fail("mode-3 self-test failed");
    }
    println!("self-test=ok");
    Ok(())
}

#[test]
fn codec_round_trip() -> Result<()> {
    self_test()
}
