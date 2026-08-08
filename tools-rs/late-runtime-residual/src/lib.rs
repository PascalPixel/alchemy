// Port of tools/make/late_runtime_residual.ts.
//
// WHY THIS FILE EXISTS: the late runtime data package (tools/lib/late_runtime_data.ts,
// ported to tools-rs/late-runtime-data) understands the *typed* shape of the
// three late ROM ranges -- one compressed background stream and fourteen small
// numeric tables. This tool is the residual counterpart: it captures every byte
// of those same ranges as a flat, dumb, canonical JSON source, with no codec and
// no interpretation. Each catalog component becomes either a `uniform_fill`
// recipe (when the whole component is one repeated 0x00 or 0xff byte) or an
// explicit `byte_values` array. That gives the tree a second, independent
// witness for the 7294 residual bytes: the typed package can be rebuilt and
// checked against this one, and a bug in a codec cannot hide in both.
//
// The port keeps the TypeScript's exact error strings, stdout lines and JSON key
// order: the parity harness diffs them byte for byte.
//
// PORT NOTE (floats): every number this tool emits is an integer -- `format`,
// `source_bytes`, the fill `value`, and the `values` byte array. Addresses are
// emitted as `0x........` strings. So `serde_json`'s ryu float formatting is
// never reached and no ECMAScript `ToString(Number)` shim is needed.
// `emitted_json_is_integers_only` in the tests pins that by walking a real
// emitted document and failing on any non-integral number.

use std::path::{Path, PathBuf};

use canonical_json::{canonical_json, is_canonical_json_text};
use late_runtime_data::{
    read_late_runtime_catalog, ComponentKind, LateRuntimeCatalog, LateRuntimeComponent, TableKind,
    ROM_BASE,
};
use serde_json::{Map, Value};

/// `Result` with the TypeScript's `Error` message as the payload. The CLI prints
/// it as `error: <message>`, which is what Bun prints for an uncaught `Error`.
pub type Result<T> = std::result::Result<T, String>;

fn err<T>(message: impl Into<String>) -> Result<T> {
    Err(message.into())
}

/// The 7294 bytes the three catalog ranges cover. The TypeScript hard-codes this
/// literal in three places; so does the port, deliberately.
pub const SOURCE_BYTES: i64 = 7294;

// ---------------------------------------------------------------------------
// model
// ---------------------------------------------------------------------------

/// `Values` in the TypeScript -- a discriminated union on `representation`.
#[derive(Debug, Clone, PartialEq, Eq)]
pub enum Values {
    UniformFill { value: u8 },
    ByteValues { values: Vec<u8> },
}

/// The rebuilt residual: region start address -> region bytes, in catalog order.
///
/// PORT NOTE: the TypeScript uses `Map<number, Buffer>`, whose iteration order is
/// insertion order, and `main`/`exportLateRuntimeResidual` iterate it to report
/// the first mismatching address. A `HashMap` would make which address gets
/// reported nondeterministic, so this is an insertion-ordered vector.
#[derive(Debug, Clone, Default, PartialEq, Eq)]
pub struct Regions {
    entries: Vec<(u32, Vec<u8>)>,
}

impl Regions {
    fn set(&mut self, key: u32, value: Vec<u8>) {
        match self
            .entries
            .iter_mut()
            .find(|(existing, _)| *existing == key)
        {
            Some(slot) => slot.1 = value,
            None => self.entries.push((key, value)),
        }
    }

    pub fn iter(&self) -> std::slice::Iter<'_, (u32, Vec<u8>)> {
        self.entries.iter()
    }

    pub fn len(&self) -> usize {
        self.entries.len()
    }

    pub fn is_empty(&self) -> bool {
        self.entries.is_empty()
    }
}

#[derive(Debug, Clone)]
pub struct LateRuntimeResidualBuild {
    pub regions: Regions,
    pub source_bytes: i64,
}

// ---------------------------------------------------------------------------
// JS-shaped primitives
// ---------------------------------------------------------------------------

fn pretty(value: &Value) -> String {
    format!("{}\n", canonical_json(value))
}

/// `hex` -- `0x${value.toString(16).padStart(8, "0")}`.
fn hexadecimal(value: u32) -> String {
    format!("0x{value:08x}")
}

/// `object(value, label)` -- rejects non-objects, `null` and arrays.
fn object<'a>(value: &'a Value, label: &str) -> Result<&'a Map<String, Value>> {
    match value {
        Value::Object(map) => Ok(map),
        _ => err(format!("{label} must be an object")),
    }
}

/// `exactKeys` -- `Object.keys(value).sort()` against the expected set.
///
/// PORT NOTE: JS sorts strings by UTF-16 code unit and Rust by byte; every key
/// compared here is ASCII, where the two orders coincide.
fn exact_keys(value: &Map<String, Value>, keys: &[&str], label: &str) -> Result<()> {
    let mut actual: Vec<&str> = value.keys().map(String::as_str).collect();
    actual.sort_unstable();
    let mut expected: Vec<&str> = keys.to_vec();
    expected.sort_unstable();
    if actual != expected {
        return err(format!("{label} fields differ"));
    }
    Ok(())
}

/// `Number.isSafeInteger(value)`.
///
/// PORT NOTE: JS has one number type, so the JSON token `1.0` is the integer 1
/// and passes `Number.isSafeInteger`. `serde_json` may parse it as `f64`, so the
/// check goes through `as_f64` rather than `as_i64`, which would reject it.
fn safe_integer(value: &Value) -> Option<i64> {
    let Value::Number(number) = value else {
        return None;
    };
    let raw = number.as_f64()?;
    if !raw.is_finite() || raw.fract() != 0.0 || raw.abs() > 9_007_199_254_740_991.0 {
        return None;
    }
    Some(raw as i64)
}

/// `integer(value, minimum, maximum, label)`.
fn integer(value: &Value, minimum: i64, maximum: i64, label: &str) -> Result<i64> {
    match safe_integer(value) {
        Some(parsed) if parsed >= minimum && parsed <= maximum => Ok(parsed),
        _ => err(format!("{label} differs")),
    }
}

/// `value.field` where `value` may be any JSON -- JS yields `undefined` for a
/// missing property on any non-nullish value, which every caller below then
/// fails a `!==` comparison against.
fn field<'a>(value: &'a Value, key: &str) -> &'a Value {
    value.get(key).unwrap_or(&Value::Null)
}

fn string_is(value: &Value, expected: &str) -> bool {
    value.as_str() == Some(expected)
}

/// PORT NOTE: the TypeScript indexes `source.regions[index]` and immediately
/// reads `.name` off it. For a JSON `null` that is a `TypeError`, not the
/// tool's own `Error`, so Bun prints a different first line. The port raises
/// this message instead and the parity harness counts those cases separately
/// rather than diffing the prose. Every other non-object (string, number,
/// array) yields `undefined` fields in JS and falls through to the normal
/// "layout differs" / "metadata differs" error, which the port reproduces.
fn reject_null(value: &Value, label: &str) -> Result<()> {
    if value.is_null() {
        return err(format!("{label} is null"));
    }
    Ok(())
}

// ---------------------------------------------------------------------------
// export side
// ---------------------------------------------------------------------------

/// `typedKind` -- the catalog's component kind narrowed to the three document
/// types this residual format records.
fn typed_kind(component: &LateRuntimeComponent) -> &'static str {
    match component.kind {
        ComponentKind::Table(TableKind::S16) => "s16",
        ComponentKind::Table(TableKind::U16) => "u16",
        _ => "u8",
    }
}

/// `representation(data)`.
pub fn representation(data: &[u8]) -> Result<Values> {
    let Some(&first) = data.first() else {
        return err("late residual component is empty");
    };
    if (first == 0 || first == 255) && data.iter().all(|byte| *byte == first) {
        return Ok(Values::UniformFill { value: first });
    }
    Ok(Values::ByteValues {
        values: data.to_vec(),
    })
}

fn values_pairs(values: &Values) -> Vec<(&'static str, Value)> {
    match values {
        Values::UniformFill { value } => vec![
            ("representation", Value::from("uniform_fill")),
            ("value", Value::from(*value)),
        ],
        Values::ByteValues { values } => vec![
            ("representation", Value::from("byte_values")),
            (
                "values",
                Value::Array(values.iter().map(|byte| Value::from(*byte)).collect()),
            ),
        ],
    }
}

fn object_of(pairs: Vec<(&str, Value)>) -> Value {
    let mut map = Map::new();
    for (key, value) in pairs {
        map.insert(key.to_string(), value);
    }
    Value::Object(map)
}

/// `componentSource(component, image)`.
///
/// PORT NOTE: `Uint8Array#subarray` clamps out-of-range indices where Rust
/// slicing panics, and the TypeScript relies on that: it takes the (possibly
/// short) slice first and only then compares its length, which is how a ROM too
/// small for the catalog produces "<name> lies outside the ROM" rather than a
/// crash.
fn component_source(component: &LateRuntimeComponent, image: &[u8]) -> Result<Value> {
    let start = (component.address - ROM_BASE) as usize;
    let end = (component.end - ROM_BASE) as usize;
    let start = start.min(image.len());
    let end = end.clamp(start, image.len());
    let data = &image[start..end];
    if data.len() != component.size() {
        return err(format!("{} lies outside the ROM", component.name));
    }
    let mut pairs = vec![
        ("name", Value::from(component.name.clone())),
        ("address", Value::from(hexadecimal(component.address))),
        ("end", Value::from(hexadecimal(component.end))),
        ("role", Value::from(component.role.clone())),
        ("type", Value::from(typed_kind(component))),
    ];
    pairs.extend(values_pairs(&representation(data)?));
    Ok(object_of(pairs))
}

/// The `Source` document `exportLateRuntimeResidual` writes.
fn residual_document(catalog: &LateRuntimeCatalog, image: &[u8]) -> Result<Value> {
    let mut regions = Vec::new();
    for range in &catalog.ranges {
        let mut components = Vec::new();
        for component in &range.components {
            components.push(component_source(component, image)?);
        }
        regions.push(object_of(vec![
            ("name", Value::from(range.name.clone())),
            ("address", Value::from(hexadecimal(range.address))),
            ("end", Value::from(hexadecimal(range.end))),
            ("components", Value::Array(components)),
        ]));
    }
    Ok(object_of(vec![
        ("format", Value::from(1)),
        ("kind", Value::from("golden-sun-late-runtime-residual")),
        ("source_bytes", Value::from(SOURCE_BYTES)),
        ("regions", Value::Array(regions)),
    ]))
}

// ---------------------------------------------------------------------------
// build side
// ---------------------------------------------------------------------------

/// `parseSource(indexPath)`.
///
/// PORT NOTE: a missing file or malformed JSON is a native JS exception in the
/// TypeScript (`readFileSync` ENOENT, `JSON.parse` SyntaxError), which Bun
/// prints without an `error:` line at all. The port turns both into ordinary
/// error strings; the harness asserts same-failure for those, not same prose.
fn parse_source(index_path: &Path) -> Result<Value> {
    let bytes =
        std::fs::read(index_path).map_err(|error| format!("{}: {error}", index_path.display()))?;
    let text = String::from_utf8_lossy(&bytes).into_owned();
    let value: Value = serde_json::from_str(&text).map_err(|error| format!("{error}"))?;
    let map = object(&value, "late residual source")?;
    if !is_canonical_json_text(&text, &value) {
        return err("late residual source is not canonical JSON");
    }
    exact_keys(
        map,
        &["format", "kind", "source_bytes", "regions"],
        "late residual source",
    )?;
    let format_is_one = safe_integer(field(&value, "format")) == Some(1);
    if !format_is_one
        || !string_is(field(&value, "kind"), "golden-sun-late-runtime-residual")
        || !field(&value, "regions").is_array()
    {
        return err("late residual source identity differs");
    }
    Ok(value)
}

/// `componentData(source, component)`.
fn component_data(source: &Value, component: &LateRuntimeComponent) -> Result<Vec<u8>> {
    let size = component.size();
    reject_null(source, &component.name)?;
    if !string_is(field(source, "name"), &component.name)
        || !string_is(field(source, "address"), &hexadecimal(component.address))
        || !string_is(field(source, "end"), &hexadecimal(component.end))
        || !string_is(field(source, "role"), &component.role)
        || !string_is(field(source, "type"), typed_kind(component))
    {
        return err(format!("{} metadata differs", component.name));
    }
    let raw = object(source, &component.name)?;
    if string_is(field(source, "representation"), "uniform_fill") {
        exact_keys(
            raw,
            &[
                "name",
                "address",
                "end",
                "role",
                "type",
                "representation",
                "value",
            ],
            &component.name,
        )?;
        let value = field(source, "value");
        let fill = safe_integer(value);
        if fill != Some(0) && fill != Some(255) {
            return err(format!("{} fill differs", component.name));
        }
        return Ok(vec![fill.unwrap_or(0) as u8; size]);
    }
    exact_keys(
        raw,
        &[
            "name",
            "address",
            "end",
            "role",
            "type",
            "representation",
            "values",
        ],
        &component.name,
    )?;
    let values = field(source, "values");
    let items = values.as_array();
    if !string_is(field(source, "representation"), "byte_values")
        || items.is_none()
        || items.map(Vec::len) != Some(size)
    {
        return err(format!("{} values differ", component.name));
    }
    let items = items.expect("checked above");
    let mut data = Vec::with_capacity(size);
    for (index, item) in items.iter().enumerate() {
        data.push(integer(item, 0, 255, &format!("{} value {index}", component.name))? as u8);
    }
    Ok(data)
}

/// `buildLateRuntimeResidual(indexPath, catalogPath)`.
pub fn build_late_runtime_residual(
    index_path: &Path,
    catalog_path: &Path,
) -> Result<LateRuntimeResidualBuild> {
    let catalog = read_late_runtime_catalog(catalog_path)?;
    let source = parse_source(index_path)?;
    let source_regions = field(&source, "regions")
        .as_array()
        .cloned()
        .unwrap_or_default();
    if source_regions.len() != catalog.ranges.len() {
        return err("late residual region count differs");
    }
    let mut regions = Regions::default();
    for (index, range) in catalog.ranges.iter().enumerate() {
        let item = &source_regions[index];
        reject_null(item, &range.name)?;
        let components = field(item, "components").as_array();
        if !string_is(field(item, "name"), &range.name)
            || !string_is(field(item, "address"), &hexadecimal(range.address))
            || !string_is(field(item, "end"), &hexadecimal(range.end))
            || components.is_none()
            || components.map(Vec::len) != Some(range.components.len())
        {
            return err(format!("{} layout differs", range.name));
        }
        let components = components.expect("checked above");
        let mut data = Vec::new();
        for (component_index, component) in range.components.iter().enumerate() {
            data.extend(component_data(&components[component_index], component)?);
        }
        if data.len() != (range.end - range.address) as usize {
            return err(format!("{} length differs", range.name));
        }
        regions.set(range.address, data);
    }
    let source_bytes: i64 = regions.iter().map(|(_, data)| data.len() as i64).sum();
    let declared = safe_integer(field(&source, "source_bytes"));
    if declared != Some(source_bytes) || source_bytes != SOURCE_BYTES {
        return err("late residual source-byte total differs");
    }
    Ok(LateRuntimeResidualBuild {
        regions,
        source_bytes,
    })
}

/// `image.subarray(start, start + length)` -- clamping, as JS does.
fn window(image: &[u8], address: u32, length: usize) -> &[u8] {
    let start = ((address - ROM_BASE) as usize).min(image.len());
    let end = start.saturating_add(length).min(image.len());
    &image[start..end]
}

/// `exportLateRuntimeResidual(romPath, directory, catalogPath)`.
pub fn export_late_runtime_residual(
    rom_path: &Path,
    directory: &Path,
    catalog_path: &Path,
) -> Result<()> {
    let image =
        std::fs::read(rom_path).map_err(|error| format!("{}: {error}", rom_path.display()))?;
    let catalog = read_late_runtime_catalog(catalog_path)?;
    let source = residual_document(&catalog, &image)?;
    std::fs::create_dir_all(directory)
        .map_err(|error| format!("{}: {error}", directory.display()))?;
    let index = directory.join("index.json");
    std::fs::write(&index, pretty(&source))
        .map_err(|error| format!("{}: {error}", index.display()))?;
    let built = build_late_runtime_residual(&index, catalog_path)?;
    for (address, data) in built.regions.iter() {
        if window(&image, *address, data.len()) != data.as_slice() {
            return err(format!("{} differs from the ROM", hexadecimal(*address)));
        }
    }
    Ok(())
}

/// `verify` in `main`.
pub fn verify_late_runtime_residual(
    rom_path: &Path,
    index_path: &Path,
    catalog_path: &Path,
) -> Result<LateRuntimeResidualBuild> {
    let image =
        std::fs::read(rom_path).map_err(|error| format!("{}: {error}", rom_path.display()))?;
    let built = build_late_runtime_residual(index_path, catalog_path)?;
    for (address, data) in built.regions.iter() {
        if window(&image, *address, data.len()) != data.as_slice() {
            return err(format!("{} differs from the ROM", hexadecimal(*address)));
        }
    }
    Ok(built)
}

// ---------------------------------------------------------------------------
// self-test
// ---------------------------------------------------------------------------

/// `Bun.env.TMPDIR ?? "/private/tmp"`, then `resolve`d.
fn temporary_root() -> PathBuf {
    let base = std::env::var("TMPDIR")
        .ok()
        .filter(|value| !value.is_empty());
    PathBuf::from(base.unwrap_or_else(|| "/private/tmp".to_string()))
}

/// `selfTest()`. Returns the stdout line.
pub fn self_test(catalog_path: &Path) -> Result<String> {
    let temporary = temporary_root().join(format!("alchemy-late-residual-{}", std::process::id()));
    if temporary.exists() {
        return err("late residual self-test directory exists");
    }
    let outcome = self_test_body(catalog_path, &temporary);
    if temporary.exists() {
        let _ = std::fs::remove_dir_all(&temporary);
    }
    outcome?;
    Ok("self-test=ok source_bytes=7294".to_string())
}

/// Counted so a future edit cannot silently drop a check: the self-test must
/// execute exactly this many assertions.
pub const SELF_TEST_CHECKS: usize = 3;

fn self_test_body(catalog_path: &Path, temporary: &Path) -> Result<()> {
    let mut checks = 0usize;
    let catalog = read_late_runtime_catalog(catalog_path)?;
    let mut image = vec![0u8; 0x100000];
    for range in &catalog.ranges {
        for component in &range.components {
            let start = (component.address - ROM_BASE) as usize;
            let end = (component.end - ROM_BASE) as usize;
            let fill = if component.name.starts_with("aki_") {
                255
            } else {
                0
            };
            image[start..end].fill(fill);
        }
    }
    let rom = temporary.join("gs1-en.gba");
    let output = temporary.join("source");
    std::fs::create_dir_all(temporary).map_err(|error| format!("{error}"))?;
    std::fs::write(&rom, &image).map_err(|error| format!("{error}"))?;
    export_late_runtime_residual(&rom, &output, catalog_path)?;
    checks += 1;

    let index = output.join("index.json");
    if build_late_runtime_residual(&index, catalog_path)?.source_bytes != SOURCE_BYTES {
        return err("late residual self-test total differs");
    }
    checks += 1;

    let text = std::fs::read_to_string(&index).map_err(|error| format!("{error}"))?;
    let mut source: Value = serde_json::from_str(&text).map_err(|error| format!("{error}"))?;
    source["regions"][0]["components"][0]["values"] = Value::Array(Vec::new());
    std::fs::write(&index, pretty(&source)).map_err(|error| format!("{error}"))?;
    if build_late_runtime_residual(&index, catalog_path).is_ok() {
        return err("late residual malformed values were accepted");
    }
    checks += 1;

    if checks != SELF_TEST_CHECKS {
        return err("late residual self-test check count differs");
    }
    Ok(())
}

#[cfg(test)]
mod tests;
