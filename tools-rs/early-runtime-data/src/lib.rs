// What is actually IN the two spans the early-runtime code reads?
//
// `0x08012f20..0x08013784` and `0x0801403c..0x08015000` are ROM regions the
// early-runtime path touches before any map is loaded. They are not code, so a
// disassembler says nothing useful about them; they are also not one thing, so
// a single blob would erase the structure. This tool holds a CATALOG that
// partitions both spans -- tiles, lookup tables, coefficient words, an object
// command stream, a handler pointer table, and the still-unexplained residue --
// exports each partition as a reviewable source form, and then proves the
// exported sources rebuild the original bytes exactly. Anything that cannot be
// explained yet is kept as an explicit, verifiable byte source rather than as a
// silent copy of the ROM, so the "not understood yet" surface stays visible and
// shrinks as meanings are found.
//
// Rust port of tools/make/early_runtime_data.ts. Byte-for-byte parity with the
// TypeScript -- exported package, built regions, stdout, and error text -- is
// the contract. See the PORT NOTEs for the deliberate divergences.
//
//   early-runtime-data export ROM --directory DIR
//   early-runtime-data build INDEX --directory DIR
//   early-runtime-data verify ROM INDEX
//   early-runtime-data --self-test

pub mod jsvalue;
pub mod nodepath;

use jsvalue::{as_array, as_number, as_str, get, is_plain_object, normalize_numbers, to_js_string};
use serde_json::{Map, Value};
use std::path::Path;

// ---------------------------------------------------------------------------
// Errors
// ---------------------------------------------------------------------------

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Error(pub String);

impl std::fmt::Display for Error {
    fn fmt(&self, formatter: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        formatter.write_str(&self.0)
    }
}

impl std::error::Error for Error {}

pub type Result<T> = std::result::Result<T, Error>;

fn fail<T>(message: impl Into<String>) -> Result<T> {
    Err(Error(message.into()))
}

// ---------------------------------------------------------------------------
// Constants -- these mirror the exported TypeScript values exactly.
// ---------------------------------------------------------------------------

pub const ROM_BASE: i64 = 0x0800_0000;
pub const EARLY_RUNTIME_ADDRESS: i64 = 0x0801_2f20;
pub const EARLY_RUNTIME_END: i64 = 0x0801_3784;
pub const POST_MAP_ADDRESS: i64 = 0x0801_403c;
pub const POST_MAP_END: i64 = 0x0801_5000;
pub const EARLY_RUNTIME_SOURCE_BYTES: i64 =
    EARLY_RUNTIME_END - EARLY_RUNTIME_ADDRESS + POST_MAP_END - POST_MAP_ADDRESS;

const DISPLAY_SOURCE: &str = "display.4bpp.png";
const DISPLAY_WIDTH: usize = 32;
const DISPLAY_HEIGHT: usize = 8;

pub type Rgb = [u8; 3];

fn palette() -> Vec<Rgb> {
    (0..16u8).map(|index| [index * 8, index * 8, index * 8]).collect()
}

/// `TYPED_WIDTH` -- the encodings with a fixed element width. Written as a slice
/// rather than a map because iteration order is never observed and the table is
/// six entries long.
fn typed_width(encoding: &str) -> Option<usize> {
    match encoding {
        "u8" => Some(1),
        "s16le" => Some(2),
        "s32le" | "u32le" | "pointer32le" | "command32le" => Some(4),
        _ => None,
    }
}

pub fn repo_root() -> String {
    let manifest = Path::new(env!("CARGO_MANIFEST_DIR"));
    manifest
        .parent()
        .and_then(Path::parent)
        .expect("the crate lives two levels below the repository root")
        .to_string_lossy()
        .into_owned()
}

pub fn default_catalog_path() -> String {
    nodepath::join(&[&repo_root(), "assets/data/early_runtime_data.json"])
}

// ---------------------------------------------------------------------------
// Hand-written regular expressions
//
// The originals are all anchored, ASCII-only character classes. Spelling them
// out beats a dependency, and beats a regex engine whose `\d` is Unicode-aware
// where JavaScript's is not.
// ---------------------------------------------------------------------------

/// `/^[a-z0-9_]+$/`
fn is_identifier(text: &str) -> bool {
    !text.is_empty()
        && text
            .bytes()
            .all(|byte| byte.is_ascii_lowercase() || byte.is_ascii_digit() || byte == b'_')
}

/// `/^0x080[0-9a-f]{5}$/`
fn is_rom_address(text: &str) -> bool {
    text.len() == 10
        && text.starts_with("0x080")
        && text.as_bytes()[5..].iter().all(u8::is_ascii_hexdigit)
        && text.as_bytes()[5..].iter().all(|byte| !byte.is_ascii_uppercase())
}

/// `/^0x[0-9a-f]{8}$/`
fn is_canonical_word(text: &str) -> bool {
    text.len() == 10
        && text.starts_with("0x")
        && text.as_bytes()[2..]
            .iter()
            .all(|byte| byte.is_ascii_digit() || (b'a'..=b'f').contains(byte))
}

/// `/^Func_080[0-7][0-9a-f]{4}$/`
fn is_thumb_symbol(text: &str) -> bool {
    let bytes = text.as_bytes();
    text.len() == 13
        && text.starts_with("Func_080")
        && (b'0'..=b'7').contains(&bytes[8])
        && bytes[9..]
            .iter()
            .all(|byte| byte.is_ascii_digit() || (b'a'..=b'f').contains(byte))
}

/// JavaScript sorts strings by UTF-16 code unit; Rust sorts `String` by UTF-8
/// byte. The two agree on ASCII and disagree above U+FFFF, and both
/// `Object.keys(...).sort()` and `readdirSync(...).sort()` here run on
/// attacker-supplied names.
fn js_sort(values: &mut [String]) {
    values.sort_by(|left, right| left.encode_utf16().cmp(right.encode_utf16()));
}

// ---------------------------------------------------------------------------
// Shared validators
// ---------------------------------------------------------------------------

fn pretty(value: &Value) -> String {
    format!("{}\n", canonical_json::canonical_json(value))
}

fn hexadecimal(value: i64) -> String {
    format!("0x{:08x}", value as u32)
}

/// `address(value, label)` -- a canonical ROM address string, as a number.
fn address_of(value: Option<&Value>, label: &str) -> Result<i64> {
    match as_str(value) {
        Some(text) if is_rom_address(text) => {
            Ok(i64::from_str_radix(&text[2..], 16).expect("ten canonical hex digits"))
        }
        _ => fail(format!("{label} is not a canonical ROM address")),
    }
}

fn address(value: Option<&Value>) -> Result<i64> {
    address_of(value, "address")
}

fn object<'a>(value: Option<&'a Value>, label: &str) -> Result<&'a Map<String, Value>> {
    if !is_plain_object(value) {
        return fail(format!("{label} must be an object"));
    }
    Ok(value.and_then(Value::as_object).expect("checked above"))
}

fn exact_keys(value: &Map<String, Value>, keys: &[&str], label: &str) -> Result<()> {
    let mut actual: Vec<String> = value.keys().cloned().collect();
    js_sort(&mut actual);
    let mut expected: Vec<String> = keys.iter().map(|key| (*key).to_string()).collect();
    js_sort(&mut expected);
    if actual != expected {
        return fail(format!("{label} fields differ"));
    }
    Ok(())
}

/// `Number.isSafeInteger` plus a range check. The result is an `i64` because
/// every safe integer fits one exactly.
fn integer(value: Option<&Value>, minimum: i64, maximum: i64, label: &str) -> Result<i64> {
    let raw = match as_number(value) {
        Some(raw) => raw,
        None => return fail(format!("{label} is outside its range")),
    };
    let safe = raw.is_finite() && raw.fract() == 0.0 && raw.abs() <= 9_007_199_254_740_991.0;
    if !safe || raw < minimum as f64 || raw > maximum as f64 {
        return fail(format!("{label} is outside its range"));
    }
    Ok(raw as i64)
}

fn array<'a>(value: Option<&'a Value>, length: usize, label: &str) -> Result<&'a Vec<Value>> {
    match as_array(value) {
        Some(items) if items.len() == length => Ok(items),
        _ => fail(format!("{label} requires {length} entries")),
    }
}

// ---------------------------------------------------------------------------
// Catalog
//
// PORT NOTE: the catalog is kept as `serde_json::Value` rather than deserialised
// into structs. The TypeScript casts the parsed JSON to an interface WITHOUT
// checking it and then validates field by field, so a catalog whose `shape`
// holds strings, or whose `segments` is `null`, reaches the validators. Typed
// structs would reject those inputs earlier and with different messages.
// ---------------------------------------------------------------------------

fn shape_of(entry: &Value) -> Option<&Vec<Value>> {
    as_array(get(entry, "shape"))
}

/// `elements(shape)` -- the product of a positive integer shape.
fn elements(entry: &Value) -> Result<i64> {
    let shape = match get(entry, "shape") {
        None => return fail("typed entry requires a positive shape"),
        Some(value) => value,
    };
    let items = match shape.as_array() {
        Some(items) => items,
        // PORT NOTE: JavaScript reaches `shape.some(...)` on a non-array and
        // throws a TypeError. The message differs; the rejection does not.
        None => return fail("typed entry requires a positive shape"),
    };
    if items.is_empty() {
        return fail("typed entry requires a positive shape");
    }
    let mut product: i64 = 1;
    for item in items {
        let raw = match item.as_f64() {
            Some(raw) if item.is_number() => raw,
            _ => return fail("typed entry requires a positive shape"),
        };
        if !raw.is_finite() || raw.fract() != 0.0 || raw <= 0.0 {
            return fail("typed entry requires a positive shape");
        }
        product = product.saturating_mul(raw as i64);
    }
    Ok(product)
}

fn entry_id(entry: &Value) -> String {
    to_js_string(get(entry, "id"))
}

fn expected_size(entry: &Value) -> Result<Option<i64>> {
    let encoding = get(entry, "encoding");
    if let Some(width) = as_str(encoding).and_then(typed_width) {
        return Ok(Some(elements(entry)? * width as i64));
    }
    if as_str(encoding) == Some("gba_4bpp_tiles") {
        let shape = shape_of(entry);
        let ok = shape.is_some_and(|items| {
            items.len() == 3 && items[1] == 8 && items[2] == 8
        });
        if !ok {
            return fail(format!("{}: 4bpp tiles require [count,8,8]", entry_id(entry)));
        }
        let count = shape.expect("checked above")[0]
            .as_f64()
            .ok_or_else(|| Error(format!("{}: 4bpp tiles require [count,8,8]", entry_id(entry))))?;
        return Ok(Some(count as i64 * 32));
    }
    Ok(None)
}

fn validate_entry(entry: &Value, region_start: i64, region_end: i64) -> Result<()> {
    let consumers_ok = match as_array(get(entry, "consumers")) {
        None => false,
        Some(items) => items.iter().all(|item| match item.as_str() {
            Some(text) => !text.contains("..") && !text.starts_with('/'),
            None => false,
        }),
    };
    if !is_identifier(&entry_id(entry)) || !consumers_ok {
        return fail("invalid catalog entry");
    }
    let id = entry_id(entry);
    let start = address_of(get(entry, "address"), &format!("{id} address"))?;
    let end = address_of(get(entry, "end"), &format!("{id} end"))?;
    if start < region_start || end <= start || end > region_end {
        return fail(format!("{id}: invalid range"));
    }
    if let Some(size) = expected_size(entry)? {
        if size != end - start {
            return fail(format!("{id}: shape does not match range"));
        }
    }
    if let Some(segments) = get(entry, "segments") {
        let items = match segments.as_array() {
            Some(items) => items,
            // PORT NOTE: `null.some(...)` is a TypeError in JavaScript. Same
            // rejection, different message.
            None => return fail(format!("{id}: invalid command segments")),
        };
        let mut sum: i64 = 0;
        let mut bad = as_str(get(entry, "encoding")) != Some("command32le");
        for item in items {
            match item.as_f64() {
                Some(raw) if item.is_number() && raw.is_finite() && raw.fract() == 0.0 && raw > 0.0 => {
                    sum += raw as i64;
                }
                _ => {
                    bad = true;
                    // The JavaScript `some` short-circuits, but `reduce` still
                    // runs afterwards only when `some` was false, so the sum is
                    // irrelevant once `bad` is set.
                    break;
                }
            }
        }
        if bad || sum != elements(entry)? {
            return fail(format!("{id}: invalid command segments"));
        }
    }
    Ok(())
}

fn find_entry<'a>(region: &'a Value, id: &str, view: bool) -> Result<&'a Value> {
    let list = get(region, if view { "views" } else { "partitions" })
        .and_then(Value::as_array)
        .map(|items| items.as_slice())
        .unwrap_or(&[]);
    match list.iter().find(|item| as_str(get(item, "id")) == Some(id)) {
        Some(found) => Ok(found),
        None => fail(format!("{}: missing {id}", to_js_string(get(region, "id")))),
    }
}

fn same_shape(entry: &Value, expected: &[i64]) -> bool {
    match shape_of(entry) {
        None => false,
        Some(items) => {
            items.len() == expected.len()
                && items
                    .iter()
                    .zip(expected)
                    .all(|(item, want)| item.as_f64() == Some(*want as f64) && item.is_number())
        }
    }
}

fn exact_entry(entry: &Value, start: i64, end: i64, encoding: &str, shape: Option<&[i64]>) -> Result<()> {
    let differs = address(get(entry, "address"))? != start
        || address(get(entry, "end"))? != end
        || as_str(get(entry, "encoding")) != Some(encoding)
        || shape.is_some_and(|want| !same_shape(entry, want));
    if differs {
        return fail(format!("{}: canonical layout differs", entry_id(entry)));
    }
    Ok(())
}

type LayoutRow = (&'static str, i64, i64, &'static str, Option<&'static [i64]>);

fn canonical_partitions() -> Vec<LayoutRow> {
    const TILES: &[i64] = &[4, 8, 8];
    const COEFFICIENTS: &[i64] = &[16];
    const SURFACE: &[i64] = &[3, 16];
    const LIMITS: &[i64] = &[2];
    const PROGRAMS: &[i64] = &[38];
    const HANDLERS: &[i64] = &[64];
    vec![
        ("display_tiles", 0x0801_2f20, 0x0801_2fa0, "gba_4bpp_tiles", Some(TILES)),
        ("shared_lookup_storage", 0x0801_2fa0, 0x0801_34fc, "overlapping_views", None),
        ("interpolation_coefficients", 0x0801_34fc, 0x0801_353c, "u32le", Some(COEFFICIENTS)),
        ("surface_lookup", 0x0801_353c, 0x0801_356c, "u8", Some(SURFACE)),
        ("unclassified_0801356c", 0x0801_356c, 0x0801_3584, "unresolved", None),
        ("render_limits", 0x0801_3584, 0x0801_358c, "s32le", Some(LIMITS)),
        ("object_programs", 0x0801_358c, 0x0801_3624, "command32le", Some(PROGRAMS)),
        ("object_handlers", 0x0801_3624, 0x0801_3724, "pointer32le", Some(HANDLERS)),
        ("unclassified_08013724", 0x0801_3724, 0x0801_3784, "unresolved", None),
    ]
}

fn canonical_views() -> Vec<LayoutRow> {
    const PAGES: &[i64] = &[5, 256];
    const GRID: &[i64] = &[16, 16];
    const SLOTS: &[i64] = &[2, 6];
    const MODES: &[i64] = &[16];
    vec![
        ("byte_translation_pages", 0x0801_2fa0, 0x0801_34a0, "u8", Some(PAGES)),
        ("grid_lookup_a", 0x0801_32fc, 0x0801_33fc, "u8", Some(GRID)),
        ("grid_lookup_b", 0x0801_33fc, 0x0801_34fc, "u8", Some(GRID)),
        ("resource_slot_descriptors", 0x0801_32cc, 0x0801_32fc, "u32le", Some(SLOTS)),
        ("signed_mode_table", 0x0801_3254, 0x0801_3274, "s16le", Some(MODES)),
    ]
}

fn find_region<'a>(catalog: &'a Value, id: &str) -> Option<&'a Value> {
    get(catalog, "regions")
        .and_then(Value::as_array)
        .and_then(|regions| regions.iter().find(|region| as_str(get(region, "id")) == Some(id)))
}

fn region_length(catalog: &Value) -> usize {
    get(catalog, "regions").and_then(Value::as_array).map_or(0, Vec::len)
}

/// The two canonical regions, by identity and by exact layout.
fn canonical_layout(catalog: &Value) -> Result<(&Value, &Value)> {
    let early = find_region(catalog, "early_runtime_tables");
    let residual = find_region(catalog, "post_map_load_residual");
    let (early, residual) = match (early, residual) {
        (Some(early), Some(residual)) if region_length(catalog) == 2 => (early, residual),
        _ => return fail("canonical early-runtime regions differ"),
    };
    let partitions = canonical_partitions();
    let views = canonical_views();
    let partition_count = get(early, "partitions").and_then(Value::as_array).map_or(usize::MAX, Vec::len);
    let view_count = get(early, "views").and_then(Value::as_array).map_or(usize::MAX, Vec::len);
    if partition_count != partitions.len() || view_count != views.len() {
        return fail("canonical early-runtime catalog size differs");
    }
    for (id, start, end, encoding, shape) in partitions {
        exact_entry(find_entry(early, id, false)?, start, end, encoding, shape)?;
    }
    for (id, start, end, encoding, shape) in views {
        exact_entry(find_entry(early, id, true)?, start, end, encoding, shape)?;
    }
    exact_entry(
        find_entry(residual, "unreferenced_storage", false)?,
        POST_MAP_ADDRESS,
        POST_MAP_END,
        "unresolved",
        None,
    )?;
    if as_str(get(early, "claim")) != Some("pending_private_verification")
        || as_str(get(residual, "claim")) != Some("unresolved")
    {
        return fail("canonical claim state differs");
    }
    Ok((early, residual))
}

pub fn validate_catalog(catalog: &Value) -> Result<()> {
    let regions = match get(catalog, "regions") {
        Some(Value::Array(regions)) if !regions.is_empty() => regions,
        _ => return fail("unsupported early-runtime catalog"),
    };
    if get(catalog, "format") != Some(&Value::from(1))
        || as_str(get(catalog, "address_space")) != Some("gba-rom")
        || as_str(get(catalog, "canonical_target")) != Some("gs1-en")
    {
        return fail("unsupported early-runtime catalog");
    }
    let mut ids: Vec<String> = Vec::new();
    let mut previous_region_end: i64 = 0;
    for region in regions {
        let id = to_js_string(get(region, "id"));
        if !is_identifier(&id) || ids.contains(&id) {
            return fail("invalid or duplicate region id");
        }
        ids.push(id.clone());
        let start = address_of(get(region, "address"), &format!("{id} address"))?;
        let end = address_of(get(region, "end"), &format!("{id} end"))?;
        if start < previous_region_end || end <= start {
            return fail(format!("{id}: invalid region range"));
        }
        previous_region_end = end;
        let partitions = as_array(get(region, "partitions"));
        let views = as_array(get(region, "views"));
        match (partitions, views) {
            (Some(partitions), Some(_)) if !partitions.is_empty() => {}
            _ => return fail(format!("{id}: missing partitions or views")),
        }
        let partitions = partitions.expect("checked above");
        let views = views.expect("checked above");
        let mut entry_ids: Vec<String> = Vec::new();
        let mut cursor = start;
        for item in partitions {
            validate_entry(item, start, end)?;
            let item_id = entry_id(item);
            if entry_ids.contains(&item_id) {
                return fail(format!("{id}: duplicate entry id"));
            }
            entry_ids.push(item_id.clone());
            if address(get(item, "address"))? != cursor {
                return fail(format!("{item_id}: partition gap or overlap"));
            }
            cursor = address(get(item, "end"))?;
        }
        if cursor != end {
            return fail(format!("{id}: partitions do not cover region"));
        }
        for item in views {
            validate_entry(item, start, end)?;
            let item_id = entry_id(item);
            if entry_ids.contains(&item_id) {
                return fail(format!("{id}: duplicate entry id"));
            }
            entry_ids.push(item_id);
        }
    }
    let (early, _) = canonical_layout(catalog)?;
    let backing = find_entry(early, "shared_lookup_storage", false)?;
    let backing_start = address(get(backing, "address"))?;
    let backing_end = address(get(backing, "end"))?;
    for view in as_array(get(early, "views")).expect("validated above") {
        if address(get(view, "address"))? < backing_start || address(get(view, "end"))? > backing_end {
            return fail(format!("{}: view escapes canonical backing storage", entry_id(view)));
        }
    }
    let mut total: i64 = 0;
    for region in regions {
        total += address(get(region, "end"))? - address(get(region, "address"))?;
    }
    if total != EARLY_RUNTIME_SOURCE_BYTES {
        return fail("canonical source-byte total differs");
    }
    Ok(())
}

/// Bun rethrows Node's `SystemError` and `SyntaxError` verbatim, and because
/// neither is an `Error` subclass it constructed, it prints them WITHOUT the
/// `error: ` prefix. `main.rs` reproduces that by keying on the prefix here, so
/// these two spellings are load-bearing.
///
/// PORT NOTE: the text after `JSON Parse error: ` comes from JavaScriptCore in
/// Bun and from `serde_json` here, so the wording of a malformed-JSON message
/// differs even though the leading `SyntaxError: JSON Parse error: ` and the
/// exit code match. Nothing in the repository matches on that tail.
pub fn system_error(syscall: &str, path: &str, error: &std::io::Error) -> Error {
    let (code, text) = match error.raw_os_error() {
        Some(2) => ("ENOENT", "no such file or directory"),
        Some(13) => ("EACCES", "permission denied"),
        Some(20) => ("ENOTDIR", "not a directory"),
        Some(21) => ("EISDIR", "illegal operation on a directory"),
        Some(62) => ("ELOOP", "too many symbolic links encountered"),
        Some(66) => ("ENOTEMPTY", "directory not empty"),
        _ => ("EIO", "input/output error"),
    };
    Error(format!("{code}: {text}, {syscall} '{path}'"))
}

fn read_utf8(path: &str) -> Result<String> {
    match std::fs::read(path) {
        Ok(bytes) => Ok(String::from_utf8_lossy(&bytes).into_owned()),
        Err(error) => Err(system_error("open", path, &error)),
    }
}

fn parse_json(text: &str) -> Result<Value> {
    match serde_json::from_str::<Value>(text) {
        Ok(value) => Ok(normalize_numbers(value)),
        Err(error) => fail(format!("SyntaxError: JSON Parse error: {error}")),
    }
}

pub fn load_catalog(path: &str) -> Result<Value> {
    let text = read_utf8(path)?;
    let catalog = parse_json(&text)?;
    if !canonical_json::is_canonical_json_text(&text, &catalog) {
        return fail("early-runtime catalog is not canonical JSON");
    }
    validate_catalog(&catalog)?;
    Ok(catalog)
}

// ---------------------------------------------------------------------------
// Typed encodings
// ---------------------------------------------------------------------------

fn value_range(encoding: &str) -> Result<(i64, i64)> {
    match encoding {
        "u8" => Ok((0, 0xff)),
        "s16le" => Ok((-0x8000, 0x7fff)),
        "s32le" => Ok((-0x8000_0000, 0x7fff_ffff)),
        "u32le" | "pointer32le" | "command32le" => Ok((0, 0xffff_ffff)),
        other => fail(format!("cannot encode {other}")),
    }
}

pub fn decode_typed(data: &[u8], encoding: &str) -> Result<Vec<i64>> {
    let width = match typed_width(encoding) {
        Some(width) => width,
        None => return fail(format!("cannot decode {encoding}")),
    };
    if !data.len().is_multiple_of(width) {
        return fail("typed data has a partial element");
    }
    let mut result = Vec::with_capacity(data.len() / width);
    for offset in (0..data.len()).step_by(width) {
        let value = match encoding {
            "u8" => i64::from(data[offset]),
            "s16le" => i64::from(i16::from_le_bytes([data[offset], data[offset + 1]])),
            "s32le" => i64::from(i32::from_le_bytes([
                data[offset],
                data[offset + 1],
                data[offset + 2],
                data[offset + 3],
            ])),
            _ => i64::from(u32::from_le_bytes([
                data[offset],
                data[offset + 1],
                data[offset + 2],
                data[offset + 3],
            ])),
        };
        result.push(value);
    }
    Ok(result)
}

pub fn encode_typed(values: &[Value], encoding: &str) -> Result<Vec<u8>> {
    let width = match typed_width(encoding) {
        Some(width) => width,
        None => return fail(format!("cannot encode {encoding}")),
    };
    let (minimum, maximum) = value_range(encoding)?;
    let mut data = vec![0u8; values.len() * width];
    for (index, item) in values.iter().enumerate() {
        let value = integer(Some(item), minimum, maximum, &format!("{encoding} value {index}"))?;
        let offset = index * width;
        match encoding {
            "u8" => data[offset] = value as u8,
            "s16le" => data[offset..offset + 2].copy_from_slice(&(value as i16).to_le_bytes()),
            "s32le" => data[offset..offset + 4].copy_from_slice(&(value as i32).to_le_bytes()),
            _ => data[offset..offset + 4].copy_from_slice(&(value as u32).to_le_bytes()),
        }
    }
    Ok(data)
}

fn encode_numbers(values: &[i64], encoding: &str) -> Result<Vec<u8>> {
    let boxed: Vec<Value> = values.iter().map(|value| Value::from(*value)).collect();
    encode_typed(&boxed, encoding)
}

fn flat_values(value: Option<&Value>, count: usize, encoding: &str, label: &str) -> Result<Vec<i64>> {
    let (minimum, maximum) = value_range(encoding)?;
    let items = array(value, count, label)?;
    let mut result = Vec::with_capacity(items.len());
    for (index, item) in items.iter().enumerate() {
        result.push(integer(Some(item), minimum, maximum, &format!("{label} {index}"))?);
    }
    Ok(result)
}

fn rows(
    value: Option<&Value>,
    row_count: usize,
    columns: usize,
    encoding: &str,
    label: &str,
) -> Result<Vec<i64>> {
    let items = array(value, row_count, label)?;
    let mut result = Vec::new();
    for (index, row) in items.iter().enumerate() {
        result.extend(flat_values(Some(row), columns, encoding, &format!("{label} row {index}"))?);
    }
    Ok(result)
}

// ---------------------------------------------------------------------------
// Display tiles
//
// PORT NOTE: `png()` is inlined from tools/make/skip_sprite_archive.ts, which
// has no Rust crate yet. Only the indexed-PNG writer is needed, and it is nine
// lines; the chunk framing and the deflate come from the already-ported
// `export-asset` and `alchemy-zlib` crates so the byte output stays shared.
// ---------------------------------------------------------------------------

fn png_header(width: usize, height: usize, depth: u8, color_type: u8) -> Vec<u8> {
    let mut header = Vec::with_capacity(13);
    header.extend_from_slice(&(width as u32).to_be_bytes());
    header.extend_from_slice(&(height as u32).to_be_bytes());
    header.extend_from_slice(&[depth, color_type, 0, 0, 0]);
    header
}

fn png(pixels: &[u8], width: usize, height: usize, colors: &[Rgb]) -> Result<Vec<u8>> {
    if pixels.len() != width * height || colors.is_empty() || colors.len() > 256 {
        return fail("invalid indexed sprite image");
    }
    let flat: Vec<u8> = colors.iter().flat_map(|color| color.iter().copied()).collect();
    let mut rows = vec![0u8; height * (width + 1)];
    for y in 0..height {
        rows[y * (width + 1) + 1..y * (width + 1) + 1 + width]
            .copy_from_slice(&pixels[y * width..(y + 1) * width]);
    }
    let mut transparency = vec![0u8];
    transparency.extend(std::iter::repeat_n(0xffu8, colors.len() - 1));
    let compressed = alchemy_zlib::deflate_sync(&rows, alchemy_zlib::DeflateOptions { level: Some(9) });

    let mut out = Vec::new();
    out.extend_from_slice(&import_asset::PNG_SIGNATURE);
    out.extend(export_asset::chunk(b"IHDR", &png_header(width, height, 8, 3)));
    out.extend(export_asset::chunk(b"PLTE", &flat));
    out.extend(export_asset::chunk(b"tRNS", &transparency));
    out.extend(export_asset::chunk(b"IDAT", &compressed));
    out.extend(export_asset::chunk(b"IEND", &[]));
    Ok(out)
}

fn write_display_tiles(raw: &[u8], path: &str) -> Result<()> {
    if raw.len() != 128 {
        return fail("display tile extent differs");
    }
    let mut pixels = vec![0u8; DISPLAY_WIDTH * DISPLAY_HEIGHT];
    let mut source = 0usize;
    for tile in 0..4usize {
        for y in 0..8usize {
            for x in (0..8usize).step_by(2) {
                let value = raw[source];
                source += 1;
                let target = y * DISPLAY_WIDTH + tile * 8 + x;
                pixels[target] = value & 15;
                pixels[target + 1] = value >> 4;
            }
        }
    }
    let encoded = png(&pixels, DISPLAY_WIDTH, DISPLAY_HEIGHT, &palette())?;
    write_file(path, &encoded)
}

fn read_display_tiles(path: &str) -> Result<Vec<u8>> {
    let encoded = read_bytes(path)?;
    let image = import_asset::indexed_png(&encoded).map_err(|error| Error(error.0))?;
    let expected_palette = palette();
    let layout_ok = image.width as usize == DISPLAY_WIDTH
        && image.height as usize == DISPLAY_HEIGHT
        && image.palette == expected_palette
        && image.pixels.iter().all(|value| *value < 16);
    if !layout_ok {
        return fail("display tile PNG layout differs");
    }
    let pixels: Vec<u8> = image.pixels.iter().map(|value| *value as u8).collect();
    if encoded != png(&pixels, image.width as usize, image.height as usize, &image.palette)? {
        return fail("display tile PNG is not canonical");
    }
    let mut output = vec![0u8; 128];
    let mut target = 0usize;
    for tile in 0..4usize {
        for y in 0..8usize {
            for x in (0..8usize).step_by(2) {
                let source = y * DISPLAY_WIDTH + tile * 8 + x;
                output[target] = pixels[source] | (pixels[source + 1] << 4);
                target += 1;
            }
        }
    }
    Ok(output)
}

// ---------------------------------------------------------------------------
// Residual spans
//
// 保留領域は、証明できる埋め値なら簡潔に保持し、それ以外は明示的な
// バイト列として保持する。後者も ROM の代替入力ではなく、検証可能な
// 正規ソースであり、意味が判明した時点でより強い表現に置き換える。
// ---------------------------------------------------------------------------

fn residual_source(data: &[u8], label: &str) -> Result<Value> {
    if data.is_empty() {
        return fail(format!("{label} is empty"));
    }
    let first = data[0];
    if (first == 0 || first == 0xff) && data.iter().all(|value| *value == first) {
        let mut map = Map::new();
        map.insert("kind".to_string(), Value::from("uniform_fill"));
        map.insert("value".to_string(), Value::from(format!("0x{first:02x}")));
        return Ok(Value::Object(map));
    }
    let mut map = Map::new();
    map.insert("kind".to_string(), Value::from("byte_values"));
    map.insert(
        "values".to_string(),
        Value::Array(data.iter().map(|byte| Value::from(i64::from(*byte))).collect()),
    );
    Ok(Value::Object(map))
}

fn build_residual(value: Option<&Value>, size: usize, label: &str) -> Result<Vec<u8>> {
    let source = object(value, label)?;
    let source_value = Value::Object(source.clone());
    if as_str(source.get("kind")) == Some("uniform_fill") {
        exact_keys(source, &["kind", "value"], label)?;
        let fill = as_str(source.get("value"));
        let byte = match fill {
            Some("0x00") => 0u8,
            Some("0xff") => 0xffu8,
            _ => return fail(format!("{label} has an unsupported uniform fill")),
        };
        return Ok(vec![byte; size]);
    }
    exact_keys(source, &["kind", "values"], label)?;
    if as_str(source.get("kind")) != Some("byte_values") {
        return fail(format!("{label} has an unsupported residual representation"));
    }
    let values = flat_values(get(&source_value, "values"), size, "u8", &format!("{label} values"))?;
    encode_numbers(&values, "u8")
}

// ---------------------------------------------------------------------------
// Views, words and handlers
// ---------------------------------------------------------------------------

/// PORT NOTE: a catalog view without a `shape` would make the TypeScript emit
/// the bare token `undefined` into the JSON it writes (`JSON.stringify(undefined)`
/// returns `undefined`, and the template literal stringifies it). That is
/// unreachable: `canonicalLayout` runs first and rejects any early-region view
/// whose shape is not the exact canonical one, and every canonical view has a
/// shape. Here the key is simply omitted in that impossible case.
fn view_sources(region: &Value) -> Vec<Value> {
    let views = as_array(get(region, "views")).cloned().unwrap_or_default();
    views
        .iter()
        .map(|item| {
            let mut map = Map::new();
            for key in ["id", "address", "end", "encoding", "shape"] {
                if let Some(value) = get(item, key) {
                    map.insert(key.to_string(), value.clone());
                }
            }
            Value::Object(map)
        })
        .collect()
}

fn validate_view_sources(value: Option<&Value>, region: &Value) -> Result<()> {
    let expected = view_sources(region);
    let actual = array(value, expected.len(), "shared lookup views")?;
    for (index, want) in expected.iter().enumerate() {
        let label = format!("shared lookup view {index}");
        let item = object(actual.get(index), &label)?;
        exact_keys(item, &["id", "address", "end", "encoding", "shape"], &label)?;
        if pretty(&Value::Object(item.clone())) != pretty(want) {
            return fail(format!("shared lookup view {index} differs from catalog"));
        }
    }
    Ok(())
}

fn word(value: Option<&Value>, label: &str) -> Result<i64> {
    match as_str(value) {
        Some(text) if is_canonical_word(text) => {
            Ok(i64::from_str_radix(&text[2..], 16).expect("eight canonical hex digits"))
        }
        _ => fail(format!("{label} is not a canonical word")),
    }
}

/// PORT NOTE: `value & ~1` in JavaScript runs `ToInt32` on both operands, so a
/// pointer word at or above `0x80000000` becomes NEGATIVE and then fails the
/// `target < ROM_BASE` check. Reading the word into an `i64` and masking would
/// keep it positive and let it through, so the `ToInt32` step is explicit.
fn handler_source(value: i64) -> Result<Value> {
    if value == 0 {
        return Ok(Value::Null);
    }
    let target = i64::from((value as u32 as i32) & !1i32);
    if (value & 1) == 0 || !(ROM_BASE..ROM_BASE + 0x0080_0000).contains(&target) {
        return fail("object handler is not a null or Thumb ROM function pointer");
    }
    Ok(Value::from(format!("Func_{target:08x}")))
}

fn handler_value(value: Option<&Value>, label: &str) -> Result<i64> {
    if value == Some(&Value::Null) {
        return Ok(0);
    }
    match as_str(value) {
        Some(text) if is_thumb_symbol(text) => {
            let parsed = i64::from_str_radix(&text[5..], 16).expect("eight canonical hex digits");
            Ok(i64::from((parsed as u32 as i32 | 1) as u32))
        }
        _ => fail(format!("{label} is not a canonical Thumb function symbol")),
    }
}

fn program_sources(data: &[u8], spec: &Value) -> Result<Vec<Value>> {
    let values = decode_typed(data, "command32le")?;
    let segments = as_array(get(spec, "segments")).cloned().unwrap_or_default();
    let base = address(get(spec, "address"))?;
    let mut result = Vec::new();
    let mut cursor: usize = 0;
    for (index, count) in segments.iter().enumerate() {
        let count = count.as_f64().unwrap_or(0.0) as usize;
        let mut map = Map::new();
        map.insert("id".to_string(), Value::from(format!("program_{index:02}")));
        map.insert(
            "address".to_string(),
            Value::from(hexadecimal(base + cursor as i64 * 4)),
        );
        map.insert(
            "words".to_string(),
            Value::Array(
                values[cursor..cursor + count]
                    .iter()
                    .map(|value| Value::from(hexadecimal(*value)))
                    .collect(),
            ),
        );
        result.push(Value::Object(map));
        cursor += count;
    }
    Ok(result)
}

fn build_programs(value: Option<&Value>, spec: &Value) -> Result<Vec<u8>> {
    let segments = as_array(get(spec, "segments")).cloned().unwrap_or_default();
    let programs = array(value, segments.len(), "object programs")?;
    let base = address(get(spec, "address"))?;
    let mut values: Vec<i64> = Vec::new();
    let mut cursor: usize = 0;
    for (index, segment) in segments.iter().enumerate() {
        let label = format!("object program {index}");
        let program = object(programs.get(index), &label)?;
        exact_keys(program, &["id", "address", "words"], &label)?;
        let expected_id = format!("program_{index:02}");
        let expected_address = hexadecimal(base + cursor as i64 * 4);
        if as_str(program.get("id")) != Some(expected_id.as_str())
            || as_str(program.get("address")) != Some(expected_address.as_str())
        {
            return fail(format!("object program {index} identity differs"));
        }
        let count = segment.as_f64().unwrap_or(f64::NAN);
        let count = if count.is_finite() && count >= 0.0 { count as usize } else { usize::MAX };
        let words = array(program.get("words"), count, &format!("{label} words"))?;
        for (word_index, item) in words.iter().enumerate() {
            values.push(word(Some(item), &format!("{label} word {word_index}"))?);
        }
        cursor += words.len();
    }
    encode_numbers(&values, "command32le")
}

// ---------------------------------------------------------------------------
// ROM slicing
// ---------------------------------------------------------------------------

fn range(rom: &[u8], start: i64, end: i64) -> Result<Vec<u8>> {
    let from = start - ROM_BASE;
    let to = end - ROM_BASE;
    if from < 0 || to <= from || to > rom.len() as i64 {
        return fail("ROM does not contain the early-runtime range");
    }
    Ok(rom[from as usize..to as usize].to_vec())
}

fn slice_region(region: &[u8], region_start: i64, spec: &Value) -> Result<Vec<u8>> {
    let from = (address(get(spec, "address"))? - region_start) as usize;
    let to = (address(get(spec, "end"))? - region_start) as usize;
    Ok(region[from..to].to_vec())
}

fn place(output: &mut [u8], region_start: i64, spec: &Value, data: &[u8]) -> Result<()> {
    let size = address(get(spec, "end"))? - address(get(spec, "address"))?;
    if data.len() as i64 != size {
        return fail(format!("{}: built size differs", entry_id(spec)));
    }
    let at = (address(get(spec, "address"))? - region_start) as usize;
    output[at..at + data.len()].copy_from_slice(data);
    Ok(())
}

// ---------------------------------------------------------------------------
// Source document
// ---------------------------------------------------------------------------

fn source_document(rom: &[u8], catalog: &Value) -> Result<Value> {
    let (early, residual) = canonical_layout(catalog)?;
    let early_data = range(rom, EARLY_RUNTIME_ADDRESS, EARLY_RUNTIME_END)?;
    let residual_data = range(rom, POST_MAP_ADDRESS, POST_MAP_END)?;
    let part = |id: &str| -> Result<Vec<u8>> {
        slice_region(&early_data, EARLY_RUNTIME_ADDRESS, find_entry(early, id, false)?)
    };
    let lookup = part("shared_lookup_storage")?;
    let coefficients = decode_typed(&part("interpolation_coefficients")?, "u32le")?;
    let surface = decode_typed(&part("surface_lookup")?, "u8")?;
    let limits = decode_typed(&part("render_limits")?, "s32le")?;
    let handler_words = decode_typed(&part("object_handlers")?, "pointer32le")?;
    let mut handlers = Vec::with_capacity(handler_words.len());
    for value in handler_words {
        handlers.push(handler_source(value)?);
    }

    let numbers = |values: &[i64]| Value::Array(values.iter().map(|v| Value::from(*v)).collect());

    let mut display = Map::new();
    display.insert("source".to_string(), Value::from(DISPLAY_SOURCE));
    display.insert("encoding".to_string(), Value::from("gba-4bpp"));
    display.insert("tiles".to_string(), Value::from(4));

    let mut shared = Map::new();
    shared.insert("encoding".to_string(), Value::from("u8-backing"));
    shared.insert("values".to_string(), numbers(&decode_typed(&lookup, "u8")?));
    shared.insert("views".to_string(), Value::Array(view_sources(early)));

    let mut coefficient_map = Map::new();
    coefficient_map.insert("encoding".to_string(), Value::from("u32le"));
    coefficient_map.insert("values".to_string(), numbers(&coefficients));

    let mut surface_map = Map::new();
    surface_map.insert("encoding".to_string(), Value::from("u8"));
    surface_map.insert(
        "rows".to_string(),
        Value::Array(vec![
            numbers(&surface[0..16]),
            numbers(&surface[16..32]),
            numbers(&surface[32..48]),
        ]),
    );

    let mut limits_map = Map::new();
    limits_map.insert("encoding".to_string(), Value::from("s32le"));
    limits_map.insert("values".to_string(), numbers(&limits));

    let mut programs_map = Map::new();
    programs_map.insert("encoding".to_string(), Value::from("command32le"));
    programs_map.insert(
        "programs".to_string(),
        Value::Array(program_sources(
            &part("object_programs")?,
            find_entry(early, "object_programs", false)?,
        )?),
    );

    let mut handlers_map = Map::new();
    handlers_map.insert("encoding".to_string(), Value::from("thumb-function-pointer32le"));
    handlers_map.insert("values".to_string(), Value::Array(handlers));

    let mut early_map = Map::new();
    early_map.insert(
        "address".to_string(),
        get(early, "address").cloned().unwrap_or(Value::Null),
    );
    early_map.insert("end".to_string(), get(early, "end").cloned().unwrap_or(Value::Null));
    early_map.insert("display_tiles".to_string(), Value::Object(display));
    early_map.insert("shared_lookup_storage".to_string(), Value::Object(shared));
    early_map.insert(
        "interpolation_coefficients".to_string(),
        Value::Object(coefficient_map),
    );
    early_map.insert("surface_lookup".to_string(), Value::Object(surface_map));
    early_map.insert(
        "unclassified_0801356c".to_string(),
        residual_source(&part("unclassified_0801356c")?, "unclassified 0801356c span")?,
    );
    early_map.insert("render_limits".to_string(), Value::Object(limits_map));
    early_map.insert("object_programs".to_string(), Value::Object(programs_map));
    early_map.insert("object_handlers".to_string(), Value::Object(handlers_map));
    early_map.insert(
        "unclassified_08013724".to_string(),
        residual_source(&part("unclassified_08013724")?, "unclassified 08013724 span")?,
    );

    let mut residual_map = Map::new();
    residual_map.insert(
        "address".to_string(),
        get(residual, "address").cloned().unwrap_or(Value::Null),
    );
    residual_map.insert("end".to_string(), get(residual, "end").cloned().unwrap_or(Value::Null));
    residual_map.insert(
        "unreferenced_storage".to_string(),
        residual_source(&residual_data, "post-map residual")?,
    );

    let mut document = Map::new();
    document.insert("format".to_string(), Value::from(1));
    document.insert("kind".to_string(), Value::from("golden-sun-early-runtime-data"));
    document.insert(
        "catalog_format".to_string(),
        get(catalog, "format").cloned().unwrap_or(Value::Null),
    );
    document.insert("source_bytes".to_string(), Value::from(EARLY_RUNTIME_SOURCE_BYTES));
    document.insert("early_runtime_tables".to_string(), Value::Object(early_map));
    document.insert("post_map_load_residual".to_string(), Value::Object(residual_map));
    Ok(Value::Object(document))
}

fn source_index(path: &str, catalog: &Value) -> Result<Value> {
    let text = read_utf8(path)?;
    let parsed = parse_json(&text)?;
    let source = object(Some(&parsed), "early-runtime source")?.clone();
    if !canonical_json::is_canonical_json_text(&text, &parsed) {
        return fail("early-runtime source is not canonical JSON");
    }
    exact_keys(
        &source,
        &[
            "format",
            "kind",
            "catalog_format",
            "source_bytes",
            "early_runtime_tables",
            "post_map_load_residual",
        ],
        "early-runtime source",
    )?;
    if source.get("format") != Some(&Value::from(1))
        || as_str(source.get("kind")) != Some("golden-sun-early-runtime-data")
        || source.get("catalog_format") != get(catalog, "format")
        || source.get("source_bytes") != Some(&Value::from(EARLY_RUNTIME_SOURCE_BYTES))
    {
        return fail("unsupported early-runtime source");
    }
    Ok(Value::Object(source))
}

// ---------------------------------------------------------------------------
// Package layout
// ---------------------------------------------------------------------------

fn strip_index_suffix(name: &str) -> String {
    match name.strip_suffix("index.json") {
        Some(prefix) => prefix.to_string(),
        None => name.to_string(),
    }
}

fn realpath(path: &str) -> Result<String> {
    match std::fs::canonicalize(path) {
        Ok(resolved) => Ok(resolved.to_string_lossy().into_owned()),
        Err(error) => Err(system_error("realpath", path, &error)),
    }
}

fn child(index_path: &str, name: Option<&Value>) -> Result<String> {
    if as_str(name) != Some(DISPLAY_SOURCE) {
        return fail("display tile source name differs");
    }
    let prefix = strip_index_suffix(&nodepath::basename(index_path));
    let root = realpath(&nodepath::dirname(index_path))?;
    let wanted = format!("{prefix}{DISPLAY_SOURCE}");
    let path = realpath(&nodepath::resolve(&[&root, &wanted]))?;
    if nodepath::relative(&root, &path) != wanted {
        return fail("display tile source escaped its package");
    }
    Ok(path)
}

fn is_symlink(path: &str) -> Result<bool> {
    match std::fs::symlink_metadata(path) {
        Ok(meta) => Ok(meta.file_type().is_symlink()),
        Err(error) => Err(system_error("lstat", path, &error)),
    }
}

// `assets/data/` is a flat, shared directory: filtering by prefix alone would
// also catch this package's own catalog sibling (early_runtime_data.json),
// which is a real, expected neighbor rather than a stray package file.
fn checked_package_files(index_path: &str, catalog_path: &str) -> Result<()> {
    let root = nodepath::dirname(index_path);
    let prefix = strip_index_suffix(&nodepath::basename(index_path));
    let mut expected = vec![format!("{prefix}{DISPLAY_SOURCE}"), format!("{prefix}index.json")];
    js_sort(&mut expected);
    let catalog_name = nodepath::basename(catalog_path);
    let entries = match std::fs::read_dir(&root) {
        Ok(entries) => entries,
        Err(error) => return Err(system_error("scandir", &root, &error)),
    };
    let mut names: Vec<String> = Vec::new();
    for entry in entries.flatten() {
        let name = entry.file_name().to_string_lossy().into_owned();
        if name.starts_with(&prefix) && name != catalog_name {
            names.push(name);
        }
    }
    js_sort(&mut names);
    if names != expected {
        return fail("early-runtime package contains unexpected files");
    }
    for name in &names {
        if is_symlink(&nodepath::join(&[&root, name]))? {
            return fail("early-runtime package contains a symbolic link");
        }
    }
    Ok(())
}

// ---------------------------------------------------------------------------
// Build
// ---------------------------------------------------------------------------

#[derive(Debug, Clone)]
pub struct EarlyRuntimeBuild {
    /// Insertion-ordered, like the JavaScript `Map` it replaces: the early
    /// region first, then the post-map residual. `writeBuild` walks it in that
    /// order and the manifest records the order, so a `HashMap` would be wrong.
    pub regions: Vec<(i64, Vec<u8>)>,
    pub source_bytes: i64,
}

pub fn build_early_runtime_data(index_path: &str, catalog_path: &str) -> Result<EarlyRuntimeBuild> {
    let catalog = load_catalog(catalog_path)?;
    let (early, residual) = canonical_layout(&catalog)?;
    let source = source_index(index_path, &catalog)?;
    checked_package_files(index_path, catalog_path)?;
    let early_source = object(get(&source, "early_runtime_tables"), "early-runtime tables")?.clone();
    exact_keys(
        &early_source,
        &[
            "address",
            "end",
            "display_tiles",
            "shared_lookup_storage",
            "interpolation_coefficients",
            "surface_lookup",
            "unclassified_0801356c",
            "render_limits",
            "object_programs",
            "object_handlers",
            "unclassified_08013724",
        ],
        "early-runtime tables",
    )?;
    if early_source.get("address") != get(early, "address") || early_source.get("end") != get(early, "end") {
        return fail("early-runtime table extent differs");
    }
    let mut output = vec![0u8; (EARLY_RUNTIME_END - EARLY_RUNTIME_ADDRESS) as usize];

    let display = object(early_source.get("display_tiles"), "display tiles")?;
    exact_keys(display, &["source", "encoding", "tiles"], "display tiles")?;
    if as_str(display.get("encoding")) != Some("gba-4bpp") || display.get("tiles") != Some(&Value::from(4)) {
        return fail("display tile source metadata differs");
    }
    let tiles = read_display_tiles(&child(index_path, display.get("source"))?)?;
    place(
        &mut output,
        EARLY_RUNTIME_ADDRESS,
        find_entry(early, "display_tiles", false)?,
        &tiles,
    )?;

    let lookup = object(early_source.get("shared_lookup_storage"), "shared lookup storage")?;
    exact_keys(lookup, &["encoding", "values", "views"], "shared lookup storage")?;
    if as_str(lookup.get("encoding")) != Some("u8-backing") {
        return fail("shared lookup backing encoding differs");
    }
    validate_view_sources(lookup.get("views"), early)?;
    let lookup_spec = find_entry(early, "shared_lookup_storage", false)?;
    let lookup_size = (address(get(lookup_spec, "end"))? - address(get(lookup_spec, "address"))?) as usize;
    let lookup_values = flat_values(lookup.get("values"), lookup_size, "u8", "shared lookup values")?;
    place(
        &mut output,
        EARLY_RUNTIME_ADDRESS,
        lookup_spec,
        &encode_numbers(&lookup_values, "u8")?,
    )?;

    let coefficients = object(
        early_source.get("interpolation_coefficients"),
        "interpolation coefficients",
    )?;
    exact_keys(coefficients, &["encoding", "values"], "interpolation coefficients")?;
    if as_str(coefficients.get("encoding")) != Some("u32le") {
        return fail("interpolation coefficient encoding differs");
    }
    let coefficient_values = flat_values(coefficients.get("values"), 16, "u32le", "interpolation coefficients")?;
    place(
        &mut output,
        EARLY_RUNTIME_ADDRESS,
        find_entry(early, "interpolation_coefficients", false)?,
        &encode_numbers(&coefficient_values, "u32le")?,
    )?;

    let surface = object(early_source.get("surface_lookup"), "surface lookup")?;
    exact_keys(surface, &["encoding", "rows"], "surface lookup")?;
    if as_str(surface.get("encoding")) != Some("u8") {
        return fail("surface lookup encoding differs");
    }
    let surface_values = rows(surface.get("rows"), 3, 16, "u8", "surface lookup")?;
    place(
        &mut output,
        EARLY_RUNTIME_ADDRESS,
        find_entry(early, "surface_lookup", false)?,
        &encode_numbers(&surface_values, "u8")?,
    )?;

    let unresolved_a = find_entry(early, "unclassified_0801356c", false)?;
    let size_a = (address(get(unresolved_a, "end"))? - address(get(unresolved_a, "address"))?) as usize;
    let residual_a = build_residual(
        early_source.get("unclassified_0801356c"),
        size_a,
        &entry_id(unresolved_a),
    )?;
    place(&mut output, EARLY_RUNTIME_ADDRESS, unresolved_a, &residual_a)?;

    let limits = object(early_source.get("render_limits"), "render limits")?;
    exact_keys(limits, &["encoding", "values"], "render limits")?;
    if as_str(limits.get("encoding")) != Some("s32le") {
        return fail("render-limit encoding differs");
    }
    let limit_values = flat_values(limits.get("values"), 2, "s32le", "render limits")?;
    place(
        &mut output,
        EARLY_RUNTIME_ADDRESS,
        find_entry(early, "render_limits", false)?,
        &encode_numbers(&limit_values, "s32le")?,
    )?;

    let programs = object(early_source.get("object_programs"), "object programs")?;
    exact_keys(programs, &["encoding", "programs"], "object programs")?;
    if as_str(programs.get("encoding")) != Some("command32le") {
        return fail("object-program encoding differs");
    }
    let program_spec = find_entry(early, "object_programs", false)?;
    let program_data = build_programs(programs.get("programs"), program_spec)?;
    place(&mut output, EARLY_RUNTIME_ADDRESS, program_spec, &program_data)?;

    let handlers = object(early_source.get("object_handlers"), "object handlers")?;
    exact_keys(handlers, &["encoding", "values"], "object handlers")?;
    if as_str(handlers.get("encoding")) != Some("thumb-function-pointer32le") {
        return fail("object-handler encoding differs");
    }
    let handler_items = array(handlers.get("values"), 64, "object handlers")?;
    let mut handler_values: Vec<i64> = Vec::with_capacity(64);
    for (index, item) in handler_items.iter().enumerate() {
        handler_values.push(handler_value(Some(item), &format!("object handler {index}"))?);
    }
    place(
        &mut output,
        EARLY_RUNTIME_ADDRESS,
        find_entry(early, "object_handlers", false)?,
        &encode_numbers(&handler_values, "pointer32le")?,
    )?;

    let unresolved_b = find_entry(early, "unclassified_08013724", false)?;
    let size_b = (address(get(unresolved_b, "end"))? - address(get(unresolved_b, "address"))?) as usize;
    let residual_b = build_residual(
        early_source.get("unclassified_08013724"),
        size_b,
        &entry_id(unresolved_b),
    )?;
    place(&mut output, EARLY_RUNTIME_ADDRESS, unresolved_b, &residual_b)?;

    let residual_src = object(get(&source, "post_map_load_residual"), "post-map residual")?.clone();
    exact_keys(
        &residual_src,
        &["address", "end", "unreferenced_storage"],
        "post-map residual",
    )?;
    if residual_src.get("address") != get(residual, "address")
        || residual_src.get("end") != get(residual, "end")
    {
        return fail("post-map residual extent differs");
    }
    let residual_output = build_residual(
        residual_src.get("unreferenced_storage"),
        (POST_MAP_END - POST_MAP_ADDRESS) as usize,
        "post-map residual",
    )?;
    let source_bytes = (output.len() + residual_output.len()) as i64;
    Ok(EarlyRuntimeBuild {
        regions: vec![
            (EARLY_RUNTIME_ADDRESS, output),
            (POST_MAP_ADDRESS, residual_output),
        ],
        source_bytes,
    })
}

fn compare_build(rom: &[u8], built: &EarlyRuntimeBuild) -> Result<()> {
    if built.source_bytes != EARLY_RUNTIME_SOURCE_BYTES || built.regions.len() != 2 {
        return fail("early-runtime build extent differs");
    }
    for (start, data) in &built.regions {
        if *data != range(rom, *start, start + data.len() as i64)? {
            return fail(format!("{} differs from ROM", hexadecimal(*start)));
        }
    }
    Ok(())
}

// ---------------------------------------------------------------------------
// Filesystem helpers
// ---------------------------------------------------------------------------

fn read_bytes(path: &str) -> Result<Vec<u8>> {
    match std::fs::read(path) {
        Ok(bytes) => Ok(bytes),
        Err(error) => Err(system_error("open", path, &error)),
    }
}

fn write_file(path: &str, data: &[u8]) -> Result<()> {
    std::fs::write(path, data).map_err(|error| Error(format!("failed to write '{path}': {error}")))
}

fn mkdir_recursive(path: &str) -> Result<()> {
    std::fs::create_dir_all(path).map_err(|error| Error(format!("failed to create '{path}': {error}")))
}

/// `fs.mkdtempSync(prefix)` -- six random characters appended to the prefix,
/// created with `O_EXCL` semantics by retrying until `create_dir` succeeds.
fn mkdtemp(prefix: &str) -> Result<String> {
    use std::time::{SystemTime, UNIX_EPOCH};
    const ALPHABET: &[u8] = b"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    let mut seed = SystemTime::now()
        .duration_since(UNIX_EPOCH)
        .map(|elapsed| elapsed.as_nanos() as u64)
        .unwrap_or(0)
        ^ (std::process::id() as u64) << 32;
    for _ in 0..1000 {
        let mut suffix = String::new();
        for _ in 0..6 {
            seed = seed.wrapping_mul(6_364_136_223_846_793_005).wrapping_add(1_442_695_040_888_963_407);
            suffix.push(ALPHABET[(seed >> 33) as usize % ALPHABET.len()] as char);
        }
        let candidate = format!("{prefix}{suffix}");
        if std::fs::create_dir(&candidate).is_ok() {
            return Ok(candidate);
        }
    }
    fail(format!("could not create a temporary directory at '{prefix}'"))
}

fn exists(path: &str) -> bool {
    std::fs::symlink_metadata(path).is_ok()
}

fn tmpdir() -> String {
    let raw = std::env::var("TMPDIR").unwrap_or_else(|_| "/tmp".to_string());
    let trimmed = raw.trim_end_matches('/');
    if trimmed.is_empty() {
        "/".to_string()
    } else {
        trimmed.to_string()
    }
}

// ---------------------------------------------------------------------------
// Export, verify, build outputs
// ---------------------------------------------------------------------------

fn valid_destination(path: &str, catalog_path: &str) -> Result<()> {
    let meta = std::fs::symlink_metadata(path).map_err(|error| system_error("lstat", path, &error))?;
    if meta.file_type().is_symlink() || !meta.is_dir() {
        return fail("early-runtime export destination must be a directory");
    }
    build_early_runtime_data(&nodepath::join(&[path, "index.json"]), catalog_path)?;
    Ok(())
}

fn write_package(rom: &[u8], directory: &str, catalog: &Value, catalog_path: &str) -> Result<()> {
    mkdir_recursive(directory)?;
    let early_data = range(rom, EARLY_RUNTIME_ADDRESS, EARLY_RUNTIME_END)?;
    let (early, _) = canonical_layout(catalog)?;
    let tiles = slice_region(
        &early_data,
        EARLY_RUNTIME_ADDRESS,
        find_entry(early, "display_tiles", false)?,
    )?;
    write_display_tiles(&tiles, &nodepath::join(&[directory, DISPLAY_SOURCE]))?;
    let index_path = nodepath::join(&[directory, "index.json"]);
    write_file(&index_path, pretty(&source_document(rom, catalog)?).as_bytes())?;
    compare_build(rom, &build_early_runtime_data(&index_path, catalog_path)?)
}

pub fn export_early_runtime_data(
    rom: &[u8],
    directory: &str,
    rom_path: Option<&str>,
    catalog_path: &str,
) -> Result<()> {
    let catalog = load_catalog(catalog_path)?;
    let requested = nodepath::resolve(&[directory]);
    let requested_parent = nodepath::dirname(&requested);
    if requested == requested_parent {
        return fail("early-runtime export requires a dedicated directory");
    }
    mkdir_recursive(&requested_parent)?;
    let parent = realpath(&requested_parent)?;
    let destination = nodepath::join(&[&parent, &nodepath::basename(&requested)]);
    if let Some(rom_path) = rom_path {
        let source = realpath(rom_path)?;
        let relation = nodepath::relative(&destination, &source);
        if relation.is_empty() || (!relation.starts_with("..") && !nodepath::is_absolute(&relation)) {
            return fail("early-runtime export directory must not contain its input ROM");
        }
    }
    if exists(&destination) {
        valid_destination(&destination, catalog_path)?;
    }
    let transaction = mkdtemp(&nodepath::join(&[&parent, ".early-runtime-export-"]))?;
    let staged = nodepath::join(&[&transaction, "new"]);
    let previous = nodepath::join(&[&transaction, "previous"]);
    let mut installed = false;

    let mut outcome: Result<()> = (|| {
        write_package(rom, &staged, &catalog, catalog_path)?;
        if exists(&destination) {
            std::fs::rename(&destination, &previous)
                .map_err(|error| Error(format!("failed to stage the previous package: {error}")))?;
        }
        match std::fs::rename(&staged, &destination) {
            Ok(()) => {
                installed = true;
                Ok(())
            }
            Err(error) => {
                if exists(&previous) {
                    let _ = std::fs::rename(&previous, &destination);
                }
                fail(format!("failed to install the new package: {error}"))
            }
        }
    })();

    if !installed && exists(&previous) && !exists(&destination) {
        if let Err(error) = std::fs::rename(&previous, &destination) {
            if outcome.is_ok() {
                outcome = fail(format!("failed to restore the previous package: {error}"));
            }
        }
    }
    let _ = std::fs::remove_dir_all(&transaction);
    outcome
}

pub fn verify_early_runtime_data(rom: &[u8], index_path: &str, catalog_path: &str) -> Result<()> {
    compare_build(rom, &build_early_runtime_data(index_path, catalog_path)?)
}

fn private_output(path: &str) -> bool {
    let target = nodepath::resolve(&[path]);
    let roots = [
        nodepath::resolve(&[&repo_root(), "out"]),
        nodepath::resolve(&[&tmpdir()]),
        "/private/tmp".to_string(),
    ];
    roots.iter().any(|root| {
        let relation = nodepath::relative(root, &target);
        !relation.is_empty() && !relation.starts_with("..") && !nodepath::is_absolute(&relation)
    })
}

pub fn write_build(index_path: &str, directory: &str, catalog_path: &str) -> Result<()> {
    if !private_output(directory) {
        return fail("early-runtime build output must stay under out/ or a temporary directory");
    }
    let built = build_early_runtime_data(index_path, catalog_path)?;
    mkdir_recursive(directory)?;
    let mut outputs: Vec<Value> = Vec::new();
    for (start, data) in &built.regions {
        let name = format!("{:08x}.bin", *start as u32);
        write_file(&nodepath::join(&[directory, &name]), data)?;
        let mut entry = Map::new();
        entry.insert("address".to_string(), Value::from(hexadecimal(*start)));
        entry.insert("size".to_string(), Value::from(data.len() as i64));
        entry.insert("output".to_string(), Value::from(name));
        outputs.push(Value::Object(entry));
    }
    let mut manifest = Map::new();
    manifest.insert("format".to_string(), Value::from(1));
    manifest.insert("kind".to_string(), Value::from("golden-sun-early-runtime-build"));
    manifest.insert("source_bytes".to_string(), Value::from(built.source_bytes));
    manifest.insert("regions".to_string(), Value::Array(outputs));
    write_file(
        &nodepath::join(&[directory, "manifest.json"]),
        pretty(&Value::Object(manifest)).as_bytes(),
    )
}

// ---------------------------------------------------------------------------
// Self-test
// ---------------------------------------------------------------------------

fn synthetic_rom(catalog: &Value) -> Result<Vec<u8>> {
    let mut rom = vec![0xffu8; (POST_MAP_END - ROM_BASE) as usize];
    let mut early = vec![0u8; (EARLY_RUNTIME_END - EARLY_RUNTIME_ADDRESS) as usize];
    for (index, byte) in early.iter_mut().enumerate() {
        *byte = ((index as i64 * 73 + (index as i64 >> 2) * 19 + 11) & 0xff) as u8;
    }
    let (region, _) = canonical_layout(catalog)?;
    let fill_a = find_entry(region, "unclassified_0801356c", false)?;
    let fill_b = find_entry(region, "unclassified_08013724", false)?;
    let span = |spec: &Value| -> Result<(usize, usize)> {
        Ok((
            (address(get(spec, "address"))? - EARLY_RUNTIME_ADDRESS) as usize,
            (address(get(spec, "end"))? - EARLY_RUNTIME_ADDRESS) as usize,
        ))
    };
    let (from_a, to_a) = span(fill_a)?;
    early[from_a..to_a].fill(0);
    let (from_b, to_b) = span(fill_b)?;
    early[from_b..to_b].fill(0xff);
    let handlers = find_entry(region, "object_handlers", false)?;
    let (from_h, _) = span(handlers)?;
    for index in 0..64usize {
        let word: u32 = if index % 5 == 0 {
            0
        } else {
            0x0800_0101 + index as u32 * 2
        };
        early[from_h + index * 4..from_h + index * 4 + 4].copy_from_slice(&word.to_le_bytes());
    }
    let at = (EARLY_RUNTIME_ADDRESS - ROM_BASE) as usize;
    rom[at..at + early.len()].copy_from_slice(&early);
    rom[(POST_MAP_ADDRESS - ROM_BASE) as usize..(POST_MAP_END - ROM_BASE) as usize].fill(0xff);
    Ok(rom)
}

fn rejects<T>(action: impl FnOnce() -> Result<T>) -> bool {
    action().is_err()
}

/// One adversarial edit applied to a parsed copy of the exported source.
type Mutation = Box<dyn FnOnce(&mut Value)>;

/// Mutate a copy of the source document and confirm the build refuses it.
fn mutate(
    index_path: &str,
    source_text: &str,
    catalog_path: &str,
    change: impl FnOnce(&mut Value),
) -> Result<bool> {
    let mut value = parse_json(source_text)?;
    change(&mut value);
    write_file(index_path, pretty(&value).as_bytes())?;
    let rejected = rejects(|| build_early_runtime_data(index_path, catalog_path));
    write_file(index_path, source_text.as_bytes())?;
    Ok(rejected)
}

fn at<'a>(value: &'a mut Value, path: &[&str]) -> &'a mut Value {
    let mut cursor = value;
    for key in path {
        cursor = match key.parse::<usize>() {
            Ok(index) => cursor
                .as_array_mut()
                .and_then(|items| items.get_mut(index))
                .expect("the self-test fixture has this element"),
            Err(_) => cursor
                .as_object_mut()
                .and_then(|map| map.get_mut(*key))
                .expect("the self-test fixture has this key"),
        };
    }
    cursor
}

pub fn self_test() -> Result<String> {
    let catalog_path = default_catalog_path();
    let catalog = load_catalog(&catalog_path)?;
    let temporary = mkdtemp(&nodepath::join(&[&tmpdir(), "alchemy-early-runtime-test-"]))?;
    let outcome = self_test_body(&catalog, &catalog_path, &temporary);
    let _ = std::fs::remove_dir_all(&temporary);
    outcome?;
    Ok(format!(
        "self-test=ok source_bytes={EARLY_RUNTIME_SOURCE_BYTES} adversarial=18 transaction=atomic"
    ))
}

fn self_test_body(catalog: &Value, catalog_path: &str, temporary: &str) -> Result<()> {
    let rom = synthetic_rom(catalog)?;
    let rom_path = nodepath::join(&[temporary, "gs1-en.gba"]);
    let destination = nodepath::join(&[temporary, "source"]);
    write_file(&rom_path, &rom)?;
    export_early_runtime_data(&rom, &destination, Some(&rom_path), catalog_path)?;
    let index_path = nodepath::join(&[&destination, "index.json"]);
    verify_early_runtime_data(&rom, &index_path, catalog_path)?;
    let built = build_early_runtime_data(&index_path, catalog_path)?;
    if built.source_bytes != EARLY_RUNTIME_SOURCE_BYTES {
        return fail("synthetic source-byte count differs");
    }
    let build_directory = nodepath::join(&[temporary, "build"]);
    write_build(&index_path, &build_directory, catalog_path)?;
    if !exists(&nodepath::join(&[&build_directory, "08012f20.bin"]))
        || !exists(&nodepath::join(&[&build_directory, "0801403c.bin"]))
    {
        return fail("synthetic build output differs");
    }

    let source_text = read_utf8(&index_path)?;
    let display_path = nodepath::join(&[&destination, DISPLAY_SOURCE]);
    let image = read_bytes(&display_path)?;
    let mut adversarial = 0usize;

    let mutations: Vec<Mutation> = vec![
        Box::new(|value: &mut Value| {
            value
                .as_object_mut()
                .expect("object")
                .insert("extra".to_string(), Value::Bool(true));
        }),
        Box::new(|value: &mut Value| {
            at(value, &["early_runtime_tables", "shared_lookup_storage", "values"])
                .as_array_mut()
                .expect("array")
                .pop();
        }),
        Box::new(|value: &mut Value| {
            *at(value, &["early_runtime_tables", "shared_lookup_storage", "values", "0"]) = Value::from(256);
        }),
        Box::new(|value: &mut Value| {
            *at(value, &["early_runtime_tables", "shared_lookup_storage", "views", "0", "end"]) =
                Value::from("0x0801349f");
        }),
        Box::new(|value: &mut Value| {
            at(value, &["early_runtime_tables", "surface_lookup", "rows", "0"])
                .as_array_mut()
                .expect("array")
                .push(Value::from(0));
        }),
        Box::new(|value: &mut Value| {
            *at(value, &["early_runtime_tables", "unclassified_0801356c", "value"]) = Value::from("0x7f");
        }),
        Box::new(|value: &mut Value| {
            *at(
                value,
                &["early_runtime_tables", "object_programs", "programs", "0", "words", "0"],
            ) = Value::from("0X00000000");
        }),
        Box::new(|value: &mut Value| {
            *at(value, &["early_runtime_tables", "object_handlers", "values", "1"]) =
                Value::from("0x08000100");
        }),
        Box::new(|value: &mut Value| {
            *at(value, &["post_map_load_residual", "unreferenced_storage", "kind"]) = Value::from("bytes");
        }),
    ];
    for change in mutations {
        if mutate(&index_path, &source_text, catalog_path, change)? {
            adversarial += 1;
        }
    }

    write_file(&index_path, format!("{source_text}\n").as_bytes())?;
    if rejects(|| build_early_runtime_data(&index_path, catalog_path)) {
        adversarial += 1;
    }
    write_file(&index_path, source_text.as_bytes())?;

    let mut longer = image.clone();
    longer.push(0);
    write_file(&display_path, &longer)?;
    if rejects(|| build_early_runtime_data(&index_path, catalog_path)) {
        adversarial += 1;
    }
    write_file(&display_path, &image)?;

    let hidden = nodepath::join(&[&destination, "hidden.bin"]);
    write_file(&hidden, &[0u8])?;
    if rejects(|| build_early_runtime_data(&index_path, catalog_path)) {
        adversarial += 1;
    }
    std::fs::remove_file(&hidden).map_err(|error| Error(format!("failed to remove the probe file: {error}")))?;

    let outside = nodepath::join(&[temporary, "outside.png"]);
    write_file(&outside, &image)?;
    std::fs::remove_file(&display_path).map_err(|error| Error(format!("failed to unlink: {error}")))?;
    std::os::unix::fs::symlink(&outside, &display_path)
        .map_err(|error| Error(format!("failed to symlink: {error}")))?;
    if rejects(|| build_early_runtime_data(&index_path, catalog_path)) {
        adversarial += 1;
    }
    std::fs::remove_file(&display_path).map_err(|error| Error(format!("failed to unlink: {error}")))?;
    write_file(&display_path, &image)?;

    if rejects(|| encode_typed(&[Value::from(256)], "u8")) {
        adversarial += 1;
    }
    if rejects(|| encode_typed(&[Value::from(-32769)], "s16le")) {
        adversarial += 1;
    }

    let mut bad_catalog = catalog.clone();
    *at(&mut bad_catalog, &["regions", "0", "views", "0", "end"]) = Value::from("0x080134ff");
    if rejects(|| validate_catalog(&bad_catalog)) {
        adversarial += 1;
    }
    let mut bad_segments = catalog.clone();
    {
        let partitions = at(&mut bad_segments, &["regions", "0", "partitions"])
            .as_array_mut()
            .expect("array");
        let entry = partitions
            .iter_mut()
            .find(|item| item.get("id").and_then(Value::as_str) == Some("object_programs"))
            .expect("the canonical catalog has object_programs");
        let first = entry
            .get_mut("segments")
            .and_then(Value::as_array_mut)
            .and_then(|items| items.first_mut())
            .expect("object_programs carries segments");
        *first = Value::from(first.as_i64().expect("integer segment") + 1);
    }
    if rejects(|| validate_catalog(&bad_segments)) {
        adversarial += 1;
    }

    let preserved_index = read_bytes(&index_path)?;
    let preserved_image = read_bytes(&display_path)?;
    let mut bad_rom = rom.clone();
    // A malformed handler pointer is rejected before the transactional export
    // can replace an existing package. Ordinary residual bytes are valid
    // byte-value sources and must no longer be used as an artificial failure.
    let handler_at = (0x0801_3624 - ROM_BASE) as usize;
    bad_rom[handler_at..handler_at + 4].copy_from_slice(&0x0800_0000u32.to_le_bytes());
    if rejects(|| export_early_runtime_data(&bad_rom, &destination, Some(&rom_path), catalog_path)) {
        adversarial += 1;
    }
    if read_bytes(&index_path)? != preserved_index || read_bytes(&display_path)? != preserved_image {
        return fail("failed transactional export changed the installed package");
    }
    if adversarial != 18 {
        return fail(format!("adversarial validation count differs: {adversarial}"));
    }

    let mut changed_rom = rom.clone();
    changed_rom[(EARLY_RUNTIME_ADDRESS - ROM_BASE) as usize] ^= 0x11;
    export_early_runtime_data(&changed_rom, &destination, Some(&rom_path), catalog_path)?;
    verify_early_runtime_data(
        &changed_rom,
        &nodepath::join(&[&destination, "index.json"]),
        catalog_path,
    )?;
    if read_bytes(&display_path)? == preserved_image {
        return fail("successful transactional export did not replace the package");
    }
    Ok(())
}

#[cfg(test)]
mod tests;
