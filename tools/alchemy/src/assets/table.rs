//! Integer arrays, radix record tables and typed tables from editable JSON.
//!
//! A typed table is a list of segments laid out one after another from the
//! table's own label. Values that name a segment are offsets from the table's
//! start when the field is narrower than a word, and linked addresses of the
//! segment's label when it is a word; `pointer` and `thumb-pointer` fields
//! name any other linked symbol. No table records or receives an address.
use super::asm::{identifier, Data};
use super::graphics::{glyph_rows, indices, Atlas};
use psynergy::assets::image::IndexedImage;
use serde_json::Value;
use std::collections::{BTreeMap, HashMap, HashSet};

fn number(value: &Value, label: &str) -> Result<usize, String> {
    let text = match value {
        Value::Number(value) => value.to_string(),
        Value::String(value) => value.clone(),
        _ => return Err(format!("{label} must be an integer")),
    };
    let parsed = match text.strip_prefix("0x").or_else(|| text.strip_prefix("0X")) {
        Some(hex) => usize::from_str_radix(hex, 16),
        None => text.parse::<usize>(),
    };
    parsed.map_err(|_| format!("{label} must be an integer"))
}

fn string<'a>(value: &'a Value, label: &str) -> Result<&'a str, String> {
    value
        .as_str()
        .ok_or_else(|| format!("{label} must be a string"))
}

/// An integer array member: a JSON integer, or hexadecimal text such as
/// `"0x3c"` for values that read better in the radix their consumer uses.
fn array_member(value: &Value) -> Result<i64, String> {
    if value.is_string() {
        return i64::try_from(number(value, "array member")?)
            .map_err(|_| "array member exceeds i64".into());
    }
    value
        .as_i64()
        .ok_or("array member is not an integer".into())
}

fn encode_integer(value: i64, kind: &str) -> Result<Vec<u8>, String> {
    let out_of_range = || format!("array member {value} exceeds {kind}");
    Ok(match kind {
        "u8-array" => vec![u8::try_from(value).map_err(|_| out_of_range())?],
        "s8-array" => vec![i8::try_from(value).map_err(|_| out_of_range())? as u8],
        "le-u16-array" => u16::try_from(value)
            .map_err(|_| out_of_range())?
            .to_le_bytes()
            .to_vec(),
        "le-s16-array" => i16::try_from(value)
            .map_err(|_| out_of_range())?
            .to_le_bytes()
            .to_vec(),
        "be-s16-array" => i16::try_from(value)
            .map_err(|_| out_of_range())?
            .to_be_bytes()
            .to_vec(),
        "le-u32-array" => u32::try_from(value)
            .map_err(|_| out_of_range())?
            .to_le_bytes()
            .to_vec(),
        "le-s32-array" => i32::try_from(value)
            .map_err(|_| out_of_range())?
            .to_le_bytes()
            .to_vec(),
        _ => return Err(format!("unknown integer encoding {kind}")),
    })
}

/// A (possibly nested) JSON array of integers in one element encoding:
/// `u8-array`, `s8-array`, `le-u16-array`, `le-s16-array`, `be-s16-array`,
/// `le-u32-array` or `le-s32-array`.
pub(crate) fn integer_array(value: &Value, kind: &str) -> Result<Vec<u8>, String> {
    let mut output = Vec::new();
    for value in value.as_array().ok_or("integer array is not an array")? {
        if value.is_array() {
            output.extend(integer_array(value, kind)?);
        } else {
            output.extend(encode_integer(array_member(value)?, kind)?);
        }
    }
    Ok(output)
}

/// A table of records whose fields are written as text in `radix`, less
/// `bias`, in the element encoding `element`; `index_field` numbers the
/// records from zero.
pub(crate) fn record_table(document: &Value) -> Result<Vec<u8>, String> {
    let fields = document["fields"]
        .as_array()
        .ok_or("record fields missing")?;
    if fields.is_empty() {
        return Err("empty record layout".into());
    }
    let kind = string(&document["element"], "record element")?;
    let bias = number(&document["bias"], "record bias")?;
    let radix = number(&document["radix"], "record radix")?;
    if !(2..=36).contains(&radix) {
        return Err("invalid record radix".into());
    }
    let index_field = string(&document["index_field"], "record index field")?;
    let mut values = Vec::new();
    for (i, record) in document["records"]
        .as_array()
        .ok_or("records missing")?
        .iter()
        .enumerate()
    {
        if number(&record[index_field], "record index")? != i {
            return Err("record indices are not sequential".into());
        }
        for field in fields {
            let name = string(field, "record field")?;
            let value = u64::from_str_radix(string(&record[name], name)?, radix as u32)
                .map_err(|e| e.to_string())?;
            let value = value
                .checked_sub(bias as u64)
                .ok_or("record value is below bias")?;
            values.push(Value::from(value));
        }
    }
    integer_array(&Value::Array(values), kind)
}

/// One value of a typed table: an integer, or an address of `symbol +
/// addend`. `offset` is the address's offset from the table's start when the
/// symbol is one of its own segments.
#[derive(Clone, Debug, PartialEq, Eq)]
enum Item {
    Constant(i64),
    Address {
        symbol: String,
        addend: i64,
        offset: Option<usize>,
    },
}

/// The names a typed table's values may use: each segment's start and
/// stride, the table's label and its named constants.
struct Scope<'a> {
    label: &'a str,
    segments: BTreeMap<String, Option<(usize, usize)>>,
    names: Value,
}

impl Scope<'_> {
    fn segment_label(&self, name: &str) -> String {
        format!("{}_{name}", self.label)
    }
    /// A string value: a named constant from the field's or the table's
    /// `names`, a number, a segment `name` or `name[index]`, or, where
    /// `linked` allows it, `Symbol` or `Symbol+N` of any linked symbol.
    fn item(&self, text: &str, spec: &Value, linked: bool) -> Result<Item, String> {
        if let Some(named) = spec
            .get("names")
            .and_then(|names| names.get(text))
            .or_else(|| self.names.get(text))
        {
            return named
                .as_i64()
                .map(Item::Constant)
                .ok_or_else(|| format!("named value {text} is not an integer"));
        }
        if text.starts_with("0x") || text.bytes().all(|byte| byte.is_ascii_digit()) {
            return i64::try_from(number(&Value::from(text), "numeric value")?)
                .map(Item::Constant)
                .map_err(|_| format!("numeric value {text} overflows"));
        }
        let (name, index) = match text.strip_suffix(']').and_then(|text| text.split_once('[')) {
            Some((name, index)) => (name, number(&Value::from(index), "element index")?),
            None => (text, 0),
        };
        match self.segments.get(name) {
            Some(Some((start, stride))) => {
                let addend = index
                    .checked_mul(*stride)
                    .filter(|addend| i64::try_from(*addend).is_ok())
                    .ok_or_else(|| format!("element index overflows in {text}"))?;
                Ok(Item::Address {
                    symbol: self.segment_label(name),
                    addend: addend as i64,
                    offset: Some(start + addend),
                })
            }
            Some(None) => Err(format!("segment name {name} is ambiguous")),
            None if linked => {
                let (symbol, addend) = match text.split_once('+') {
                    Some((symbol, addend)) => (
                        symbol.trim(),
                        i64::try_from(number(&Value::from(addend.trim()), "symbol addend")?)
                            .map_err(|_| format!("addend of {text} overflows"))?,
                    ),
                    None => (text, 0),
                };
                if !identifier(symbol) {
                    return Err(format!("{symbol} is not a symbol name"));
                }
                Ok(Item::Address {
                    symbol: symbol.to_owned(),
                    addend,
                    offset: None,
                })
            }
            None => Err(format!("unknown table value {text}")),
        }
    }
}

/// The encoding, width and linking of a scalar element.
fn element(name: &str) -> Option<(&'static str, usize, bool)> {
    Some(match name {
        "u8" => ("u8-array", 1, false),
        "s8" => ("s8-array", 1, false),
        "le-u16" => ("le-u16-array", 2, false),
        "le-s16" => ("le-s16-array", 2, false),
        "le-u32" => ("le-u32-array", 4, false),
        "le-s32" => ("le-s32-array", 4, false),
        "pointer" | "thumb-pointer" => ("le-u32-array", 4, true),
        _ => return None,
    })
}

/// The values of one field or scalar segment, at `start` bytes into the table.
fn table_values(
    value: &Value,
    spec: &Value,
    max_bytes: usize,
    scope: &Scope,
) -> Result<Data, String> {
    let name = string(&spec["element"], "field element")?;
    let (kind, width, linked) = element(name).ok_or("unknown field element")?;
    fn flatten(
        value: &Value,
        spec: &Value,
        scope: &Scope,
        linked: bool,
        out: &mut Vec<Item>,
    ) -> Result<(), String> {
        match value {
            Value::Array(items) => {
                for item in items {
                    flatten(item, spec, scope, linked, out)?;
                }
            }
            Value::String(text) => match scope.item(text, spec, linked)? {
                Item::Constant(_) if linked => {
                    return Err(format!("pointer {text} names no symbol"))
                }
                item => out.push(item),
            },
            Value::Null if linked => out.push(Item::Constant(0)),
            _ if linked => return Err("a pointer names a symbol or is null".into()),
            _ => out.push(Item::Constant(
                array_member(value).map_err(|_| "field value is not an integer")?,
            )),
        }
        Ok(())
    }
    let mut items = Vec::new();
    if let Some(bits) = spec.get("bits") {
        let widths = bits
            .as_object()
            .ok_or("bit widths must map part names to widths")?
            .values()
            .map(|w| number(w, "bit width"))
            .collect::<Result<Vec<_>, _>>()?;
        if linked || widths.iter().sum::<usize>() != width * 8 || widths.contains(&0) {
            return Err("bit widths do not fill the element".into());
        }
        let groups = match value.as_array() {
            Some(items) if items.iter().all(Value::is_array) => items.iter().collect(),
            _ => vec![value],
        };
        for group in groups {
            let parts = group
                .as_array()
                .filter(|parts| parts.len() == widths.len())
                .ok_or("packed value has the wrong number of parts")?;
            let mut packed = 0i64;
            let mut shift = 0;
            for (part, &bits) in parts.iter().zip(&widths) {
                let part = part.as_i64().ok_or("packed part is not an integer")?;
                if part < 0 || part >= 1i64 << bits {
                    return Err("packed part exceeds its bit width".into());
                }
                packed |= part << shift;
                shift += bits;
            }
            items.push(Item::Constant(packed));
        }
    } else {
        flatten(value, spec, scope, linked, &mut items)?;
    }
    let constants: Vec<i64> = items
        .iter()
        .filter_map(|item| match item {
            Item::Constant(value) => Some(*value),
            Item::Address { .. } => None,
        })
        .collect();
    for key in ["min", "max"] {
        if let Some(limit) = spec.get(key) {
            let limit = limit.as_i64().ok_or("field bound is not an integer")?;
            if constants
                .iter()
                .any(|&v| if key == "min" { v < limit } else { v > limit })
            {
                return Err("field value outside bounds".into());
            }
        }
    }
    if let Some(unique) = spec.get("unique") {
        if unique.as_bool().ok_or("unique must be boolean")? {
            let mut sorted = constants.clone();
            sorted.sort_unstable();
            if sorted.windows(2).any(|v| v[0] == v[1]) {
                return Err("duplicate field value".into());
            }
        }
    }
    let mut data = Data::default();
    for item in &items {
        match item {
            Item::Constant(value) => data.bytes.extend(encode_integer(*value, kind)?),
            Item::Address { symbol, addend, .. } if width == 4 => data.pointer(symbol, *addend),
            Item::Address {
                offset: Some(offset),
                ..
            } => data.bytes.extend(encode_integer(*offset as i64, kind)?),
            Item::Address { symbol, .. } => {
                return Err(format!(
                    "a {width}-byte field cannot hold the address of {symbol}"
                ))
            }
        }
    }
    let count = items.len();
    if let Some(capacity) = spec.get("terminated_capacity") {
        let capacity = number(capacity, "terminated capacity")?;
        if spec.get("capacity").is_some()
            || capacity == 0
            || capacity > max_bytes / width
            || count >= capacity
            || constants.contains(&0)
        {
            return Err("terminated field has no room for terminator or contains zero".into());
        }
        data.bytes.resize(capacity * width, 0);
    } else if let Some(capacity) = spec.get("capacity") {
        let capacity = number(capacity, "capacity")?;
        if capacity == 0 || capacity > max_bytes / width || count > capacity {
            return Err("field exceeds its zero-padded capacity".into());
        }
        data.bytes.resize(capacity * width, 0);
    }
    Ok(data)
}

/// Values a typed-table segment computes from its formula instead of listing.
/// `ceiling-reciprocal` gives entry `i` the ceiling of 2^`numerator_bits` / `i`
/// reduced to the unsigned element width, the multiplier an unsigned divide by
/// `i` reads: a 32-bit numerator leaves entry 1 as 0, and entry 0 has no
/// reciprocal and holds 0.
fn generated_values(
    generator: &Value,
    kind: &str,
    width: usize,
    count: usize,
) -> Result<Vec<u8>, String> {
    if generator["formula"] != "ceiling-reciprocal" {
        return Err("unknown table generator".into());
    }
    if !matches!(kind, "u8-array" | "le-u16-array" | "le-u32-array") {
        return Err("reciprocals need an unsigned element".into());
    }
    let bits = number(&generator["numerator_bits"], "numerator bits")?;
    if bits == 0 || bits > 64 {
        return Err("reciprocal numerator exceeds 64 bits".into());
    }
    let numerator = 1u128 << bits;
    let modulus = 1u128 << (width * 8);
    let values = (0..count)
        .map(|i| match i {
            0 => Value::from(0),
            i => Value::from((numerator.div_ceil(i as u128) % modulus) as u64),
        })
        .collect();
    integer_array(&Value::Array(values), kind)
}

/// Replace a record segment's `1bpp-rows` fields with the packed rows of the
/// atlas frame each record names in the segment's `image`: rows of eight
/// pixels become `u8` values and rows of sixteen `le-u16` values.
fn resolve_glyphs(
    segment: &mut Value,
    images: &dyn Fn(&str) -> Result<IndexedImage, String>,
) -> Result<(), String> {
    let Some(image) = segment.get("image").cloned() else {
        return Ok(());
    };
    let decoded = images(string(&image["source"], "glyph source")?)?;
    let atlas = Atlas {
        frame_width: number(&image["frame_width"], "frame width")?,
        frame_height: number(&image["frame_height"], "frame height")?,
        columns: number(&image["columns"], "atlas columns")?,
    };
    let (width, height) = (decoded.width as usize, decoded.height as usize);
    let slots: Vec<usize> = (0..atlas.slots(width, height)?).collect();
    let frames = atlas.frames(&indices(&decoded), width, height, 1, &slots)?;
    let mut glyph_fields = Vec::new();
    for field in segment["fields"]
        .as_array_mut()
        .ok_or("record fields missing")?
    {
        if field["element"] != "1bpp-rows" {
            continue;
        }
        let rows = number(&field["rows"], "glyph rows")?;
        field["element"] = Value::from(match atlas.frame_width {
            8 => "u8",
            16 => "le-u16",
            _ => return Err("glyph rows must be 8 or 16 pixels wide".into()),
        });
        glyph_fields.push((string(&field["name"], "field name")?.to_string(), rows));
    }
    for record in segment["records"].as_array_mut().ok_or("records missing")? {
        for (name, rows) in &glyph_fields {
            let frame = frames
                .get(number(&record[name.as_str()], "glyph frame")?)
                .ok_or("glyph frame lies outside its atlas")?;
            record[name.as_str()] = Value::from(glyph_rows(frame, atlas.frame_width, *rows)?);
        }
    }
    Ok(())
}

/// A typed table labelled `label`, from its editable document: `segments`
/// of `size` bytes each, in order, every one an `element` array of `values`,
/// a `fill` byte, a `generator` formula, `record`s of named fields, fixed
/// `ascii-fixed` text, an `ascii-pool` of aligned strings, or `pool-pointer`
/// words addressing a pool's strings. A named segment is labelled
/// `label_name`. `images` opens the PNG a glyph segment names.
pub(crate) fn typed_table(
    label: &str,
    document: &Value,
    images: &dyn Fn(&str) -> Result<IndexedImage, String>,
) -> Result<Data, String> {
    if document["format"] != 2 || document["kind"] != "typed-table" {
        return Err("typed table identity differs".into());
    }
    if !identifier(label) {
        return Err(format!("{label} is not a symbol name"));
    }
    if let Some(key) = ["address", "base", "base_address"]
        .into_iter()
        .find(|key| document.get(*key).is_some())
    {
        return Err(format!(
            "a typed table records no {key}; the linker places it"
        ));
    }
    let mut segments = document["segments"]
        .as_array()
        .ok_or("table segments missing")?
        .clone();
    let mut scope = Scope {
        label,
        segments: BTreeMap::new(),
        names: document.get("names").cloned().unwrap_or(Value::Null),
    };
    let mut start = 0usize;
    let mut starts = Vec::with_capacity(segments.len());
    for segment in &segments {
        if segment.get("end").is_some() || segment.get("address").is_some() {
            return Err("a segment gives its size, never an address".into());
        }
        let size = number(&segment["size"], "segment size")?;
        if size == 0 {
            return Err("table segment is empty".into());
        }
        if let Some(name) = segment.get("name") {
            let name = string(name, "segment name")?;
            if !identifier(name) {
                return Err(format!("segment name {name} is not a symbol name"));
            }
            let stride = number(&segment["stride"], "segment stride")?;
            scope
                .segments
                .entry(name.to_string())
                .and_modify(|slot| *slot = None)
                .or_insert(Some((start, stride)));
        }
        starts.push(start);
        start += size;
    }
    let mut data = Data::default();
    data.label(label, true);
    let mut pools: HashMap<String, Vec<usize>> = HashMap::new();
    for (segment, start) in segments.iter_mut().zip(starts) {
        resolve_glyphs(segment, images)?;
        let size = number(&segment["size"], "segment size")?;
        let stride = number(&segment["stride"], "segment stride")?;
        let element_name = string(&segment["element"], "segment element")?;
        let (kind, width) = match element(element_name) {
            Some((kind, width, _)) => (kind, width),
            None => match element_name {
                "ascii-fixed" | "ascii-pool" | "record" => (element_name, 1),
                "pool-pointer" => (element_name, 4),
                _ => return Err(format!("unknown table element {element_name}")),
            },
        };
        if stride < width || stride % width != 0 || size % stride != 0 {
            return Err("table stride differs".into());
        }
        if let Some(name) = segment.get("name") {
            let name = string(name, "segment name")?;
            if scope.segments.get(name).is_some_and(Option::is_some) {
                data.label(&scope.segment_label(name), false);
            }
        }
        let field_width = segment["fields"].as_array().map_or(width, |fields| {
            fields
                .iter()
                .filter_map(|field| field["element"].as_str().and_then(element))
                .map(|(_, width, _)| width)
                .fold(width, usize::max)
        });
        data.align = data.align.max(field_width);
        let built = if let Some(fill) = segment.get("fill") {
            if segment.get("values").is_some() || kind != "u8-array" {
                return Err("fill requires an unsigned byte segment without values".into());
            }
            Data::from_bytes(vec![
                u8::try_from(number(fill, "fill")?)
                    .map_err(|_| "fill exceeds u8")?;
                size
            ])
        } else if let Some(generator) = segment.get("generator") {
            if segment.get("values").is_some() {
                return Err("a generated segment lists no values".into());
            }
            Data::from_bytes(generated_values(generator, kind, width, size / width)?)
        } else if kind == "record" {
            let fields = segment["fields"]
                .as_array()
                .ok_or("record fields missing")?;
            let names = fields
                .iter()
                .map(|f| string(&f["name"], "field name"))
                .collect::<Result<HashSet<_>, _>>()?;
            if names.is_empty() || names.len() != fields.len() {
                return Err("record field names must be nonempty and unique".into());
            }
            let record_label = segment
                .get("label")
                .map(|label| string(label, "record label"))
                .transpose()?;
            let mut seen = HashSet::new();
            let mut built = Data::default();
            for record in segment["records"].as_array().ok_or("records missing")? {
                let record = record.as_object().ok_or("record must be an object")?;
                for (key, value) in record {
                    if Some(key.as_str()) == record_label {
                        let text = string(value, "record label")?;
                        if text.is_empty() || !seen.insert(text.to_string()) {
                            return Err("record labels must be nonempty and unique".into());
                        }
                    } else if !names.contains(key.as_str()) {
                        return Err("record fields differ".into());
                    }
                }
                let before = built.bytes.len();
                for field in fields {
                    let name = string(&field["name"], "field name")?;
                    let value = record
                        .get(name)
                        .or_else(|| field.get("default"))
                        .ok_or("record field absent")?;
                    let values = table_values(value, field, stride, &scope)?;
                    if values.pointers.is_empty() {
                        built.bytes.extend(values.bytes);
                    } else {
                        let at = built.bytes.len();
                        built.bytes.extend(values.bytes);
                        built.pointers.extend(
                            values
                                .pointers
                                .into_iter()
                                .map(|(site, pointer)| (site + at, pointer)),
                        );
                    }
                }
                if built.bytes.len() - before != stride {
                    return Err("record stride differs".into());
                }
            }
            built
        } else if kind == "ascii-fixed" {
            let text = string(&segment["text"], "fixed text")?;
            if stride != 1
                || text.len() >= size
                || !text
                    .bytes()
                    .all(|b| (0x20..=0x7e).contains(&b) || b == b'\n')
            {
                return Err("fixed text differs".into());
            }
            let mut bytes = text.as_bytes().to_vec();
            bytes.resize(size, 0);
            Data::from_bytes(bytes)
        } else if kind == "ascii-pool" {
            // Zero-terminated printable strings; every string after the first
            // starts on an `alignment` boundary, and the pool's `name` lets a
            // later `pool-pointer` segment address them by index.
            let alignment = number(&segment["alignment"], "pool alignment")?;
            let texts = segment["texts"].as_array().ok_or("pool texts missing")?;
            if stride != 1 || !alignment.is_power_of_two() || texts.is_empty() {
                return Err("text pool layout differs".into());
            }
            data.align = data.align.max(alignment);
            let mut bytes = Vec::new();
            let mut offsets = Vec::new();
            for text in texts {
                let text = string(text, "pool text")?;
                if !text.bytes().all(|b| (0x20..=0x7e).contains(&b)) {
                    return Err("pool text is not printable ASCII".into());
                }
                if !offsets.is_empty() {
                    bytes.resize((start + bytes.len()).next_multiple_of(alignment) - start, 0);
                }
                offsets.push(start + bytes.len());
                bytes.extend_from_slice(text.as_bytes());
                bytes.push(0);
            }
            bytes.resize(size, 0);
            let name = string(&segment["name"], "pool name")?;
            if pools.insert(name.to_string(), offsets).is_some() {
                return Err("duplicate pool name".into());
            }
            Data::from_bytes(bytes)
        } else if kind == "pool-pointer" {
            let pool = pools
                .get(string(&segment["pool"], "pointer pool")?)
                .ok_or("pointer pool is not an earlier text pool")?;
            // An entry is a string's pool index or, for a reader that also
            // accepts a marker word in place of a pointer, a named constant.
            let mut built = Data::default();
            for entry in segment["values"]
                .as_array()
                .ok_or("pointer values missing")?
            {
                match entry.as_str() {
                    Some(name) => {
                        let Item::Constant(value) = scope.item(name, segment, false)? else {
                            return Err(format!("pool pointer {name} names no constant"));
                        };
                        built.bytes.extend(encode_integer(value, "le-u32-array")?);
                    }
                    None => {
                        let offset = pool
                            .get(number(entry, "pool index")?)
                            .copied()
                            .ok_or("pool index is outside its pool")?;
                        built.pointer(label, offset as i64);
                    }
                }
            }
            built
        } else {
            table_values(&segment["values"], segment, size, &scope)?
        };
        if built.bytes.len() != size {
            return Err(format!(
                "table segment holds {} bytes, not {size}",
                built.bytes.len()
            ));
        }
        if let Some(count) = segment.get("index_count") {
            let count = number(count, "index count")?;
            if kind != "u8-array"
                || count == 0
                || count > 256
                || built.bytes.iter().any(|&byte| usize::from(byte) >= count)
            {
                return Err("byte index is outside its table".into());
            }
            if segment
                .get("permutation")
                .map(|value| value.as_bool().ok_or("permutation must be boolean"))
                .transpose()?
                == Some(true)
            {
                let mut ordered = built.bytes.clone();
                ordered.sort_unstable();
                if ordered.len() != count
                    || ordered
                        .iter()
                        .enumerate()
                        .any(|(i, &b)| i != usize::from(b))
                {
                    return Err("byte table is not a permutation".into());
                }
            }
        } else if segment.get("permutation").is_some() {
            return Err("permutation requires index_count".into());
        }
        let at = data.bytes.len();
        data.bytes.extend(built.bytes);
        data.pointers.extend(
            built
                .pointers
                .into_iter()
                .map(|(site, pointer)| (site + at, pointer)),
        );
    }
    Ok(data)
}

#[cfg(test)]
mod tests {
    use super::*;
    use serde_json::json;

    fn no_images(name: &str) -> Result<IndexedImage, String> {
        Err(format!("no image {name}"))
    }

    fn table(segments: Value) -> Value {
        json!({"format": 2, "kind": "typed-table", "segments": segments})
    }

    #[test]
    fn integer_arrays_preserve_endianness_sign_and_order() {
        assert_eq!(
            integer_array(&json!([[-128, 127], [0, -1]]), "s8-array").unwrap(),
            [128, 127, 0, 255]
        );
        assert_eq!(
            integer_array(&json!([160, -39]), "be-s16-array").unwrap(),
            [0, 160, 255, 217]
        );
        assert_eq!(
            integer_array(&json!(["0x3c", 1]), "le-u16-array").unwrap(),
            [0x3c, 0, 1, 0]
        );
        for value in [
            json!([128]),
            json!([-129]),
            json!([1.5]),
            json!([null]),
            json!({}),
        ] {
            assert!(integer_array(&value, "s8-array").is_err());
        }
        assert!(integer_array(&json!([32768]), "be-s16-array").is_err());
        assert!(integer_array(&json!([-1]), "u8-array").is_err());
    }

    #[test]
    fn record_tables_read_radix_text_less_their_bias() {
        let records = json!({"fields":["a","b"],"element":"le-u16-array","bias":16,"radix":16,"index_field":"id","records":[{"id":0,"a":"11","b":"1234"}]});
        assert_eq!(record_table(&records).unwrap(), [1, 0, 36, 18]);
        for (pointer, value) in [
            ("/records/0/id", json!(1)),
            ("/records/0/a", json!("f")),
            ("/records/0/a", json!("10010")),
            ("/radix", json!(1)),
        ] {
            let mut bad = records.clone();
            *bad.pointer_mut(pointer).unwrap() = value;
            assert!(record_table(&bad).is_err());
        }
    }

    #[test]
    fn narrow_segment_references_are_offsets_and_words_are_linked() {
        let source = table(json!([
            {"name":"offsets","size":2,"stride":2,"element":"le-u16","values":["entries_end"]},
            {"name":"entries","size":4,"stride":2,"element":"record",
                "fields":[{"name":"entry","element":"s8"},{"name":"value","element":"u8"}],
                "records":[{"entry":-1,"value":7},{"entry":2,"value":9}]},
            {"name":"entries_end","size":1,"stride":1,"element":"u8","values":[255]},
            {"name":"links","size":8,"stride":4,"element":"le-u32","values":["entries[1]", "offsets"]}
        ]));
        let data = typed_table("Map", &source, &no_images).unwrap();
        // Inside a stream the words are offsets from the stream's start.
        assert_eq!(
            data.bytes_at(0).unwrap(),
            [6, 0, 255, 7, 2, 9, 255, 4, 0, 0, 0, 0, 0, 0, 0]
        );
        let text = data.source().unwrap();
        assert!(
            text.contains("\t.global Map\nMap:\nMap_offsets:\n"),
            "{text}"
        );
        assert!(text.contains("\t.4byte Map_entries + 2\n\t.4byte Map_offsets\n"));
        assert_eq!(data.align, 4);
    }

    #[test]
    fn pointers_name_linked_symbols_and_never_hold_addresses() {
        let source = table(json!([
            {"size":12,"stride":4,"element":"thumb-pointer","values":["Callback_Run",null,"Data_Table+8"]},
            {"size":4,"stride":4,"element":"le-s32","values":[-2]}
        ]));
        let data = typed_table("Callbacks", &source, &no_images).unwrap();
        let text = data.source().unwrap();
        assert!(text.contains("\t.4byte Callback_Run\n\t.byte 0x00, 0x00, 0x00, 0x00\n\t.4byte Data_Table + 8\n\t.byte 0xfe, 0xff, 0xff, 0xff\n"), "{text}");
        assert!(data.bytes_at(0).is_err());
        // A pointer names a symbol or is null: never a number.
        for bad in [
            json!(134217729),
            json!("0x08001003"),
            json!("4096"),
            json!(true),
        ] {
            let mut changed = source.clone();
            changed["segments"][0]["values"] = json!([bad.clone(), null, null]);
            assert!(
                typed_table("Callbacks", &changed, &no_images).is_err(),
                "{bad}"
            );
        }
        // An address in a narrow field has no linked form.
        let narrow = table(json!([
            {"size":2,"stride":2,"element":"le-u16","values":["Elsewhere"]}
        ]));
        assert!(typed_table("Narrow", &narrow, &no_images).is_err());
        // Tables and segments record sizes, never addresses.
        let mut placed = source.clone();
        placed["address"] = json!("0x08001000");
        assert!(typed_table("Callbacks", &placed, &no_images)
            .unwrap_err()
            .contains("linker places it"));
        let ended = table(json!([{"end":4,"stride":4,"element":"le-u32","values":[0]}]));
        assert!(typed_table("Ended", &ended, &no_images).is_err());
    }

    #[test]
    fn byte_tables_check_index_ranges_and_permutations() {
        let source = table(json!([
            {"size":4,"element":"u8","stride":1,"values":[2,0,3,1],"index_count":4,"permutation":true}
        ]));
        assert_eq!(
            typed_table("Order", &source, &no_images).unwrap().bytes,
            [2, 0, 3, 1]
        );
        for (pointer, value) in [
            ("/segments/0/values/0", json!(1)),
            ("/segments/0/values/0", json!(4)),
            ("/segments/0/index_count", json!(3)),
        ] {
            let mut bad = source.clone();
            *bad.pointer_mut(pointer).unwrap() = value;
            assert!(typed_table("Order", &bad, &no_images).is_err(), "{pointer}");
        }
    }

    #[test]
    fn records_apply_defaults_capacities_bit_fields_and_constants() {
        let source = json!({"format":2,"kind":"typed-table","names":{"NONE":255},"segments":[
            {"name":"items","size":8,"stride":8,"element":"record","label":"id","fields":[
                {"name":"kind","element":"u8","names":{"SWORD":3}},
                {"name":"flags","element":"u8","bits":{"low":4,"high":4}},
                {"name":"list","element":"u8","capacity":4},
                {"name":"cost","element":"le-u16","default":"0x10"}
            ],"records":[{"id":"sword","kind":"SWORD","flags":[1,2],"list":[1,"NONE"]}]}
        ]});
        assert_eq!(
            typed_table("Items", &source, &no_images).unwrap().bytes,
            [3, 0x21, 1, 255, 0, 0, 0x10, 0]
        );
        let mut overfull = source.clone();
        overfull["segments"][0]["records"][0]["list"] = json!([1, 2, 3, 4, 5]);
        assert!(typed_table("Items", &overfull, &no_images).is_err());
        let mut stray = source.clone();
        stray["segments"][0]["records"][0]["stray"] = json!(1);
        assert!(typed_table("Items", &stray, &no_images).is_err());
    }

    #[test]
    fn text_pools_align_strings_and_link_their_pointers() {
        let source = table(json!([
            {"name":"names","size":8,"stride":1,"element":"ascii-pool","alignment":4,"texts":["ab","c"]},
            {"size":12,"stride":4,"element":"pool-pointer","pool":"names","values":[1,0,"END"],"names":{"END":4294967295u32}}
        ]));
        let data = typed_table("Names", &source, &no_images).unwrap();
        assert_eq!(
            data.bytes_at(0x100).unwrap(),
            [b'a', b'b', 0, 0, b'c', 0, 0, 0, 4, 1, 0, 0, 0, 1, 0, 0, 255, 255, 255, 255]
        );
        assert!(data.source().unwrap().contains("\t.4byte Names + 4\n"));
        let fixed = table(json!([{"size":4,"stride":1,"element":"ascii-fixed","text":"ab"}]));
        assert_eq!(
            typed_table("Fixed", &fixed, &no_images).unwrap().bytes,
            [b'a', b'b', 0, 0]
        );
    }

    #[test]
    fn generated_reciprocals_follow_their_formula_and_width() {
        let source = table(json!([
            {"size":24,"element":"le-u32","stride":4,
             "generator":{"formula":"ceiling-reciprocal","numerator_bits":32}}
        ]));
        let words: Vec<u32> = typed_table("Reciprocals", &source, &no_images)
            .unwrap()
            .bytes
            .chunks_exact(4)
            .map(|word| u32::from_le_bytes(word.try_into().unwrap()))
            .collect();
        assert_eq!(
            words,
            [0, 0, 0x8000_0000, 0x5555_5556, 0x4000_0000, 0x3333_3334]
        );
        for (key, value) in [
            ("formula", json!("floor-reciprocal")),
            ("numerator_bits", json!(65)),
        ] {
            let mut changed = source.clone();
            changed["segments"][0]["generator"][key] = value;
            assert!(typed_table("Reciprocals", &changed, &no_images).is_err());
        }
    }

    #[test]
    fn glyph_fields_pack_rows_from_their_atlas() {
        let mut pixels = vec![0u32; 16 * 16];
        pixels[0] = 1;
        pixels[16 + 15] = 1;
        let image = move |_: &str| {
            Ok(IndexedImage {
                width: 16,
                height: 16,
                pixels: pixels.clone(),
                palette: vec![[0, 0, 0], [255, 255, 255]],
                has_transparency: false,
            })
        };
        let source = table(json!([
            {"size":32,"stride":32,"element":"record",
             "image":{"source":"GLYPHS.PNG","frame_width":16,"frame_height":16,"columns":1},
             "fields":[{"name":"advance","element":"le-u16"},{"name":"glyph","element":"1bpp-rows","rows":15}],
             "records":[{"advance":6,"glyph":0}]}
        ]));
        let mut expected = vec![6, 0, 0, 0x80, 1, 0];
        expected.resize(32, 0);
        assert_eq!(
            typed_table("Font", &source, &image).unwrap().bytes,
            expected
        );
    }
}
