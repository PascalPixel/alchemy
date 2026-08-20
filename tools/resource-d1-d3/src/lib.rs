use alignment_tail::{build_alignment_tail, parse_alignment_tail, AlignmentTail};
use export_asset::{palette_rgba_image, tile_png};
use extract_resource::{encode_palette, PaletteGroup, PaletteOperation};
use import_asset::{gba_graphics, gba_palette_rgba, indexed_png};
use serde_json::{Map, Value};
use std::fs;
use std::io::{self, Write};
use std::path::{Path, PathBuf};

pub const D1_D3_ADDRESS: u32 = 0x0841_01d0;
pub const D1_D3_END: u32 = 0x0841_1474;
pub const D1_D3_BOUNDARY_SIZE: usize = (D1_D3_END - D1_D3_ADDRESS) as usize;

const D1_PALETTE_SIZE: usize = 0x80;
const D1_DECODED_SIZE: usize = 0x2a00;
const D1_COLUMNS: f64 = 14.0;
const D1_WIDTH: u32 = 112;
const D1_HEIGHT: u32 = 96;

#[derive(Debug, Clone, Copy)]
struct Spec {
    id: u32,
    address: u32,
    boundary_size: usize,
    source_size: usize,
    source: &'static str,
}

const D1: Spec = Spec {
    id: 0x0d1,
    address: 0x0841_01d0,
    boundary_size: 0x10a8,
    source_size: 0x10a5,
    source: "stream.json",
};
const D2: Spec = Spec {
    id: 0x0d2,
    address: 0x0841_1278,
    boundary_size: 0x01a8,
    source_size: 0x01a8,
    source: "idou_d2.json",
};
const D3: Spec = Spec {
    id: 0x0d3,
    address: 0x0841_1420,
    boundary_size: 0x0054,
    source_size: 0x0054,
    source: "idou_d3.json",
};
const SPECS: [Spec; 3] = [D1, D2, D3];

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Error(pub String);

impl std::fmt::Display for Error {
    fn fmt(&self, output: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        output.write_str(&self.0)
    }
}

fn err(message: impl Into<String>) -> Error {
    Error(message.into())
}

#[derive(Debug, Clone)]
pub struct BuiltResource {
    pub id: u32,
    pub address: u32,
    pub boundary_size: usize,
    pub data: Vec<u8>,
    pub sources: Vec<PathBuf>,
}

#[derive(Debug, Clone)]
enum Suffix {
    Fallback,
    Tail(AlignmentTail),
}

#[derive(Debug, Clone)]
struct StreamPlan {
    tokens: Vec<PaletteGroup>,
    suffix: Suffix,
}

#[derive(Debug, Clone)]
struct MotionPlan {
    initial: [i16; 2],
    deltas: Vec<[i8; 2]>,
    boundary_deltas: Vec<[i8; 2]>,
}

fn read(path: &Path) -> Result<Vec<u8>, Error> {
    fs::read(path).map_err(|error| err(format!("{}: {error}", path.display())))
}

fn object<'a>(value: &'a Value, label: &str) -> Result<&'a Map<String, Value>, Error> {
    value
        .as_object()
        .ok_or_else(|| err(format!("{label} must be an object")))
}

fn exact_keys(object: &Map<String, Value>, expected: &[&str], label: &str) -> Result<(), Error> {
    let mut actual: Vec<&str> = object.keys().map(String::as_str).collect();
    let mut wanted = expected.to_vec();
    actual.sort_unstable();
    wanted.sort_unstable();
    if actual != wanted {
        return Err(err(format!("{label} has unexpected fields")));
    }
    Ok(())
}

fn field<'a>(object: &'a Map<String, Value>, name: &str) -> Result<&'a Value, Error> {
    object
        .get(name)
        .ok_or_else(|| err(format!("missing field {name}")))
}

fn string<'a>(object: &'a Map<String, Value>, name: &str) -> Result<&'a str, Error> {
    field(object, name)?
        .as_str()
        .ok_or_else(|| err(format!("{name} must be a string")))
}

fn number(value: &Value, label: &str) -> Result<i64, Error> {
    let Some(value) = value.as_f64() else {
        return Err(err(format!("{label} must be an integer")));
    };
    if !value.is_finite()
        || value.fract() != 0.0
        || value < i64::MIN as f64
        || value > i64::MAX as f64
    {
        return Err(err(format!("{label} must be an integer")));
    }
    Ok(value as i64)
}

fn hex(value: u32, width: usize) -> String {
    format!("0x{value:0width$x}")
}

fn id_text(value: u32) -> String {
    hex(value, 3)
}

fn json_document(path: &Path, label: &str) -> Result<Value, Error> {
    let text =
        fs::read_to_string(path).map_err(|error| err(format!("{}: {error}", path.display())))?;
    let value: Value =
        serde_json::from_str(&text).map_err(|error| err(format!("{label}: {error}")))?;
    Ok(value)
}

fn child(root: &Path, name: &str) -> Result<PathBuf, Error> {
    let root = fs::canonicalize(root).map_err(|error| err(error.to_string()))?;
    let path = fs::canonicalize(root.join(name)).map_err(|error| err(error.to_string()))?;
    if path.strip_prefix(&root).is_err() {
        return Err(err("D1-D3 source escaped its package"));
    }
    Ok(path)
}

fn safe_integer(value: &Value, label: &str) -> Result<i64, Error> {
    number(value, label)
}

fn parse_palette_tokens(value: &Value, decoded_size: usize) -> Result<Vec<PaletteGroup>, Error> {
    let groups = value
        .as_array()
        .ok_or_else(|| err("D1 palette tokens must be an array"))?;
    let mut output = 0usize;
    let mut ended = false;
    let mut result = Vec::with_capacity(groups.len());
    for (group_position, group_value) in groups.iter().enumerate() {
        if ended {
            return Err(err(format!("palette group {group_position} is invalid")));
        }
        let group = group_value
            .as_array()
            .ok_or_else(|| err(format!("palette group {group_position} is invalid")))?;
        match group.first().and_then(Value::as_str) {
            Some("z") => {
                if group.len() != 1 || output + 8 > decoded_size {
                    return Err(err(format!("palette group {group_position} is invalid")));
                }
                output += 8;
                result.push(PaletteGroup::Zeros);
            }
            Some("g") => {
                if group.len() != 2 {
                    return Err(err(format!("palette group {group_position} is invalid")));
                }
                let operations = group[1]
                    .as_array()
                    .ok_or_else(|| err(format!("palette group {group_position} is invalid")))?;
                if operations.is_empty() || operations.len() > 8 {
                    return Err(err(format!("palette group {group_position} is invalid")));
                }
                let terminal = operations
                    .last()
                    .and_then(Value::as_array)
                    .and_then(|token| token.first())
                    .and_then(Value::as_str)
                    == Some("e");
                let has_copy_or_end = operations.iter().any(|token| {
                    token
                        .as_array()
                        .and_then(|token| token.first())
                        .and_then(Value::as_str)
                        .is_some_and(|opcode| opcode == "c" || opcode == "e")
                });
                if (!terminal && operations.len() != 8) || !has_copy_or_end {
                    return Err(err(format!(
                        "palette group {group_position} is not canonical"
                    )));
                }
                let mut parsed = Vec::with_capacity(operations.len());
                for (token_position, token_value) in operations.iter().enumerate() {
                    let token = token_value.as_array().ok_or_else(|| {
                        err(format!(
                            "palette token {group_position}:{token_position} is invalid"
                        ))
                    })?;
                    match token.first().and_then(Value::as_str) {
                        Some("l") if token.len() == 1 => {
                            if output >= decoded_size {
                                return Err(err(format!(
                                    "palette token {group_position}:{token_position} is invalid"
                                )));
                            }
                            output += 1;
                            parsed.push(PaletteOperation::Literal);
                        }
                        Some("e") if token.len() == 1 && token_position + 1 == operations.len() => {
                            ended = true;
                            parsed.push(PaletteOperation::End);
                        }
                        Some("c") if token.len() == 3 => {
                            let length = safe_integer(
                                &token[1],
                                &format!("palette token {group_position}:{token_position} length"),
                            )?;
                            let distance = safe_integer(
                                &token[2],
                                &format!(
                                    "palette token {group_position}:{token_position} distance"
                                ),
                            )?;
                            if !(2..=272).contains(&length)
                                || !(1..=output as i64).contains(&distance)
                                || output + length as usize > decoded_size
                            {
                                return Err(err(format!(
                                    "palette token {group_position}:{token_position} crossed its replay bounds"
                                )));
                            }
                            output += length as usize;
                            parsed.push(PaletteOperation::Copy {
                                length: length as u32,
                                distance: distance as u32,
                            });
                        }
                        _ => {
                            return Err(err(format!(
                                "palette token {group_position}:{token_position} is invalid"
                            )))
                        }
                    }
                }
                result.push(PaletteGroup::Group(parsed));
            }
            _ => return Err(err(format!("palette group {group_position} is invalid"))),
        }
    }
    if !ended || output != decoded_size {
        return Err(err("palette tokens do not cover the decoded background"));
    }
    Ok(result)
}

fn parse_index(value: &Value) -> Result<Vec<String>, Error> {
    let index = object(value, "D1-D3 index")?;
    exact_keys(
        index,
        &[
            "format",
            "kind",
            "address",
            "end",
            "boundary_size",
            "resources",
        ],
        "D1-D3 index",
    )?;
    if number(field(index, "format")?, "format")? != 1
        || string(index, "kind")? != "golden-sun-d1-d3-package"
        || string(index, "address")? != hex(D1_D3_ADDRESS, 8)
        || string(index, "end")? != hex(D1_D3_END, 8)
        || string(index, "boundary_size")? != hex(D1_D3_BOUNDARY_SIZE as u32, 4)
    {
        return Err(err("unsupported D1-D3 index"));
    }
    let resources = field(index, "resources")?
        .as_array()
        .ok_or_else(|| err("unsupported D1-D3 index"))?;
    if resources.len() != SPECS.len() {
        return Err(err("unsupported D1-D3 index"));
    }
    let mut sources = Vec::with_capacity(resources.len());
    for (position, entry_value) in resources.iter().enumerate() {
        let entry = object(entry_value, "D1-D3 index entry")?;
        exact_keys(
            entry,
            &[
                "id",
                "address",
                "resource_boundary_size",
                "source_size",
                "source",
            ],
            "D1-D3 index entry",
        )?;
        let spec = SPECS[position];
        if string(entry, "id")? != id_text(spec.id)
            || string(entry, "address")? != hex(spec.address, 8)
            || string(entry, "resource_boundary_size")? != hex(spec.boundary_size as u32, 4)
            || string(entry, "source_size")? != hex(spec.source_size as u32, 4)
            || string(entry, "source")? != spec.source
        {
            return Err(err(
                "D1-D3 index entry differs from the audited resource directory",
            ));
        }
        sources.push(string(entry, "source")?.to_string());
    }
    Ok(sources)
}

fn parse_stream(value: &Value) -> Result<StreamPlan, Error> {
    let plan = object(value, "D1 stream plan")?;
    exact_keys(
        plan,
        &[
            "format",
            "kind",
            "resource_id",
            "address",
            "resource_boundary_size",
            "source_size",
            "consumer",
            "palette",
            "stream",
            "image",
            "suffix",
        ],
        "D1 stream plan",
    )?;
    if number(field(plan, "format")?, "format")? != 1
        || string(plan, "kind")? != "golden-sun-d1-background"
        || string(plan, "resource_id")? != "0x0d1"
        || string(plan, "address")? != hex(D1.address, 8)
        || string(plan, "resource_boundary_size")? != hex(D1.boundary_size as u32, 4)
        || string(plan, "source_size")? != hex(D1.source_size as u32, 4)
    {
        return Err(err("unsupported D1 stream plan"));
    }
    let consumer = object(field(plan, "consumer")?, "D1 consumer")?;
    exact_keys(
        consumer,
        &[
            "function",
            "loader",
            "decompressor",
            "palette_bytes",
            "graphics_bpp",
        ],
        "D1 consumer",
    )?;
    if string(consumer, "function")? != "Func_080d41a4"
        || string(consumer, "loader")? != "Func_080e0524"
        || string(consumer, "decompressor")? != "Func_08005340"
        || string(consumer, "palette_bytes")? != hex(D1_PALETTE_SIZE as u32, 2)
        || number(field(consumer, "graphics_bpp")?, "graphics_bpp")? != 8
    {
        return Err(err("D1 consumer framing differs from reconstructed code"));
    }
    let palette = object(field(plan, "palette")?, "D1 palette")?;
    exact_keys(palette, &["source", "size"], "D1 palette")?;
    if string(palette, "source")? != "iro.rgba.png"
        || string(palette, "size")? != hex(D1_PALETTE_SIZE as u32, 2)
    {
        return Err(err("D1 palette framing differs"));
    }
    let stream = object(field(plan, "stream")?, "D1 stream")?;
    exact_keys(
        stream,
        &["codec", "tag", "decoded_size", "tokens"],
        "D1 stream",
    )?;
    if string(stream, "codec")? != "palette-lz"
        || number(field(stream, "tag")?, "tag")? != 1
        || string(stream, "decoded_size")? != hex(D1_DECODED_SIZE as u32, 4)
    {
        return Err(err("D1 compression framing differs"));
    }
    let tokens = parse_palette_tokens(field(stream, "tokens")?, D1_DECODED_SIZE)?;
    let suffix = object(field(plan, "suffix")?, "D1 suffix")?;
    let suffix = if suffix.contains_key("policy") {
        exact_keys(suffix, &["size", "policy"], "D1 suffix")?;
        if number(field(suffix, "size")?, "D1 suffix size")? != 3
            || string(suffix, "policy")? != "fallback"
        {
            return Err(err("D1 suffix policy differs"));
        }
        Suffix::Fallback
    } else {
        Suffix::Tail(
            parse_alignment_tail(field(plan, "suffix")?, 3, 3, "D1 suffix")
                .map_err(|error| err(error.0))?,
        )
    };
    let image = object(field(plan, "image")?, "D1 image")?;
    exact_keys(
        image,
        &["source", "encoding", "width", "height", "columns"],
        "D1 image",
    )?;
    if string(image, "source")? != "haikei.8bpp.png"
        || string(image, "encoding")? != "gba-8bpp-linear-tiles"
        || number(field(image, "width")?, "width")? != D1_WIDTH as i64
        || number(field(image, "height")?, "height")? != D1_HEIGHT as i64
        || number(field(image, "columns")?, "columns")? != D1_COLUMNS as i64
    {
        return Err(err("D1 image framing differs"));
    }
    Ok(StreamPlan { tokens, suffix })
}

fn motion_shape(spec: Spec) -> (&'static str, usize, usize, usize, usize, usize) {
    match spec.id {
        0x0d2 => ("Func_080dc968", 0, 1, 209, 209, 1),
        0x0d3 => ("Func_080e89ec", 18, 19, 58, 40, 0),
        _ => panic!("resource is not a motion path"),
    }
}

fn coordinate(value: &Value, label: &str, minimum: i64, maximum: i64) -> Result<i64, Error> {
    let value = safe_integer(value, label)?;
    if value < minimum || value > maximum {
        return Err(err(format!("{label} is outside its encoded range")));
    }
    Ok(value)
}

fn pair(value: &Value, label: &str, minimum: i64, maximum: i64) -> Result<[i64; 2], Error> {
    let pair = value
        .as_array()
        .ok_or_else(|| err(format!("{label} must be a coordinate pair")))?;
    if pair.len() != 2 {
        return Err(err(format!("{label} must be a coordinate pair")));
    }
    Ok([
        coordinate(&pair[0], &format!("{label} X"), minimum, maximum)?,
        coordinate(&pair[1], &format!("{label} Y"), minimum, maximum)?,
    ])
}

fn parse_motion(value: &Value, spec: Spec) -> Result<MotionPlan, Error> {
    let plan = object(value, "motion path")?;
    exact_keys(
        plan,
        &[
            "format",
            "kind",
            "resource_id",
            "address",
            "size",
            "consumer",
            "initial",
            "deltas",
            "boundary_deltas",
        ],
        "motion path",
    )?;
    if number(field(plan, "format")?, "format")? != 1
        || string(plan, "kind")? != "golden-sun-motion-path"
        || string(plan, "resource_id")? != id_text(spec.id)
        || string(plan, "address")? != hex(spec.address, 8)
        || string(plan, "size")? != hex(spec.source_size as u32, 4)
    {
        return Err(err("unsupported motion path"));
    }
    let consumer = object(field(plan, "consumer")?, "motion-path consumer")?;
    exact_keys(
        consumer,
        &[
            "function",
            "initial_frame",
            "delta_first_frame",
            "delta_last_frame",
            "initial_encoding",
            "delta_encoding",
            "boundary_delta_pairs",
        ],
        "motion-path consumer",
    )?;
    let (function, initial_frame, first_frame, last_frame, delta_count, boundary_count) =
        motion_shape(spec);
    if string(consumer, "function")? != function
        || number(field(consumer, "initial_frame")?, "initial_frame")? != initial_frame as i64
        || number(field(consumer, "delta_first_frame")?, "delta_first_frame")? != first_frame as i64
        || number(field(consumer, "delta_last_frame")?, "delta_last_frame")? != last_frame as i64
        || string(consumer, "initial_encoding")? != "be-s16-pair"
        || string(consumer, "delta_encoding")? != "s8-pair"
        || number(
            field(consumer, "boundary_delta_pairs")?,
            "boundary_delta_pairs",
        )? != boundary_count as i64
    {
        return Err(err(
            "motion-path consumer shape differs from reconstructed code",
        ));
    }
    let initial = pair(
        field(plan, "initial")?,
        "motion-path initial coordinate",
        -0x8000,
        0x7fff,
    )?;
    let delta_values = field(plan, "deltas")?
        .as_array()
        .ok_or_else(|| err("motion-path record count differs from reconstructed code"))?;
    let boundary_values = field(plan, "boundary_deltas")?
        .as_array()
        .ok_or_else(|| err("motion-path record count differs from reconstructed code"))?;
    if delta_values.len() != delta_count || boundary_values.len() != boundary_count {
        return Err(err(
            "motion-path record count differs from reconstructed code",
        ));
    }
    let parse_pairs = |values: &[Value], label: &str| -> Result<Vec<[i8; 2]>, Error> {
        values
            .iter()
            .enumerate()
            .map(|(index, value)| {
                let pair = pair(value, &format!("{label} {index}"), -0x80, 0x7f)?;
                Ok([pair[0] as i8, pair[1] as i8])
            })
            .collect()
    };
    let deltas = parse_pairs(delta_values, "motion-path delta")?;
    let boundary_deltas = parse_pairs(boundary_values, "motion-path boundary delta")?;
    if 4 + (deltas.len() + boundary_deltas.len()) * 2 != spec.source_size {
        return Err(err(
            "motion-path records do not fill their resource boundary",
        ));
    }
    Ok(MotionPlan {
        initial: [initial[0] as i16, initial[1] as i16],
        deltas,
        boundary_deltas,
    })
}

fn source_prefix(path: &Path, suffix: &str) -> Result<String, Error> {
    let name = path
        .file_name()
        .and_then(|name| name.to_str())
        .ok_or_else(|| err("D1-D3 source path must name a file"))?;
    name.strip_suffix(suffix)
        .map(str::to_owned)
        .ok_or_else(|| err("D1-D3 source path must name a known file"))
}

fn build_stream(path: &Path) -> Result<(Vec<u8>, Vec<PathBuf>), Error> {
    let plan = parse_stream(&json_document(path, "D1 stream plan")?)?;
    let root = path
        .parent()
        .ok_or_else(|| err("D1 stream plan has no parent"))?;
    let prefix = source_prefix(path, "stream.json")?;
    let palette_path = child(root, &format!("{prefix}iro.rgba.png"))?;
    let image_path = child(root, &format!("{prefix}haikei.8bpp.png"))?;
    let palette_image = read(&palette_path)?;
    let (palette, _) = gba_palette_rgba(&palette_image).map_err(|error| err(error.0))?;
    if palette.len() != D1_PALETTE_SIZE {
        return Err(err("D1 palette has the wrong size"));
    }
    let (canonical_palette, _) =
        palette_rgba_image(&palette, 16.0).map_err(|error| err(error.0))?;
    if palette_image != canonical_palette {
        return Err(err("D1 palette image is not canonical"));
    }
    let image = read(&image_path)?;
    let (decoded, image_palette, report) =
        gba_graphics(&image, 8.0).map_err(|error| err(error.0))?;
    if decoded.len() != D1_DECODED_SIZE
        || report.get("width") != Some(D1_WIDTH as f64)
        || report.get("height") != Some(D1_HEIGHT as f64)
        || report.get("palette_entries") != Some((D1_PALETTE_SIZE / 2) as f64)
        || image_palette != palette
    {
        return Err(err(
            "D1 background image differs from its consumer-framed layout",
        ));
    }
    let indexed = indexed_png(&image).map_err(|error| err(error.0))?;
    let (canonical_image, _) = tile_png(&decoded, 8.0, D1_COLUMNS, Some(&indexed.palette))
        .map_err(|error| err(error.0))?;
    if image != canonical_image {
        return Err(err("D1 background image is not canonical"));
    }
    let encoded_tail = encode_palette(&decoded, &plan.tokens).map_err(|error| err(error.0))?;
    let mut data = Vec::with_capacity(D1.boundary_size);
    data.extend_from_slice(&palette);
    data.push(1);
    data.extend_from_slice(&encoded_tail);
    if data.len() != D1.source_size {
        return Err(err("D1 source differs from its audited encoded size"));
    }
    if let Suffix::Tail(tail) = plan.suffix {
        data.extend_from_slice(&build_alignment_tail(&tail));
    }
    if data.len() != D1.source_size && data.len() != D1.boundary_size {
        return Err(err("D1 built size differs"));
    }
    Ok((data, vec![path.to_path_buf(), palette_path, image_path]))
}

fn build_motion(path: &Path, spec: Spec) -> Result<Vec<u8>, Error> {
    let plan = parse_motion(
        &json_document(path, &format!("{} motion path", id_text(spec.id)))?,
        spec,
    )?;
    let mut output = vec![0u8; spec.source_size];
    output[0..2].copy_from_slice(&plan.initial[0].to_be_bytes());
    output[2..4].copy_from_slice(&plan.initial[1].to_be_bytes());
    let mut cursor = 4;
    for [x, y] in plan.deltas.iter().chain(plan.boundary_deltas.iter()) {
        output[cursor] = *x as u8;
        output[cursor + 1] = *y as u8;
        cursor += 2;
    }
    if cursor != output.len() {
        return Err(err("motion-path encoder did not fill its resource"));
    }
    Ok(output)
}

pub fn build_resource_d1_d3(index_path: &Path) -> Result<Vec<BuiltResource>, Error> {
    let sources = parse_index(&json_document(index_path, "D1-D3 index")?)?;
    let root = index_path
        .parent()
        .ok_or_else(|| err("D1-D3 index has no parent"))?;
    let prefix = source_prefix(index_path, "index.json")?;
    SPECS
        .iter()
        .enumerate()
        .map(|(position, spec)| {
            let source = child(root, &format!("{prefix}{}", sources[position]))?;
            let (data, nested) = if spec.id == D1.id {
                build_stream(&source)?
            } else {
                (build_motion(&source, *spec)?, vec![source.clone()])
            };
            if data.len() != spec.source_size && data.len() != spec.boundary_size {
                return Err(err(format!("{} built size differs", id_text(spec.id))));
            }
            let mut provenance = vec![index_path.to_path_buf()];
            provenance.extend(nested);
            Ok(BuiltResource {
                id: spec.id,
                address: spec.address,
                boundary_size: spec.boundary_size,
                data,
                sources: provenance,
            })
        })
        .collect()
}

pub fn run(args: Vec<String>) -> Result<(), Error> {
    if args.first().map(String::as_str) != Some("build-stdout") {
        return Err(err("usage: resource-d1-d3 build-stdout INDEX RESOURCE_ID"));
    }
    let index = args
        .get(1)
        .ok_or_else(|| err("build-stdout requires an index"))?;
    let id_text = args
        .get(2)
        .ok_or_else(|| err("build-stdout requires a resource id"))?;
    let digits = id_text.strip_prefix("0x").unwrap_or(id_text);
    let id = u32::from_str_radix(digits, 16).map_err(|_| err("invalid resource id"))?;
    let resource = build_resource_d1_d3(Path::new(index))?
        .into_iter()
        .find(|resource| resource.id == id)
        .ok_or_else(|| err(format!("resource {id_text} is absent")))?;
    io::stdout()
        .write_all(&resource.data)
        .map_err(|e| err(e.to_string()))
}
