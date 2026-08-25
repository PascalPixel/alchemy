pub mod cli;
use serde_json::{Map, Value};
use std::path::Path;
fn get<'a>(value: &'a Value, key: &str) -> Option<&'a Value> {
    value.as_object().and_then(|object| object.get(key))
}
fn as_str(value: Option<&Value>) -> Option<&str> {
    value.and_then(Value::as_str)
}
fn as_number(value: Option<&Value>) -> Option<f64> {
    value.and_then(Value::as_f64)
}
fn as_array(value: Option<&Value>) -> Option<&Vec<Value>> {
    value.and_then(Value::as_array)
}
fn is_plain_object(value: Option<&Value>) -> bool {
    matches!(value, Some(Value::Object(_)))
}
fn normalize_numbers(value: Value) -> Value {
    match value {
        Value::Number(number) if number.is_f64() => match number.as_f64() {
            Some(raw) if raw.fract() == 0.0 && raw.abs() <= 9_007_199_254_740_992.0 => Value::from(raw as i64),
            _ => Value::Number(number),
        },
        Value::Array(items) => Value::Array(items.into_iter().map(normalize_numbers).collect()),
        Value::Object(entries) => Value::Object(entries.into_iter().map(|(key, value)| (key, normalize_numbers(value))).collect()),
        other => other,
    }
}
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
pub const ROM_BASE: i64 = 0x0800_0000;
pub const EARLY_RUNTIME_ADDRESS: i64 = 0x0801_2f20;
pub const EARLY_RUNTIME_END: i64 = 0x0801_3784;
pub const POST_MAP_ADDRESS: i64 = 0x0801_403c;
pub const POST_MAP_END: i64 = 0x0801_5000;
pub const EARLY_RUNTIME_SOURCE_BYTES: i64 = EARLY_RUNTIME_END - EARLY_RUNTIME_ADDRESS + POST_MAP_END - POST_MAP_ADDRESS;
const DISPLAY_SOURCE: &str = "display.4bpp.png";
const DISPLAY_WIDTH: usize = 32;
const DISPLAY_HEIGHT: usize = 8;
pub type Rgb = [u8; 3];
fn palette() -> Vec<Rgb> {
    (0..16u8).map(|index| [index * 8, index * 8, index * 8]).collect()
}
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
    manifest.parent().and_then(Path::parent).expect("the crate lives two levels below the repository root").to_string_lossy().into_owned()
}
pub fn default_catalog_path() -> String {
    Path::new(&repo_root()).join("games/gs1/assets/data/early_runtime_data.json").to_string_lossy().into_owned()
}
fn is_rom_address(text: &str) -> bool {
    text.len() == 10 && text.starts_with("0x080") && text.as_bytes()[5..].iter().all(u8::is_ascii_hexdigit) && text.as_bytes()[5..].iter().all(|byte| !byte.is_ascii_uppercase())
}
fn is_canonical_word(text: &str) -> bool {
    text.len() == 10 && text.starts_with("0x") && text.as_bytes()[2..].iter().all(|byte| byte.is_ascii_digit() || (b'a'..=b'f').contains(byte))
}
fn is_thumb_symbol(text: &str) -> bool {
    let bytes = text.as_bytes();
    text.len() == 13 && text.starts_with("Func_080") && (b'0'..=b'7').contains(&bytes[8]) && bytes[9..].iter().all(|byte| byte.is_ascii_digit() || (b'a'..=b'f').contains(byte))
}
fn hexadecimal(value: i64) -> String {
    format!("0x{:08x}", value as u32)
}
fn address_of(value: Option<&Value>, label: &str) -> Result<i64> {
    match as_str(value) {
        Some(text) if is_rom_address(text) => Ok(i64::from_str_radix(&text[2..], 16).expect("ten canonical hex digits")),
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
    actual.sort();
    let mut expected: Vec<String> = keys.iter().map(|key| (*key).to_string()).collect();
    expected.sort();
    if actual != expected {
        return fail(format!("{label} fields differ"));
    }
    Ok(())
}
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
fn entry_id(entry: &Value) -> String {
    as_str(get(entry, "id")).unwrap_or("unknown").to_string()
}
fn find_entry<'a>(region: &'a Value, id: &str, view: bool) -> Result<&'a Value> {
    let list = get(region, if view { "views" } else { "partitions" }).and_then(Value::as_array).map(|items| items.as_slice()).unwrap_or(&[]);
    match list.iter().find(|item| as_str(get(item, "id")) == Some(id)) {
        Some(found) => Ok(found),
        None => fail(format!("{}: missing {id}", as_str(get(region, "id")).unwrap_or("unknown"))),
    }
}
fn find_region<'a>(catalog: &'a Value, id: &str) -> Option<&'a Value> {
    get(catalog, "regions").and_then(Value::as_array).and_then(|regions| regions.iter().find(|region| as_str(get(region, "id")) == Some(id)))
}
fn canonical_layout(catalog: &Value) -> Result<(&Value, &Value)> {
    let early = find_region(catalog, "early_runtime_tables").ok_or_else(|| Error("missing early-runtime region".into()))?;
    let residual = find_region(catalog, "post_map_load_residual").ok_or_else(|| Error("missing post-map region".into()))?;
    Ok((early, residual))
}
fn read_utf8(path: &str) -> Result<String> {
    std::fs::read_to_string(path).map_err(|error| Error(error.to_string()))
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
    Ok(catalog)
}
fn value_range(encoding: &str) -> Result<(i64, i64)> {
    match encoding {
        "u8" => Ok((0, 0xff)),
        "s16le" => Ok((-0x8000, 0x7fff)),
        "s32le" => Ok((-0x8000_0000, 0x7fff_ffff)),
        "u32le" | "pointer32le" | "command32le" => Ok((0, 0xffff_ffff)),
        other => fail(format!("cannot encode {other}")),
    }
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
fn rows(value: Option<&Value>, row_count: usize, columns: usize, encoding: &str, label: &str) -> Result<Vec<i64>> {
    let items = array(value, row_count, label)?;
    let mut result = Vec::new();
    for (index, row) in items.iter().enumerate() {
        result.extend(flat_values(Some(row), columns, encoding, &format!("{label} row {index}"))?);
    }
    Ok(result)
}
fn read_bytes(path: &str) -> Result<Vec<u8>> {
    std::fs::read(path).map_err(|error| Error(error.to_string()))
}
fn read_display_tiles(path: &str) -> Result<Vec<u8>> {
    let encoded = read_bytes(path)?;
    let image = import_asset::indexed_png(&encoded).map_err(|error| Error(error.0))?;
    let expected_palette = palette();
    let layout_ok = image.width as usize == DISPLAY_WIDTH && image.height as usize == DISPLAY_HEIGHT && image.palette == expected_palette && image.pixels.iter().all(|value| *value < 16);
    if !layout_ok {
        return fail("display tile PNG layout differs");
    }
    let pixels: Vec<u8> = image.pixels.iter().map(|value| *value as u8).collect();
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
        if &Value::Object(item.clone()) != want {
            return fail(format!("shared lookup view {index} differs from catalog"));
        }
    }
    Ok(())
}
fn word(value: Option<&Value>, label: &str) -> Result<i64> {
    match as_str(value) {
        Some(text) if is_canonical_word(text) => Ok(i64::from_str_radix(&text[2..], 16).expect("eight canonical hex digits")),
        _ => fail(format!("{label} is not a canonical word")),
    }
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
        if as_str(program.get("id")) != Some(expected_id.as_str()) || as_str(program.get("address")) != Some(expected_address.as_str()) {
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
fn source_index(path: &str, catalog: &Value) -> Result<Value> {
    let text = read_utf8(path)?;
    let parsed = parse_json(&text)?;
    let source = object(Some(&parsed), "early-runtime source")?.clone();
    if !canonical_json::is_canonical_json_text(&text, &parsed) {
        return fail("early-runtime source is not canonical JSON");
    }
    exact_keys(&source, &["format", "kind", "catalog_format", "source_bytes", "early_runtime_tables", "post_map_load_residual"], "early-runtime source")?;
    if source.get("format") != Some(&Value::from(1)) || as_str(source.get("kind")) != Some("golden-sun-early-runtime-data") || source.get("catalog_format") != get(catalog, "format") || source.get("source_bytes") != Some(&Value::from(EARLY_RUNTIME_SOURCE_BYTES)) {
        return fail("unsupported early-runtime source");
    }
    Ok(Value::Object(source))
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
fn strip_index_suffix(name: &str) -> String {
    match name.strip_suffix("index.json") {
        Some(prefix) => prefix.to_string(),
        None => name.to_string(),
    }
}
fn realpath(path: &Path) -> Result<String> {
    match std::fs::canonicalize(path) {
        Ok(resolved) => Ok(resolved.to_string_lossy().into_owned()),
        Err(error) => Err(Error(error.to_string())),
    }
}
fn child(index_path: &str, name: Option<&Value>) -> Result<String> {
    if as_str(name) != Some(DISPLAY_SOURCE) {
        return fail("display tile source name differs");
    }
    let index = Path::new(index_path);
    let prefix = strip_index_suffix(index.file_name().and_then(|name| name.to_str()).unwrap_or_default());
    let root = realpath(index.parent().unwrap_or(Path::new(".")))?;
    let wanted = format!("{prefix}{DISPLAY_SOURCE}");
    let path = realpath(&Path::new(&root).join(&wanted))?;
    if Path::new(&path).strip_prefix(&root).ok().and_then(Path::to_str) != Some(wanted.as_str()) {
        return fail("display tile source escaped its package");
    }
    Ok(path)
}
fn is_symlink(path: &str) -> Result<bool> {
    match std::fs::symlink_metadata(path) {
        Ok(meta) => Ok(meta.file_type().is_symlink()),
        Err(error) => Err(Error(error.to_string())),
    }
}
fn checked_package_files(index_path: &str, catalog_path: &str) -> Result<()> {
    let index = Path::new(index_path);
    let root = index.parent().unwrap_or(Path::new("."));
    let prefix = strip_index_suffix(index.file_name().and_then(|name| name.to_str()).unwrap_or_default());
    let mut expected = vec![format!("{prefix}{DISPLAY_SOURCE}"), format!("{prefix}index.json")];
    expected.sort();
    let catalog_name = Path::new(catalog_path).file_name().and_then(|name| name.to_str()).unwrap_or_default();
    let entries = match std::fs::read_dir(&root) {
        Ok(entries) => entries,
        Err(error) => return Err(Error(error.to_string())),
    };
    let mut names: Vec<String> = Vec::new();
    for entry in entries.flatten() {
        let name = entry.file_name().to_string_lossy().into_owned();
        if name.starts_with(&prefix) && name != catalog_name {
            names.push(name);
        }
    }
    names.sort();
    if names != expected {
        return fail("early-runtime package contains unexpected files");
    }
    for name in &names {
        if is_symlink(&root.join(name).to_string_lossy())? {
            return fail("early-runtime package contains a symbolic link");
        }
    }
    Ok(())
}
#[derive(Debug, Clone)]
pub struct EarlyRuntimeBuild {
    pub regions: Vec<(i64, Vec<u8>)>,
    pub source_bytes: i64,
}
pub fn build_early_runtime_data(index_path: &str, catalog_path: &str) -> Result<EarlyRuntimeBuild> {
    let catalog = load_catalog(catalog_path)?;
    let (early, residual) = canonical_layout(&catalog)?;
    let source = source_index(index_path, &catalog)?;
    checked_package_files(index_path, catalog_path)?;
    let early_source = object(get(&source, "early_runtime_tables"), "early-runtime tables")?.clone();
    exact_keys(&early_source, &["address", "end", "display_tiles", "shared_lookup_storage", "interpolation_coefficients", "surface_lookup", "unclassified_0801356c", "render_limits", "object_programs", "object_handlers", "unclassified_08013724"], "early-runtime tables")?;
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
    place(&mut output, EARLY_RUNTIME_ADDRESS, find_entry(early, "display_tiles", false)?, &tiles)?;
    let lookup = object(early_source.get("shared_lookup_storage"), "shared lookup storage")?;
    exact_keys(lookup, &["encoding", "values", "views"], "shared lookup storage")?;
    if as_str(lookup.get("encoding")) != Some("u8-backing") {
        return fail("shared lookup backing encoding differs");
    }
    validate_view_sources(lookup.get("views"), early)?;
    let lookup_spec = find_entry(early, "shared_lookup_storage", false)?;
    let lookup_size = (address(get(lookup_spec, "end"))? - address(get(lookup_spec, "address"))?) as usize;
    let lookup_values = flat_values(lookup.get("values"), lookup_size, "u8", "shared lookup values")?;
    place(&mut output, EARLY_RUNTIME_ADDRESS, lookup_spec, &encode_numbers(&lookup_values, "u8")?)?;
    let coefficients = object(early_source.get("interpolation_coefficients"), "interpolation coefficients")?;
    exact_keys(coefficients, &["encoding", "values"], "interpolation coefficients")?;
    if as_str(coefficients.get("encoding")) != Some("u32le") {
        return fail("interpolation coefficient encoding differs");
    }
    let coefficient_values = flat_values(coefficients.get("values"), 16, "u32le", "interpolation coefficients")?;
    place(&mut output, EARLY_RUNTIME_ADDRESS, find_entry(early, "interpolation_coefficients", false)?, &encode_numbers(&coefficient_values, "u32le")?)?;
    let surface = object(early_source.get("surface_lookup"), "surface lookup")?;
    exact_keys(surface, &["encoding", "rows"], "surface lookup")?;
    if as_str(surface.get("encoding")) != Some("u8") {
        return fail("surface lookup encoding differs");
    }
    let surface_values = rows(surface.get("rows"), 3, 16, "u8", "surface lookup")?;
    place(&mut output, EARLY_RUNTIME_ADDRESS, find_entry(early, "surface_lookup", false)?, &encode_numbers(&surface_values, "u8")?)?;
    let unresolved_a = find_entry(early, "unclassified_0801356c", false)?;
    let size_a = (address(get(unresolved_a, "end"))? - address(get(unresolved_a, "address"))?) as usize;
    let residual_a = build_residual(early_source.get("unclassified_0801356c"), size_a, &entry_id(unresolved_a))?;
    place(&mut output, EARLY_RUNTIME_ADDRESS, unresolved_a, &residual_a)?;
    let limits = object(early_source.get("render_limits"), "render limits")?;
    exact_keys(limits, &["encoding", "values"], "render limits")?;
    if as_str(limits.get("encoding")) != Some("s32le") {
        return fail("render-limit encoding differs");
    }
    let limit_values = flat_values(limits.get("values"), 2, "s32le", "render limits")?;
    place(&mut output, EARLY_RUNTIME_ADDRESS, find_entry(early, "render_limits", false)?, &encode_numbers(&limit_values, "s32le")?)?;
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
    place(&mut output, EARLY_RUNTIME_ADDRESS, find_entry(early, "object_handlers", false)?, &encode_numbers(&handler_values, "pointer32le")?)?;
    let unresolved_b = find_entry(early, "unclassified_08013724", false)?;
    let size_b = (address(get(unresolved_b, "end"))? - address(get(unresolved_b, "address"))?) as usize;
    let residual_b = build_residual(early_source.get("unclassified_08013724"), size_b, &entry_id(unresolved_b))?;
    place(&mut output, EARLY_RUNTIME_ADDRESS, unresolved_b, &residual_b)?;
    let residual_src = object(get(&source, "post_map_load_residual"), "post-map residual")?.clone();
    exact_keys(&residual_src, &["address", "end", "unreferenced_storage"], "post-map residual")?;
    if residual_src.get("address") != get(residual, "address") || residual_src.get("end") != get(residual, "end") {
        return fail("post-map residual extent differs");
    }
    let residual_output = build_residual(residual_src.get("unreferenced_storage"), (POST_MAP_END - POST_MAP_ADDRESS) as usize, "post-map residual")?;
    let source_bytes = (output.len() + residual_output.len()) as i64;
    Ok(EarlyRuntimeBuild { regions: vec![(EARLY_RUNTIME_ADDRESS, output), (POST_MAP_ADDRESS, residual_output)], source_bytes })
}
