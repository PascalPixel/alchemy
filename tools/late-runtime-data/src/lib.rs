// Port of tools/lib/late_runtime_data.ts.
//
// WHY THIS FILE EXISTS: three late ROM ranges (0x080f38bc, 0x080f53ce,
// 0x080f86f8) hold runtime data the decompilation must reproduce byte for byte
// -- one compressed background stream, fourteen small numeric tables, and the
// structural fill that pads each range to its boundary. This module is the
// single place that knows the layout: it reads the tracked catalog, extracts
// those components out of a ROM image into a canonical JSON package, rebuilds
// the ranges from that package, and proves the rebuild is identical to the
// image. Every write goes through a staged rename so a failed export can never
// leave a half-written package where the tree expects a canonical one.
//
// The port keeps the TypeScript's exact error strings and stdout lines: the
// parity harness diffs them.
//
// PORT NOTE (floats): every number this tool emits into tracked JSON is an
// integer (word values, table values, byte counts, sizes). Addresses are
// emitted as `0x........` strings. So `serde_json`'s ryu float formatting is
// never reached and no ECMAScript `ToString(Number)` shim is needed here --
// unlike compare-roms, which emits a ratio. `emitted_json_is_integers_only`
// below pins that: it walks every document this crate writes and fails if any
// number is non-integral.

use std::collections::HashSet;
use std::path::{Component, Path, PathBuf};

use canonical_json::{canonical_json, is_canonical_json_text};
use extract_resource::{decode_general_trace, encode_general, GeneralToken};
use serde_json::{Map, Value};

pub const ROM_BASE: u32 = 0x0800_0000;
pub const ROM_SIZE: usize = 0x0080_0000;

/// `Result` with the TypeScript's `Error` message as the payload. The CLI
/// prints it as `error: <message>`, which is what Bun prints for an uncaught
/// `Error`, so stderr's first meaningful line matches.
pub type Result<T> = std::result::Result<T, String>;

fn err<T>(message: impl Into<String>) -> Result<T> {
    Err(message.into())
}

// ---------------------------------------------------------------------------
// constants (mirrored from the TypeScript, same order)
// ---------------------------------------------------------------------------

/// `PACKAGE_FILES` -- key order is load-bearing: it is emitted into index.json.
pub const PACKAGE_FILES: [(&str, &str); 5] = [
    ("catalog", "catalog.json"),
    ("stream", "haikei.json"),
    ("plan", "haikei.lz.json"),
    ("tables", "tables.json"),
    ("fills", "fills.json"),
];

fn package_file(key: &str) -> &'static str {
    PACKAGE_FILES.iter().find(|(name, _)| *name == key).expect("known package file key").1
}

const CANONICAL_RANGES: [(&str, u32, u32); 3] = [
    ("range_080f38bc", 0x080f_38bc, 0x080f_4000),
    ("range_080f53ce", 0x080f_53ce, 0x080f_6000),
    ("range_080f86f8", 0x080f_86f8, 0x080f_9000),
];

const CANONICAL_COMPONENTS: [(&str, &str, u32, u32, i64); 18] = [
    ("haikei_stream", "general_lz_stream", 0x080f_38bc, 0x080f_39ab, 600),
    ("haichi_hyou", "u8_table", 0x080f_39ab, 0x080f_39b1, 6),
    ("henka_hyou", "u8_table", 0x080f_39b1, 0x080f_39ed, 60),
    ("soroe_080f39ed", "unresolved_fill", 0x080f_39ed, 0x080f_39ee, 0),
    ("iro_henka_00", "u16_table", 0x080f_39ee, 0x080f_3a2e, 32),
    ("iro_henka_01", "u16_table", 0x080f_3a2e, 0x080f_3a6e, 32),
    ("iro_henka_02", "u16_table", 0x080f_3a6e, 0x080f_3aae, 32),
    ("aki_080f3aae", "unresolved_fill", 0x080f_3aae, 0x080f_4000, 0),
    ("aki_080f53ce", "unresolved_fill", 0x080f_53ce, 0x080f_541a, 0),
    ("hantei_dankai", "s16_table", 0x080f_541a, 0x080f_5426, 6),
    ("aki_080f5426", "unresolved_fill", 0x080f_5426, 0x080f_6000, 0),
    ("zahyoubisei_hyou", "u16_table", 0x080f_86f8, 0x080f_870c, 10),
    ("hyouji_shoki_hyou", "u8_table", 0x080f_870c, 0x080f_8712, 6),
    ("haba_hyou_00", "u8_table", 0x080f_8712, 0x080f_871a, 8),
    ("haichi_hyou_00", "u8_table", 0x080f_871a, 0x080f_8728, 14),
    ("haichi_hyou_01", "u8_table", 0x080f_8728, 0x080f_8736, 14),
    ("haba_hyou_01", "u8_table", 0x080f_8736, 0x080f_873e, 8),
    ("aki_080f873e", "unresolved_fill", 0x080f_873e, 0x080f_9000, 0),
];

const RANGE_BYTES: i64 = 7_294;

// ---------------------------------------------------------------------------
// model
// ---------------------------------------------------------------------------

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum TableKind {
    U8,
    S16,
    U16,
}

impl TableKind {
    fn width(self) -> u32 {
        match self {
            TableKind::U8 => 1,
            TableKind::S16 | TableKind::U16 => 2,
        }
    }

    fn kind_name(self) -> &'static str {
        match self {
            TableKind::U8 => "u8_table",
            TableKind::S16 => "s16_table",
            TableKind::U16 => "u16_table",
        }
    }

    /// `tableType` in the TypeScript.
    fn document_type(self) -> &'static str {
        match self {
            TableKind::U8 => "u8",
            TableKind::S16 => "s16",
            TableKind::U16 => "u16",
        }
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum ComponentKind {
    GeneralLzStream,
    Table(TableKind),
    Fill,
    UnresolvedFill,
}

impl ComponentKind {
    fn name(self) -> &'static str {
        match self {
            ComponentKind::GeneralLzStream => "general_lz_stream",
            ComponentKind::Table(kind) => kind.kind_name(),
            ComponentKind::Fill => "fill",
            ComponentKind::UnresolvedFill => "unresolved_fill",
        }
    }

    fn parse(text: &str) -> Option<Self> {
        Some(match text {
            "general_lz_stream" => ComponentKind::GeneralLzStream,
            "u8_table" => ComponentKind::Table(TableKind::U8),
            "s16_table" => ComponentKind::Table(TableKind::S16),
            "u16_table" => ComponentKind::Table(TableKind::U16),
            "fill" => ComponentKind::Fill,
            "unresolved_fill" => ComponentKind::UnresolvedFill,
            _ => return None,
        })
    }
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct LateRuntimeComponent {
    pub name: String,
    pub kind: ComponentKind,
    pub address: u32,
    pub end: u32,
    pub role: String,
    pub consumers: Vec<String>,
    pub count: Option<i64>,
    pub decoded_bytes: Option<i64>,
    pub value: Option<i64>,
    pub reason: Option<String>,
}

impl LateRuntimeComponent {
    pub fn size(&self) -> usize {
        (self.end - self.address) as usize
    }
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct LateRuntimeRange {
    pub name: String,
    pub address: u32,
    pub end: u32,
    pub components: Vec<LateRuntimeComponent>,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct LateRuntimeCatalog {
    pub ranges: Vec<LateRuntimeRange>,
}

impl LateRuntimeCatalog {
    /// `packageComponents` -- flattened in range order.
    pub fn components(&self) -> Vec<&LateRuntimeComponent> {
        self.ranges.iter().flat_map(|range| range.components.iter()).collect()
    }

    fn tables(&self) -> Vec<(&LateRuntimeComponent, TableKind)> {
        self.components()
            .into_iter()
            .filter_map(|component| match component.kind {
                ComponentKind::Table(kind) => Some((component, kind)),
                _ => None,
            })
            .collect()
    }

    fn single_stream(&self) -> Result<&LateRuntimeComponent> {
        let streams: Vec<&LateRuntimeComponent> = self
            .components()
            .into_iter()
            .filter(|component| component.kind == ComponentKind::GeneralLzStream)
            .collect();
        if streams.len() != 1 {
            return err("late runtime catalog must contain one compressed stream");
        }
        Ok(streams[0])
    }
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub enum LateRuntimeSource {
    GeneralLzStream { decoded: Vec<u8>, tokens: Vec<GeneralToken>, lookahead: Vec<u8> },
    Table { kind: TableKind, values: Vec<i64> },
    Fill,
}

impl LateRuntimeSource {
    fn kind(&self) -> ComponentKind {
        match self {
            LateRuntimeSource::GeneralLzStream { .. } => ComponentKind::GeneralLzStream,
            LateRuntimeSource::Table { kind, .. } => ComponentKind::Table(*kind),
            LateRuntimeSource::Fill => ComponentKind::Fill,
        }
    }
}

/// PORT NOTE: the TypeScript uses `Map`, whose iteration order is insertion
/// order. A `HashMap` would reorder the emitted documents and the build report,
/// so every map in this port is an insertion-ordered vector. `set` overwrites
/// in place and keeps the original key position, matching `Map#set`.
#[derive(Debug, Clone, Default, PartialEq, Eq)]
pub struct OrderedMap<K, V> {
    entries: Vec<(K, V)>,
}

impl<K: PartialEq, V> OrderedMap<K, V> {
    pub fn new() -> Self {
        Self { entries: Vec::new() }
    }

    pub fn set(&mut self, key: K, value: V) {
        match self.entries.iter_mut().find(|(existing, _)| *existing == key) {
            Some(slot) => slot.1 = value,
            None => self.entries.push((key, value)),
        }
    }

    pub fn get(&self, key: &K) -> Option<&V> {
        self.entries.iter().find(|(existing, _)| existing == key).map(|(_, value)| value)
    }

    pub fn get_mut(&mut self, key: &K) -> Option<&mut V> {
        self.entries.iter_mut().find(|(existing, _)| existing == key).map(|(_, value)| value)
    }

    pub fn len(&self) -> usize {
        self.entries.len()
    }

    pub fn is_empty(&self) -> bool {
        self.entries.is_empty()
    }

    pub fn iter(&self) -> std::slice::Iter<'_, (K, V)> {
        self.entries.iter()
    }
}

pub type Regions = OrderedMap<u32, Vec<u8>>;

#[derive(Debug, Clone)]
pub struct LateRuntimeExtraction {
    pub sources: OrderedMap<String, LateRuntimeSource>,
    pub unresolved: Vec<LateRuntimeComponent>,
}

#[derive(Debug, Clone)]
pub struct LateRuntimeBuild {
    pub regions: Regions,
    pub unresolved: Vec<LateRuntimeComponent>,
    pub typed_bytes: usize,
}

#[derive(Debug, Clone)]
pub struct LateRuntimePackageBuild {
    pub ranges: Regions,
    pub components: Regions,
    pub source_bytes: i64,
}

// ---------------------------------------------------------------------------
// JS-shaped primitives
// ---------------------------------------------------------------------------

/// `Object.keys(value).sort()` compared against the expected key set.
///
/// PORT NOTE: JS sorts strings by UTF-16 code unit and Rust by byte; every key
/// here is ASCII, where the two orders coincide.
fn exact_keys(value: &Value, keys: &[&str], label: &str) -> Result<()> {
    let Value::Object(entries) = value else {
        return err(format!("{label} must be an object"));
    };
    let mut actual: Vec<&str> = entries.keys().map(String::as_str).collect();
    actual.sort_unstable();
    let mut expected: Vec<&str> = keys.to_vec();
    expected.sort_unstable();
    if actual != expected {
        return err(format!("{label} has unknown or missing fields"));
    }
    Ok(())
}

fn field<'a>(value: &'a Value, key: &str) -> &'a Value {
    value.get(key).unwrap_or(&Value::Null)
}

/// `typeof value === "number" && Number.isSafeInteger(value)`.
///
/// PORT NOTE: JS has one number type, so the JSON token `1.0` is the integer 1
/// and passes `Number.isSafeInteger`. `serde_json` parses it as `f64`, so the
/// check goes through `as_f64` rather than `as_i64`, which would reject it.
fn safe_integer(value: &Value) -> Option<i64> {
    let Value::Number(number) = value else { return None };
    let raw = number.as_f64()?;
    if !raw.is_finite() || raw.fract() != 0.0 || raw.abs() > 9_007_199_254_740_991.0 {
        return None;
    }
    Some(raw as i64)
}

fn integer(value: &Value, minimum: i64, maximum: i64, label: &str) -> Result<i64> {
    match safe_integer(value) {
        Some(parsed) if parsed >= minimum && parsed <= maximum => Ok(parsed),
        _ => err(format!("{label} is outside its range")),
    }
}

/// JS `===` between a JSON number and a literal, tolerating the `1.0` spelling.
fn number_is(value: &Value, expected: i64) -> bool {
    matches!(value, Value::Number(number) if number.as_f64() == Some(expected as f64))
}

fn string_is(value: &Value, expected: &str) -> bool {
    value.as_str() == Some(expected)
}

/// `/^0x08[0-9a-f]{6}$/`
fn address(value: &Value, label: &str) -> Result<u32> {
    let text = value.as_str().unwrap_or("");
    let bytes = text.as_bytes();
    let shaped = bytes.len() == 10
        && &bytes[..4] == b"0x08"
        && bytes[4..].iter().all(|byte| byte.is_ascii_digit() || (b'a'..=b'f').contains(byte));
    if !shaped {
        return err(format!("{label} is not a canonical ROM address"));
    }
    Ok(u32::from_str_radix(&text[2..], 16).expect("eight validated hex digits"))
}

/// `/^[a-z][a-z0-9_]*$/`
fn identifier(value: &Value, label: &str) -> Result<String> {
    let text = value.as_str().unwrap_or("");
    let bytes = text.as_bytes();
    let shaped = !bytes.is_empty()
        && bytes[0].is_ascii_lowercase()
        && bytes.iter().all(|byte| byte.is_ascii_lowercase() || byte.is_ascii_digit() || *byte == b'_');
    if !shaped {
        return err(format!("{label} is not a canonical identifier"));
    }
    Ok(text.to_string())
}

/// `/^Func_080[0-9a-f]{5}$/`, non-empty, no duplicates.
fn consumer_list(value: &Value, label: &str) -> Result<Vec<String>> {
    let Some(items) = value.as_array() else {
        return err(format!("{label} differs"));
    };
    let shaped = !items.is_empty()
        && items.iter().all(|item| {
            let text = item.as_str().unwrap_or("");
            let bytes = text.as_bytes();
            bytes.len() == 13
                && &bytes[..8] == b"Func_080"
                && bytes[8..].iter().all(|byte| byte.is_ascii_digit() || (b'a'..=b'f').contains(byte))
        });
    if !shaped {
        return err(format!("{label} differs"));
    }
    let values: Vec<String> = items.iter().map(|item| item.as_str().unwrap_or("").to_string()).collect();
    if values.iter().collect::<HashSet<_>>().len() != values.len() {
        return err(format!("{label} contains duplicates"));
    }
    Ok(values)
}

fn hexadecimal(value: u32) -> String {
    format!("0x{value:08x}")
}

fn pretty(value: &Value) -> String {
    format!("{}\n", canonical_json(value))
}

fn object_of(pairs: Vec<(&str, Value)>) -> Value {
    let mut map = Map::new();
    for (key, value) in pairs {
        map.insert(key.to_string(), value);
    }
    Value::Object(map)
}

// ---------------------------------------------------------------------------
// filesystem primitives
// ---------------------------------------------------------------------------

/// `readFileSync(path, "utf8")` + `JSON.parse` + the canonical-form check.
///
/// PORT NOTE: `"utf8"` in Node/Bun is lossy, so invalid UTF-8 becomes U+FFFD
/// rather than an error; `from_utf8_lossy` matches. Duplicate keys collapse in
/// both `JSON.parse` and `serde_json`, so the round-trip check rejects them the
/// same way (the TypeScript self-test relies on exactly that).
pub fn canonical_document(path: &Path, label: &str) -> Result<Value> {
    let bytes = std::fs::read(path).map_err(|error| format!("{}: {error}", path.display()))?;
    let text = String::from_utf8_lossy(&bytes).into_owned();
    let value: Value = serde_json::from_str(&text).map_err(|error| format!("{error}"))?;
    if !is_canonical_json_text(&text, &value) {
        return err(format!("{label} is not canonical JSON"));
    }
    Ok(value)
}

/// `existsSync(path) && lstatSync(path).isFile()` -- `existsSync` follows
/// symlinks but `lstat` does not, so a symlink to a regular file is rejected.
/// That asymmetry is what stops a package member from being a link out of the
/// package, and the TypeScript self-test exercises it.
fn regular_file(path: &Path, label: &str) -> Result<()> {
    let is_file = path.exists()
        && std::fs::symlink_metadata(path).map(|meta| meta.file_type().is_file()).unwrap_or(false);
    if !is_file {
        return err(format!("{label} is not a regular file"));
    }
    Ok(())
}

/// `path.resolve()` against the process cwd, without touching the filesystem.
fn resolve_path(path: &Path) -> PathBuf {
    let absolute = if path.is_absolute() {
        path.to_path_buf()
    } else {
        std::env::current_dir().unwrap_or_else(|_| PathBuf::from("/")).join(path)
    };
    let mut normalized = PathBuf::new();
    for component in absolute.components() {
        match component {
            Component::CurDir => {}
            Component::ParentDir => {
                normalized.pop();
            }
            other => normalized.push(other.as_os_str()),
        }
    }
    normalized
}

/// `physicalPath` -- realpath the nearest existing ancestor, then re-append the
/// part that does not exist yet.
fn physical_path(path: &Path) -> PathBuf {
    let absolute = resolve_path(path);
    let mut ancestor = absolute.clone();
    while !ancestor.exists() {
        let Some(parent) = ancestor.parent().map(Path::to_path_buf) else {
            return absolute;
        };
        if parent == ancestor {
            return absolute;
        }
        ancestor = parent;
    }
    let Ok(real) = std::fs::canonicalize(&ancestor) else { return absolute };
    match absolute.strip_prefix(&ancestor) {
        Ok(rest) => real.join(rest),
        Err(_) => real,
    }
}

fn same_path(left: &Path, right: &Path) -> bool {
    physical_path(left) == physical_path(right)
}

/// `containsPath` -- true when `path` is inside `directory` or is `directory`.
fn contains_path(directory: &Path, path: &Path) -> bool {
    physical_path(path).starts_with(physical_path(directory))
}

/// `readdirSync(path)`, optionally sorted the way JS sorts (ASCII names here,
/// so byte order and UTF-16 code-unit order agree).
fn read_dir_names(path: &Path) -> Result<Vec<String>> {
    let mut names = Vec::new();
    for entry in std::fs::read_dir(path).map_err(|error| format!("{}: {error}", path.display()))? {
        let entry = entry.map_err(|error| format!("{error}"))?;
        names.push(entry.file_name().to_string_lossy().into_owned());
    }
    Ok(names)
}

/// `mkdtempSync(join(parent, prefix))`.
///
/// PORT NOTE: std has no `mkdtemp`, so this is the same contract implemented by
/// hand -- create-new in a loop, so an existing name is never reused.
fn make_temporary_directory(parent: &Path, prefix: &str) -> Result<PathBuf> {
    use std::time::{SystemTime, UNIX_EPOCH};
    for attempt in 0..4096u32 {
        let nanos = SystemTime::now()
            .duration_since(UNIX_EPOCH)
            .map(|since| since.subsec_nanos())
            .unwrap_or(0);
        let candidate =
            parent.join(format!("{prefix}{:x}{:x}{attempt:x}", std::process::id(), nanos));
        match std::fs::create_dir(&candidate) {
            Ok(()) => return Ok(candidate),
            Err(error) if error.kind() == std::io::ErrorKind::AlreadyExists => continue,
            Err(error) => return err(format!("{}: {error}", candidate.display())),
        }
    }
    err("could not create a temporary directory")
}

fn write_file(path: &Path, bytes: &[u8]) -> Result<()> {
    std::fs::write(path, bytes).map_err(|error| format!("{}: {error}", path.display()))
}

fn create_dir_all(path: &Path) -> Result<()> {
    std::fs::create_dir_all(path).map_err(|error| format!("{}: {error}", path.display()))
}

// ---------------------------------------------------------------------------
// catalog
// ---------------------------------------------------------------------------

fn parse_component(value: &Value, index: usize) -> Result<LateRuntimeComponent> {
    if !value.is_object() {
        return err(format!("component {index} must be an object"));
    }
    let raw_kind = field(value, "kind");
    let kind_text = raw_kind.as_str().unwrap_or("");
    let mut keys = vec!["name", "kind", "address", "end", "role", "consumers"];
    if raw_kind.is_string() && kind_text.ends_with("_table") {
        keys.push("count");
    }
    if kind_text == "general_lz_stream" {
        keys.push("decoded_bytes");
    }
    if kind_text == "fill" {
        keys.push("value");
    }
    if kind_text == "unresolved_fill" {
        keys.push("reason");
    }
    exact_keys(value, &keys, &format!("late runtime component {index}"))?;
    let name = identifier(field(value, "name"), &format!("component {index} name"))?;
    let Some(kind) = ComponentKind::parse(kind_text) else {
        return err(format!("component {name} kind differs"));
    };
    let start = address(field(value, "address"), &format!("component {name} address"))?;
    let end = address(field(value, "end"), &format!("component {name} end"))?;
    if end <= start {
        return err(format!("component {name} has an empty extent"));
    }
    let mut component = LateRuntimeComponent {
        role: identifier(field(value, "role"), &format!("component {name} role"))?,
        consumers: consumer_list(field(value, "consumers"), &format!("component {name} consumers"))?,
        name: name.clone(),
        kind,
        address: start,
        end,
        count: None,
        decoded_bytes: None,
        value: None,
        reason: None,
    };
    match kind {
        ComponentKind::GeneralLzStream => {
            component.decoded_bytes = Some(integer(
                field(value, "decoded_bytes"),
                1,
                0x100000,
                &format!("component {name} decoded bytes"),
            )?);
        }
        ComponentKind::Table(table) => {
            let count =
                integer(field(value, "count"), 1, 0x100000, &format!("component {name} count"))?;
            component.count = Some(count);
            if count * i64::from(table.width()) != i64::from(end - start) {
                return err(format!("component {name} table extent differs"));
            }
        }
        ComponentKind::Fill => {
            component.value =
                Some(integer(field(value, "value"), 0, 0xff, &format!("component {name} fill value"))?);
        }
        ComponentKind::UnresolvedFill => {
            component.reason =
                Some(identifier(field(value, "reason"), &format!("component {name} reason"))?);
        }
    }
    Ok(component)
}

pub fn parse_late_runtime_catalog(value: &Value) -> Result<LateRuntimeCatalog> {
    exact_keys(value, &["format", "kind", "ranges"], "late runtime catalog")?;
    let raw_ranges = field(value, "ranges").as_array().cloned().unwrap_or_default();
    if !number_is(field(value, "format"), 1)
        || !string_is(field(value, "kind"), "golden-sun-late-runtime-data-layout")
        || !field(value, "ranges").is_array()
        || raw_ranges.is_empty()
    {
        return err("unsupported late runtime catalog");
    }
    let mut names: HashSet<String> = HashSet::new();
    let mut previous_end: u32 = 0;
    let mut ranges = Vec::new();
    for (range_index, raw) in raw_ranges.iter().enumerate() {
        exact_keys(
            raw,
            &["name", "address", "end", "components"],
            &format!("late runtime range {range_index}"),
        )?;
        let name = identifier(field(raw, "name"), &format!("range {range_index} name"))?;
        let start = address(field(raw, "address"), &format!("range {name} address"))?;
        let end = address(field(raw, "end"), &format!("range {name} end"))?;
        let raw_components = field(raw, "components").as_array().cloned().unwrap_or_default();
        if end <= start
            || start < previous_end
            || !field(raw, "components").is_array()
            || raw_components.is_empty()
        {
            return err(format!("range {name} extent differs"));
        }
        previous_end = end;
        let mut components = Vec::new();
        for (index, raw_component) in raw_components.iter().enumerate() {
            components.push(parse_component(raw_component, index)?);
        }
        let mut cursor = start;
        for component in &components {
            if !names.insert(component.name.clone()) {
                return err(format!("duplicate component {}", component.name));
            }
            if component.address != cursor || component.end > end {
                return err(format!("range {name} is not contiguous"));
            }
            cursor = component.end;
        }
        if cursor != end {
            return err(format!("range {name} does not reach its end"));
        }
        ranges.push(LateRuntimeRange { name, address: start, end, components });
    }
    if ranges.len() != CANONICAL_RANGES.len()
        || ranges.iter().enumerate().any(|(index, range)| {
            let expected = CANONICAL_RANGES[index];
            range.name != expected.0 || range.address != expected.1 || range.end != expected.2
        })
    {
        return err("late runtime canonical range layout differs");
    }
    let catalog = LateRuntimeCatalog { ranges };
    let components = catalog.components();
    if components.len() != CANONICAL_COMPONENTS.len()
        || components.iter().enumerate().any(|(index, component)| {
            let expected = CANONICAL_COMPONENTS[index];
            // `component.decodedBytes` for a stream, else `count ?? 0`.
            let quantity = if component.kind == ComponentKind::GeneralLzStream {
                component.decoded_bytes.unwrap_or(0)
            } else {
                component.count.unwrap_or(0)
            };
            component.name != expected.0
                || component.kind.name() != expected.1
                || component.address != expected.2
                || component.end != expected.3
                || quantity != expected.4
        })
    {
        return err("late runtime canonical component layout differs");
    }
    Ok(catalog)
}

pub fn read_late_runtime_catalog(path: &Path) -> Result<LateRuntimeCatalog> {
    parse_late_runtime_catalog(&canonical_document(path, "late runtime catalog")?)
}

// ---------------------------------------------------------------------------
// extraction / build
// ---------------------------------------------------------------------------

fn component_bytes(
    image: &[u8],
    component: &LateRuntimeComponent,
    image_base: u32,
) -> Result<Vec<u8>> {
    let start = i64::from(component.address) - i64::from(image_base);
    let end = i64::from(component.end) - i64::from(image_base);
    if start < 0 || end > image.len() as i64 {
        return err(format!("component {} is outside the image", component.name));
    }
    Ok(image[start as usize..end as usize].to_vec())
}

fn decode_table(kind: TableKind, data: &[u8]) -> Vec<i64> {
    match kind {
        TableKind::U8 => data.iter().map(|byte| i64::from(*byte)).collect(),
        TableKind::S16 => data
            .chunks_exact(2)
            .map(|pair| i64::from(i16::from_le_bytes([pair[0], pair[1]])))
            .collect(),
        TableKind::U16 => data
            .chunks_exact(2)
            .map(|pair| i64::from(u16::from_le_bytes([pair[0], pair[1]])))
            .collect(),
    }
}

pub fn extract_late_runtime_data(
    image: &[u8],
    catalog: &LateRuntimeCatalog,
    image_base: u32,
) -> Result<LateRuntimeExtraction> {
    let mut sources: OrderedMap<String, LateRuntimeSource> = OrderedMap::new();
    let mut unresolved = Vec::new();
    for range in &catalog.ranges {
        for component in &range.components {
            if component.kind == ComponentKind::UnresolvedFill {
                unresolved.push(component.clone());
                continue;
            }
            let data = component_bytes(image, component, image_base)?;
            match component.kind {
                ComponentKind::Fill => {
                    let expected = component.value.unwrap_or(0) as u8;
                    if data.iter().any(|byte| *byte != expected) {
                        return err(format!("component {} fill differs", component.name));
                    }
                    sources.set(component.name.clone(), LateRuntimeSource::Fill);
                }
                ComponentKind::GeneralLzStream => {
                    let maximum = component.decoded_bytes.unwrap_or(0);
                    let (decoded, _, tokens) =
                        decode_general_trace(&data, 0, data.len(), maximum as u64)
                            .map_err(|error| error.0)?;
                    if decoded.len() as i64 != maximum {
                        return err(format!(
                            "component {} decoded extent differs",
                            component.name
                        ));
                    }
                    let replay = encode_general(&decoded, &tokens).map_err(|error| error.0)?;
                    if replay.len() > data.len() || replay != data[..replay.len()] {
                        return err(format!(
                            "component {} compression replay differs",
                            component.name
                        ));
                    }
                    let lookahead = data[replay.len()..].to_vec();
                    sources.set(
                        component.name.clone(),
                        LateRuntimeSource::GeneralLzStream { decoded, tokens, lookahead },
                    );
                }
                ComponentKind::Table(kind) => {
                    sources.set(
                        component.name.clone(),
                        LateRuntimeSource::Table { kind, values: decode_table(kind, &data) },
                    );
                }
                ComponentKind::UnresolvedFill => unreachable!("handled above"),
            }
        }
    }
    Ok(LateRuntimeExtraction { sources, unresolved })
}

fn encode_table(component: &LateRuntimeComponent, values: &[i64]) -> Result<Vec<u8>> {
    if Some(values.len() as i64) != component.count {
        return err(format!("component {} table count differs", component.name));
    }
    let mut output = vec![0u8; component.size()];
    let ComponentKind::Table(kind) = component.kind else {
        return err(format!("component {} table count differs", component.name));
    };
    for (index, value) in values.iter().enumerate() {
        // PORT NOTE: the TypeScript's `Number.isSafeInteger` guard cannot fail
        // here -- `i64` values arrive already validated -- but the range checks
        // below are the ones that matter and are kept verbatim.
        match kind {
            TableKind::U8 => {
                if *value < 0 || *value > 0xff {
                    return err(format!("component {} value does not fit u8", component.name));
                }
                output[index] = *value as u8;
            }
            TableKind::S16 => {
                if *value < -0x8000 || *value > 0x7fff {
                    return err(format!("component {} value does not fit s16", component.name));
                }
                output[index * 2..index * 2 + 2].copy_from_slice(&(*value as i16).to_le_bytes());
            }
            TableKind::U16 => {
                if *value < 0 || *value > 0xffff {
                    return err(format!("component {} value does not fit u16", component.name));
                }
                output[index * 2..index * 2 + 2].copy_from_slice(&(*value as u16).to_le_bytes());
            }
        }
    }
    Ok(output)
}

pub fn build_late_runtime_data(
    catalog: &LateRuntimeCatalog,
    sources: &OrderedMap<String, LateRuntimeSource>,
) -> Result<LateRuntimeBuild> {
    let mut regions: Regions = OrderedMap::new();
    let mut unresolved = Vec::new();
    let mut typed_bytes = 0usize;
    for range in &catalog.ranges {
        for component in &range.components {
            if component.kind == ComponentKind::UnresolvedFill {
                unresolved.push(component.clone());
                continue;
            }
            let source = sources.get(&component.name);
            let Some(source) = source.filter(|source| source.kind() == component.kind) else {
                return err(format!("component {} source differs", component.name));
            };
            let data = match (component.kind, source) {
                (ComponentKind::Fill, _) => {
                    vec![component.value.unwrap_or(0) as u8; component.size()]
                }
                (
                    ComponentKind::GeneralLzStream,
                    LateRuntimeSource::GeneralLzStream { decoded, tokens, lookahead },
                ) => {
                    if Some(decoded.len() as i64) != component.decoded_bytes {
                        return err(format!(
                            "component {} decoded extent differs",
                            component.name
                        ));
                    }
                    let mut encoded = encode_general(decoded, tokens).map_err(|error| error.0)?;
                    encoded.extend_from_slice(lookahead);
                    encoded
                }
                (ComponentKind::Table(_), LateRuntimeSource::Table { values, .. }) => {
                    encode_table(component, values)?
                }
                _ => return err(format!("component {} source differs", component.name)),
            };
            if data.len() != component.size() {
                return err(format!("component {} encoded extent differs", component.name));
            }
            typed_bytes += data.len();
            regions.set(component.address, data);
        }
    }
    Ok(LateRuntimeBuild { regions, unresolved, typed_bytes })
}

// ---------------------------------------------------------------------------
// package documents
// ---------------------------------------------------------------------------

fn parse_tokens(value: &Value, decoded_bytes: i64, label: &str) -> Result<Vec<GeneralToken>> {
    let Some(items) = value.as_array() else {
        return err(format!("{label} differs"));
    };
    if items.is_empty() || items.len() as i64 > decoded_bytes {
        return err(format!("{label} differs"));
    }
    let mut decoded: i64 = 0;
    let mut tokens = Vec::new();
    for (index, raw) in items.iter().enumerate() {
        let Some(parts) = raw.as_array() else {
            return err(format!("{label} token {index} opcode differs"));
        };
        let opcode = parts.first().and_then(Value::as_str).unwrap_or("");
        if opcode != "l" && opcode != "c" {
            return err(format!("{label} token {index} opcode differs"));
        }
        if opcode == "l" {
            if parts.len() != 2 {
                return err(format!("{label} token {index} literal tuple differs"));
            }
            let count = integer(
                &parts[1],
                1,
                decoded_bytes,
                &format!("{label} token {index} literal count"),
            )?;
            if decoded + count > decoded_bytes {
                return err(format!("{label} token {index} crosses the decoded extent"));
            }
            decoded += count;
            tokens.push(GeneralToken::Literal(count as u32));
            continue;
        }
        if parts.len() != 3 {
            return err(format!("{label} token {index} copy tuple differs"));
        }
        let length = integer(&parts[1], 2, 137, &format!("{label} token {index} copy length"))?;
        let distance =
            integer(&parts[2], 1, decoded_bytes, &format!("{label} token {index} copy distance"))?;
        if distance > decoded || decoded + length > decoded_bytes {
            return err(format!("{label} token {index} crosses its replay bounds"));
        }
        decoded += length;
        tokens.push(GeneralToken::Copy { length: length as u32, distance: distance as u32 });
    }
    if decoded != decoded_bytes {
        return err(format!("{label} does not cover the decoded source"));
    }
    Ok(tokens)
}

fn token_value(token: &GeneralToken) -> Value {
    match token {
        GeneralToken::Literal(count) => Value::Array(vec![
            Value::String("l".into()),
            Value::Number((*count).into()),
        ]),
        GeneralToken::Copy { length, distance } => Value::Array(vec![
            Value::String("c".into()),
            Value::Number((*length).into()),
            Value::Number((*distance).into()),
        ]),
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct FillRecipe {
    pub value: u8,
    pub bytes: i64,
}

fn parse_fill_parts(
    pattern: &Value,
    value: &Value,
    bytes: &Value,
    expected_bytes: i64,
    label: &str,
) -> Result<FillRecipe> {
    if !string_is(pattern, "uniform")
        || (!number_is(value, 0) && !number_is(value, 255))
        || !number_is(bytes, expected_bytes)
        || expected_bytes < 1
    {
        return err(format!("{label} differs"));
    }
    let fill = if number_is(value, 0) { 0u8 } else { 255u8 };
    Ok(FillRecipe { value: fill, bytes: expected_bytes })
}

fn parse_fill_recipe(value: &Value, expected_bytes: i64, label: &str) -> Result<FillRecipe> {
    exact_keys(value, &["pattern", "value", "bytes"], label)?;
    parse_fill_parts(
        field(value, "pattern"),
        field(value, "value"),
        field(value, "bytes"),
        expected_bytes,
        label,
    )
}

fn uniform_fill(data: &[u8], label: &str) -> Result<FillRecipe> {
    if data.is_empty() {
        return err(format!("{label} is empty"));
    }
    let value = data[0];
    if (value != 0 && value != 255) || data.iter().any(|byte| *byte != value) {
        return err(format!("{label} is not a narrow structural fill pattern"));
    }
    Ok(FillRecipe { value, bytes: data.len() as i64 })
}

struct PackageIndex {
    sources: Vec<(String, String)>,
    range_bytes: i64,
}

fn parse_index(value: &Value) -> Result<PackageIndex> {
    exact_keys(value, &["format", "kind", "range_bytes", "sources"], "late runtime package index")?;
    let sources = field(value, "sources");
    exact_keys(
        sources,
        &["catalog", "stream", "plan", "tables", "fills"],
        "late runtime package sources",
    )?;
    let matches_files =
        PACKAGE_FILES.iter().all(|(key, leaf)| string_is(field(sources, key), leaf));
    if !number_is(field(value, "format"), 1)
        || !string_is(field(value, "kind"), "golden-sun-late-runtime-data")
        || !number_is(field(value, "range_bytes"), RANGE_BYTES)
        || !matches_files
    {
        return err("late runtime package index differs");
    }
    Ok(PackageIndex {
        sources: PACKAGE_FILES
            .iter()
            .map(|(key, _)| {
                ((*key).to_string(), field(sources, key).as_str().unwrap_or("").to_string())
            })
            .collect(),
        range_bytes: RANGE_BYTES,
    })
}

impl PackageIndex {
    fn source(&self, key: &str) -> &str {
        self.sources
            .iter()
            .find(|(name, _)| name == key)
            .map(|(_, leaf)| leaf.as_str())
            .expect("validated source key")
    }
}

struct StreamDocument {
    decoded_bytes: i64,
    words: Vec<i64>,
}

fn parse_stream_document(value: &Value, component: &LateRuntimeComponent) -> Result<StreamDocument> {
    exact_keys(
        value,
        &["format", "kind", "name", "address", "end", "role", "decoded_bytes", "element_type", "words"],
        "late runtime stream source",
    )?;
    let words = field(value, "words");
    let word_count = words.as_array().map(Vec::len).unwrap_or(0) as i64;
    if !number_is(field(value, "format"), 1)
        || !string_is(field(value, "kind"), "golden-sun-late-runtime-u16-source")
        || !string_is(field(value, "name"), &component.name)
        || !string_is(field(value, "address"), &hexadecimal(component.address))
        || !string_is(field(value, "end"), &hexadecimal(component.end))
        || !string_is(field(value, "role"), &component.role)
        || !number_is(field(value, "decoded_bytes"), component.decoded_bytes.unwrap_or(-1))
        || !string_is(field(value, "element_type"), "u16le")
        || !words.is_array()
        || word_count * 2 != component.decoded_bytes.unwrap_or(-1)
    {
        return err("late runtime stream source differs");
    }
    let mut parsed = Vec::new();
    for (index, word) in words.as_array().expect("checked above").iter().enumerate() {
        parsed.push(integer(word, 0, 0xffff, &format!("late runtime stream word {index}"))?);
    }
    Ok(StreamDocument { decoded_bytes: component.decoded_bytes.unwrap_or(0), words: parsed })
}

struct PlanDocument {
    tokens: Vec<GeneralToken>,
    trailing_fill: Option<Value>,
}

fn parse_plan_document(value: &Value, component: &LateRuntimeComponent) -> Result<PlanDocument> {
    exact_keys(
        value,
        &["format", "kind", "name", "codec", "address", "end", "decoded_bytes", "tokens", "trailing_fill"],
        "late runtime compression plan",
    )?;
    if !number_is(field(value, "format"), 1)
        || !string_is(field(value, "kind"), "golden-sun-late-runtime-lz-plan")
        || !string_is(field(value, "name"), &component.name)
        || !string_is(field(value, "codec"), "golden-sun-general-lz")
        || !string_is(field(value, "address"), &hexadecimal(component.address))
        || !string_is(field(value, "end"), &hexadecimal(component.end))
        || !number_is(field(value, "decoded_bytes"), component.decoded_bytes.unwrap_or(-1))
    {
        return err("late runtime compression plan differs");
    }
    let tokens = parse_tokens(
        field(value, "tokens"),
        component.decoded_bytes.unwrap_or(0),
        "late runtime compression plan",
    )?;
    let fill = field(value, "trailing_fill");
    // `typeof null === "object"`, so the TypeScript's guard rejects everything
    // that is neither `null` nor a non-array object.
    if !fill.is_null() && !fill.is_object() {
        return err("late runtime stream trailing fill differs");
    }
    Ok(PlanDocument {
        tokens,
        trailing_fill: if fill.is_null() { None } else { Some(fill.clone()) },
    })
}

fn parse_tables_document(value: &Value, catalog: &LateRuntimeCatalog) -> Result<Vec<Vec<i64>>> {
    exact_keys(value, &["format", "kind", "tables"], "late runtime tables source")?;
    let expected = catalog.tables();
    let raw_tables = field(value, "tables");
    if !number_is(field(value, "format"), 1)
        || !string_is(field(value, "kind"), "golden-sun-late-runtime-tables")
        || !raw_tables.is_array()
        || raw_tables.as_array().expect("checked").len() != expected.len()
    {
        return err("late runtime tables source differs");
    }
    let mut tables = Vec::new();
    for (index, raw) in raw_tables.as_array().expect("checked").iter().enumerate() {
        exact_keys(
            raw,
            &["name", "type", "address", "end", "role", "values"],
            &format!("late runtime table {index}"),
        )?;
        let (component, kind) = expected[index];
        let values = field(raw, "values");
        if !string_is(field(raw, "name"), &component.name)
            || !string_is(field(raw, "type"), kind.document_type())
            || !string_is(field(raw, "address"), &hexadecimal(component.address))
            || !string_is(field(raw, "end"), &hexadecimal(component.end))
            || !string_is(field(raw, "role"), &component.role)
            || !values.is_array()
            || Some(values.as_array().expect("checked").len() as i64) != component.count
        {
            return err(format!("late runtime table {} differs", component.name));
        }
        let minimum = if kind == TableKind::S16 { -0x8000 } else { 0 };
        let maximum = match kind {
            TableKind::U8 => 0xff,
            TableKind::S16 => 0x7fff,
            TableKind::U16 => 0xffff,
        };
        let mut parsed = Vec::new();
        for (position, item) in values.as_array().expect("checked").iter().enumerate() {
            parsed.push(integer(
                item,
                minimum,
                maximum,
                &format!("{} value {position}", component.name),
            )?);
        }
        tables.push(parsed);
    }
    Ok(tables)
}

fn parse_fills_document(value: &Value, catalog: &LateRuntimeCatalog) -> Result<Vec<FillRecipe>> {
    exact_keys(value, &["format", "kind", "spans"], "late runtime fills source")?;
    let expected: Vec<&LateRuntimeComponent> = catalog
        .components()
        .into_iter()
        .filter(|component| component.kind == ComponentKind::UnresolvedFill)
        .collect();
    let raw_spans = field(value, "spans");
    if !number_is(field(value, "format"), 1)
        || !string_is(field(value, "kind"), "golden-sun-late-runtime-structural-fills")
        || !raw_spans.is_array()
        || raw_spans.as_array().expect("checked").len() != expected.len()
    {
        return err("late runtime fills source differs");
    }
    let mut spans = Vec::new();
    for (index, raw) in raw_spans.as_array().expect("checked").iter().enumerate() {
        exact_keys(
            raw,
            &["name", "address", "end", "role", "pattern", "value", "bytes"],
            &format!("late runtime fill {index}"),
        )?;
        let component = expected[index];
        if !string_is(field(raw, "name"), &component.name)
            || !string_is(field(raw, "address"), &hexadecimal(component.address))
            || !string_is(field(raw, "end"), &hexadecimal(component.end))
            || !string_is(field(raw, "role"), &component.role)
        {
            return err(format!("late runtime fill {} differs", component.name));
        }
        spans.push(parse_fill_parts(
            field(raw, "pattern"),
            field(raw, "value"),
            field(raw, "bytes"),
            component.size() as i64,
            &format!("late runtime fill {}", component.name),
        )?);
    }
    Ok(spans)
}

fn decoded_words(source: &StreamDocument) -> Vec<u8> {
    let mut output = vec![0u8; source.decoded_bytes as usize];
    for (index, word) in source.words.iter().enumerate() {
        output[index * 2..index * 2 + 2].copy_from_slice(&(*word as u16).to_le_bytes());
    }
    output
}

/// `packageChild` -- a package member must be a plain leaf name that resolves,
/// after realpath, back to exactly that leaf inside the package directory.
fn package_child(index_path: &Path, leaf: &str, label: &str) -> Result<PathBuf> {
    if Path::new(leaf).file_name().and_then(|name| name.to_str()) != Some(leaf) {
        return err(format!("{label} must be a local leaf"));
    }
    let parent = index_path.parent().filter(|path| !path.as_os_str().is_empty()).map_or_else(
        || PathBuf::from("."),
        Path::to_path_buf,
    );
    let directory =
        std::fs::canonicalize(&parent).map_err(|error| format!("{}: {error}", parent.display()))?;
    let candidate = directory.join(leaf);
    regular_file(&candidate, label)?;
    let physical = std::fs::canonicalize(&candidate)
        .map_err(|error| format!("{}: {error}", candidate.display()))?;
    // PORT NOTE: `relative(directory, physical) !== leaf` in the TypeScript.
    // `strip_prefix` is the same test for this shape: anything outside the
    // directory fails to strip, and anything inside must strip to exactly the
    // leaf.
    if physical.strip_prefix(&directory).ok().and_then(|rest| rest.to_str()) != Some(leaf) {
        return err(format!("{label} escaped its package"));
    }
    Ok(physical)
}

pub fn build_late_runtime_package(index_path: &Path) -> Result<LateRuntimePackageBuild> {
    regular_file(index_path, "late runtime package index")?;
    let index = parse_index(&canonical_document(index_path, "late runtime package index")?)?;
    let catalog = read_late_runtime_catalog(&package_child(
        index_path,
        index.source("catalog"),
        "late runtime catalog",
    )?)?;
    let stream_component = catalog.single_stream()?.clone();
    let stream = parse_stream_document(
        &canonical_document(
            &package_child(index_path, index.source("stream"), "late runtime stream source")?,
            "late runtime stream source",
        )?,
        &stream_component,
    )?;
    let plan = parse_plan_document(
        &canonical_document(
            &package_child(index_path, index.source("plan"), "late runtime compression plan")?,
            "late runtime compression plan",
        )?,
        &stream_component,
    )?;
    let tables = parse_tables_document(
        &canonical_document(
            &package_child(index_path, index.source("tables"), "late runtime tables source")?,
            "late runtime tables source",
        )?,
        &catalog,
    )?;
    let fills = parse_fills_document(
        &canonical_document(
            &package_child(index_path, index.source("fills"), "late runtime fills source")?,
            "late runtime fills source",
        )?,
        &catalog,
    )?;
    let mut components: Regions = OrderedMap::new();
    let decoded = decoded_words(&stream);
    let encoded = encode_general(&decoded, &plan.tokens).map_err(|error| error.0)?;
    let trailing_bytes = stream_component.size() as i64 - encoded.len() as i64;
    let mut trailing: Vec<u8> = Vec::new();
    if trailing_bytes < 0 {
        return err("late runtime compressed stream crossed its extent");
    }
    if trailing_bytes == 0 {
        if plan.trailing_fill.is_some() {
            return err("late runtime compression plan has a spurious trailing fill");
        }
    } else {
        let Some(fill) = plan.trailing_fill.as_ref() else {
            return err("late runtime compression plan lacks its trailing fill");
        };
        let recipe =
            parse_fill_recipe(fill, trailing_bytes, "late runtime stream trailing fill")?;
        trailing = vec![recipe.value; recipe.bytes as usize];
    }
    let mut stream_bytes = encoded;
    stream_bytes.extend_from_slice(&trailing);
    components.set(stream_component.address, stream_bytes);
    for (index, (component, _)) in catalog.tables().into_iter().enumerate() {
        components.set(component.address, encode_table(component, &tables[index])?);
    }
    let fill_components: Vec<&LateRuntimeComponent> = catalog
        .components()
        .into_iter()
        .filter(|component| component.kind == ComponentKind::UnresolvedFill)
        .collect();
    for (index, component) in fill_components.into_iter().enumerate() {
        components.set(component.address, vec![fills[index].value; component.size()]);
    }
    for component in catalog.components().into_iter().filter(|item| item.kind == ComponentKind::Fill)
    {
        components.set(component.address, vec![component.value.unwrap_or(0) as u8; component.size()]);
    }
    let mut ranges: Regions = OrderedMap::new();
    for range in &catalog.ranges {
        let mut data = Vec::new();
        for component in &range.components {
            let part = components.get(&component.address);
            let Some(part) = part.filter(|part| part.len() == component.size()) else {
                return err(format!("late runtime component {} is incomplete", component.name));
            };
            data.extend_from_slice(part);
        }
        if data.len() != (range.end - range.address) as usize {
            return err(format!("late runtime range {} is incomplete", range.name));
        }
        ranges.set(range.address, data);
    }
    let source_bytes: i64 = ranges.iter().map(|(_, data)| data.len() as i64).sum();
    if source_bytes != index.range_bytes {
        return err("late runtime source-byte total differs");
    }
    Ok(LateRuntimePackageBuild { ranges, components, source_bytes })
}

// ---------------------------------------------------------------------------
// document emission
// ---------------------------------------------------------------------------

/// `streamDocuments` -- returns the `haikei.json` / `haikei.lz.json` pair.
/// Key order below is the emitted order and is load-bearing.
fn stream_documents(
    source: &LateRuntimeSource,
    component: &LateRuntimeComponent,
) -> Result<(Value, Value)> {
    let LateRuntimeSource::GeneralLzStream { decoded, tokens, lookahead } = source else {
        return err("late runtime extracted stream source differs");
    };
    if Some(decoded.len() as i64) != component.decoded_bytes || decoded.len() % 2 != 0 {
        return err("late runtime decoded stream is not a bounded halfword source");
    }
    let words: Vec<Value> = decoded
        .chunks_exact(2)
        .map(|pair| Value::Number(u16::from_le_bytes([pair[0], pair[1]]).into()))
        .collect();
    let trailing_fill = if lookahead.is_empty() {
        Value::Null
    } else {
        let recipe = uniform_fill(lookahead, "late runtime stream trailing fill")?;
        object_of(vec![
            ("pattern", Value::String("uniform".into())),
            ("value", Value::Number(recipe.value.into())),
            ("bytes", Value::Number(recipe.bytes.into())),
        ])
    };
    let decoded_bytes = Value::Number(component.decoded_bytes.unwrap_or(0).into());
    let stream = object_of(vec![
        ("format", Value::Number(1.into())),
        ("kind", Value::String("golden-sun-late-runtime-u16-source".into())),
        ("name", Value::String(component.name.clone())),
        ("address", Value::String(hexadecimal(component.address))),
        ("end", Value::String(hexadecimal(component.end))),
        ("role", Value::String(component.role.clone())),
        ("decoded_bytes", decoded_bytes.clone()),
        ("element_type", Value::String("u16le".into())),
        ("words", Value::Array(words)),
    ]);
    let plan = object_of(vec![
        ("format", Value::Number(1.into())),
        ("kind", Value::String("golden-sun-late-runtime-lz-plan".into())),
        ("name", Value::String(component.name.clone())),
        ("codec", Value::String("golden-sun-general-lz".into())),
        ("address", Value::String(hexadecimal(component.address))),
        ("end", Value::String(hexadecimal(component.end))),
        ("decoded_bytes", decoded_bytes),
        ("tokens", Value::Array(tokens.iter().map(token_value).collect())),
        ("trailing_fill", trailing_fill),
    ]);
    Ok((stream, plan))
}

fn table_document(extraction: &LateRuntimeExtraction, catalog: &LateRuntimeCatalog) -> Result<Value> {
    let mut tables = Vec::new();
    for (component, kind) in catalog.tables() {
        let source = extraction.sources.get(&component.name);
        let Some(LateRuntimeSource::Table { values, .. }) =
            source.filter(|source| source.kind() == component.kind)
        else {
            return err(format!("late runtime extracted table {} differs", component.name));
        };
        tables.push(object_of(vec![
            ("name", Value::String(component.name.clone())),
            ("type", Value::String(kind.document_type().into())),
            ("address", Value::String(hexadecimal(component.address))),
            ("end", Value::String(hexadecimal(component.end))),
            ("role", Value::String(component.role.clone())),
            ("values", Value::Array(values.iter().map(|value| Value::Number((*value).into())).collect())),
        ]));
    }
    Ok(object_of(vec![
        ("format", Value::Number(1.into())),
        ("kind", Value::String("golden-sun-late-runtime-tables".into())),
        ("tables", Value::Array(tables)),
    ]))
}

fn fill_document(image: &[u8], catalog: &LateRuntimeCatalog, image_base: u32) -> Result<Value> {
    let mut spans = Vec::new();
    for component in catalog
        .components()
        .into_iter()
        .filter(|component| component.kind == ComponentKind::UnresolvedFill)
    {
        let recipe = uniform_fill(
            &component_bytes(image, component, image_base)?,
            &format!("late runtime fill {}", component.name),
        )?;
        spans.push(object_of(vec![
            ("name", Value::String(component.name.clone())),
            ("address", Value::String(hexadecimal(component.address))),
            ("end", Value::String(hexadecimal(component.end))),
            ("role", Value::String(component.role.clone())),
            ("pattern", Value::String("uniform".into())),
            ("value", Value::Number(recipe.value.into())),
            ("bytes", Value::Number(recipe.bytes.into())),
        ]));
    }
    Ok(object_of(vec![
        ("format", Value::Number(1.into())),
        ("kind", Value::String("golden-sun-late-runtime-structural-fills".into())),
        ("spans", Value::Array(spans)),
    ]))
}

fn package_index_document() -> Value {
    object_of(vec![
        ("format", Value::Number(1.into())),
        ("kind", Value::String("golden-sun-late-runtime-data".into())),
        ("range_bytes", Value::Number(RANGE_BYTES.into())),
        (
            "sources",
            object_of(
                PACKAGE_FILES
                    .iter()
                    .map(|(key, leaf)| (*key, Value::String((*leaf).into())))
                    .collect(),
            ),
        ),
    ])
}

fn write_package(
    image: &[u8],
    catalog_path: &Path,
    directory: &Path,
    image_base: u32,
) -> Result<LateRuntimePackageBuild> {
    let catalog_value = canonical_document(catalog_path, "late runtime catalog")?;
    let catalog = parse_late_runtime_catalog(&catalog_value)?;
    let extraction = extract_late_runtime_data(image, &catalog, image_base)?;
    let stream_component = catalog.single_stream()?.clone();
    let Some(stream_source) = extraction.sources.get(&stream_component.name) else {
        return err("late runtime stream was not extracted");
    };
    let (stream, plan) = stream_documents(stream_source, &stream_component)?;
    let tables = table_document(&extraction, &catalog)?;
    let fills = fill_document(image, &catalog, image_base)?;
    create_dir_all(directory)?;
    write_file(&directory.join("index.json"), pretty(&package_index_document()).as_bytes())?;
    write_file(&directory.join(package_file("catalog")), pretty(&catalog_value).as_bytes())?;
    write_file(&directory.join(package_file("stream")), pretty(&stream).as_bytes())?;
    write_file(&directory.join(package_file("plan")), pretty(&plan).as_bytes())?;
    write_file(&directory.join(package_file("tables")), pretty(&tables).as_bytes())?;
    write_file(&directory.join(package_file("fills")), pretty(&fills).as_bytes())?;
    let index_path = directory.join("index.json");
    let built = build_late_runtime_package(&index_path)?;
    verify_late_runtime_package_image(image, &index_path, image_base, Some(built))
}

// ---------------------------------------------------------------------------
// destinations and transactions
// ---------------------------------------------------------------------------

fn validate_export_destination(
    rom_path: &Path,
    catalog_path: &Path,
    directory: &Path,
) -> Result<()> {
    if same_path(rom_path, directory) || contains_path(directory, rom_path) {
        return err("late runtime export directory must not contain its input ROM");
    }
    let destination = resolve_path(directory);
    if !destination.exists() {
        return Ok(());
    }
    if !std::fs::symlink_metadata(&destination)
        .map(|meta| meta.file_type().is_dir())
        .unwrap_or(false)
    {
        return err("late runtime export destination must be a directory");
    }
    let mut names = read_dir_names(&destination)?;
    names.sort_unstable();
    if names.is_empty() {
        return Ok(());
    }
    let catalog_leaf = catalog_path
        .file_name()
        .map(|name| name.to_string_lossy().into_owned())
        .unwrap_or_default();
    if names.len() == 1
        && names[0] == catalog_leaf
        && same_path(&destination.join(&names[0]), catalog_path)
    {
        read_late_runtime_catalog(catalog_path)?;
        return Ok(());
    }
    let index = destination.join("index.json");
    if !index.exists()
        || !std::fs::symlink_metadata(&index).map(|meta| meta.file_type().is_file()).unwrap_or(false)
    {
        return err("refusing to replace a directory that is not a late runtime package");
    }
    if build_late_runtime_package(&index).is_err() {
        return err("refusing to replace a directory that is not a canonical late runtime package");
    }
    Ok(())
}

/// Stage into a sibling temporary directory, then swap by rename, restoring the
/// previous contents if anything fails. A partially written package must never
/// be visible at the destination.
fn replace_directory(
    directory: &Path,
    prefix: &str,
    writer: &mut dyn FnMut(&Path) -> Result<()>,
) -> Result<()> {
    let destination = resolve_path(directory);
    let parent = destination.parent().map_or_else(|| destination.clone(), Path::to_path_buf);
    if destination == parent {
        return err("late runtime operation requires a dedicated directory");
    }
    create_dir_all(&parent)?;
    let transaction = make_temporary_directory(&parent, prefix)?;
    let staged = transaction.join("new");
    let previous = transaction.join("previous");
    let mut installed = false;
    let mut outcome: Result<()> = (|| {
        writer(&staged)?;
        if destination.exists() {
            std::fs::rename(&destination, &previous).map_err(|error| format!("{error}"))?;
        }
        match std::fs::rename(&staged, &destination) {
            Ok(()) => {
                installed = true;
                Ok(())
            }
            Err(error) => {
                if previous.exists() {
                    let _ = std::fs::rename(&previous, &destination);
                }
                Err(format!("{error}"))
            }
        }
    })();
    if !installed && previous.exists() && !destination.exists() {
        if let Err(error) = std::fs::rename(&previous, &destination) {
            if outcome.is_ok() {
                outcome = Err(format!("{error}"));
            }
        }
    }
    let _ = std::fs::remove_dir_all(&transaction);
    outcome
}

pub fn export_late_runtime_package(
    rom_path: &Path,
    catalog_path: &Path,
    directory: &Path,
) -> Result<()> {
    regular_file(rom_path, "late runtime input ROM")?;
    regular_file(catalog_path, "late runtime catalog")?;
    validate_export_destination(rom_path, catalog_path, directory)?;
    let image = std::fs::read(rom_path).map_err(|error| format!("{error}"))?;
    if image.len() != ROM_SIZE {
        return err("late runtime exporter requires an 8 MiB ROM");
    }
    let catalog_path = catalog_path.to_path_buf();
    replace_directory(directory, ".late-runtime-export-", &mut |staged| {
        write_package(&image, &catalog_path, staged, ROM_BASE).map(|_| ())
    })
}

pub fn verify_late_runtime_package_image(
    image: &[u8],
    index_path: &Path,
    image_base: u32,
    previous: Option<LateRuntimePackageBuild>,
) -> Result<LateRuntimePackageBuild> {
    let built = match previous {
        Some(built) => built,
        None => build_late_runtime_package(index_path)?,
    };
    for (start, data) in built.ranges.iter() {
        let first = i64::from(*start) - i64::from(image_base);
        let end = first + data.len() as i64;
        let inside = first >= 0 && end <= image.len() as i64;
        let identical = inside && data.as_slice() == &image[first as usize..end as usize];
        if !identical {
            let mut offset = 0usize;
            while offset < data.len()
                && first + offset as i64 >= 0
                && first + (offset as i64) < image.len() as i64
                && data[offset] == image[(first + offset as i64) as usize]
            {
                offset += 1;
            }
            return err(format!(
                "late runtime package differs at {}",
                hexadecimal(start.wrapping_add(offset as u32))
            ));
        }
    }
    Ok(built)
}

pub fn verify_late_runtime_package(rom_path: &Path, index_path: &Path) -> Result<LateRuntimePackageBuild> {
    regular_file(rom_path, "late runtime input ROM")?;
    let image = std::fs::read(rom_path).map_err(|error| format!("{error}"))?;
    if image.len() != ROM_SIZE {
        return err("late runtime verifier requires an 8 MiB ROM");
    }
    verify_late_runtime_package_image(&image, index_path, ROM_BASE, None)
}

fn build_report(built: &LateRuntimePackageBuild) -> Value {
    object_of(vec![
        ("format", Value::Number(1.into())),
        ("kind", Value::String("golden-sun-late-runtime-build".into())),
        ("source_bytes", Value::Number(built.source_bytes.into())),
        (
            "ranges",
            Value::Array(
                built
                    .ranges
                    .iter()
                    .map(|(address, data)| {
                        object_of(vec![
                            ("address", Value::String(hexadecimal(*address))),
                            ("end", Value::String(hexadecimal(address + data.len() as u32))),
                            ("size", Value::Number(data.len().into())),
                            ("output", Value::String(format!("{address:08x}.bin"))),
                        ])
                    })
                    .collect(),
            ),
        ),
    ])
}

fn validate_build_destination(directory: &Path) -> Result<()> {
    if !directory.exists() {
        return Ok(());
    }
    if !std::fs::symlink_metadata(directory).map(|meta| meta.file_type().is_dir()).unwrap_or(false) {
        return err("late runtime build destination must be a directory");
    }
    if read_dir_names(directory)?.is_empty() {
        return Ok(());
    }
    let report = directory.join("report.json");
    if !report.exists() {
        return err("refusing to replace a directory that is not a late runtime build");
    }
    let value = canonical_document(&report, "late runtime build report")?;
    exact_keys(
        &value,
        &["format", "kind", "source_bytes", "ranges"],
        "late runtime build report",
    )?;
    if !number_is(field(&value, "format"), 1)
        || !string_is(field(&value, "kind"), "golden-sun-late-runtime-build")
    {
        return err("refusing to replace a directory that is not a canonical late runtime build");
    }
    Ok(())
}

pub fn write_late_runtime_build(
    index_path: &Path,
    directory: &Path,
) -> Result<LateRuntimePackageBuild> {
    let built = build_late_runtime_package(index_path)?;
    validate_build_destination(directory)?;
    let report = pretty(&build_report(&built));
    let ranges: Vec<(u32, Vec<u8>)> =
        built.ranges.iter().map(|(address, data)| (*address, data.clone())).collect();
    replace_directory(directory, ".late-runtime-build-", &mut |staged| {
        create_dir_all(staged)?;
        for (address, data) in &ranges {
            write_file(&staged.join(format!("{address:08x}.bin")), data)?;
        }
        write_file(&staged.join("report.json"), report.as_bytes())
    })?;
    Ok(built)
}

// ---------------------------------------------------------------------------
// self-test
// ---------------------------------------------------------------------------

fn zero_sources(catalog: &LateRuntimeCatalog) -> Result<OrderedMap<String, LateRuntimeSource>> {
    let mut sources: OrderedMap<String, LateRuntimeSource> = OrderedMap::new();
    for range in &catalog.ranges {
        for component in &range.components {
            match component.kind {
                ComponentKind::UnresolvedFill => continue,
                ComponentKind::Fill => sources.set(component.name.clone(), LateRuntimeSource::Fill),
                ComponentKind::GeneralLzStream => {
                    let decoded = vec![0u8; component.decoded_bytes.unwrap_or(0) as usize];
                    let mut tokens = vec![GeneralToken::Literal(1)];
                    let mut remaining = decoded.len() as i64 - 1;
                    while remaining > 0 {
                        let count = remaining.min(137);
                        tokens.push(GeneralToken::Copy { length: count as u32, distance: 1 });
                        remaining -= count;
                    }
                    let encoded = encode_general(&decoded, &tokens).map_err(|error| error.0)?;
                    let lookahead = vec![0u8; component.size() - encoded.len()];
                    sources.set(
                        component.name.clone(),
                        LateRuntimeSource::GeneralLzStream { decoded, tokens, lookahead },
                    );
                }
                ComponentKind::Table(kind) => sources.set(
                    component.name.clone(),
                    LateRuntimeSource::Table {
                        kind,
                        values: vec![0; component.count.unwrap_or(0) as usize],
                    },
                ),
            }
        }
    }
    Ok(sources)
}

/// The TypeScript's `selfTest`, including the exact stdout line and the
/// adversarial count. Every `reject` must actually fail; the final count check
/// is what stops a future edit from silently dropping one.
pub fn self_test() -> Result<String> {
    let temporary = make_temporary_directory(&std::env::temp_dir(), "alchemy-late-runtime-")?;
    let outcome = self_test_body(&temporary);
    let _ = std::fs::remove_dir_all(&temporary);
    outcome?;
    Ok("self-test=ok source_bytes=7294 typed_json=579 structural_fill=6715 adversarial=14".into())
}

fn self_test_body(temporary: &Path) -> Result<()> {
    let catalog_path = resolve_path(Path::new("assets/data/late_runtime_catalog.json"));
    let mut rejected = 0usize;
    let catalog = read_late_runtime_catalog(&catalog_path)?;
    let mut sources = zero_sources(&catalog)?;
    for component in catalog.components() {
        let address = i64::from(component.address);
        match sources.get_mut(&component.name) {
            Some(LateRuntimeSource::GeneralLzStream { lookahead, .. }) => lookahead.fill(255),
            Some(LateRuntimeSource::Table { kind, values }) => {
                if *kind == TableKind::S16 {
                    *values = vec![-0x8000, -1, 0, 1, 1234, 0x7fff];
                } else {
                    let width = *kind;
                    *values = values
                        .iter()
                        .enumerate()
                        .map(|(index, _)| {
                            if width == TableKind::U8 {
                                (index as i64 * 17 + address) & 0xff
                            } else {
                                (index as i64 * 257 + address) & 0xffff
                            }
                        })
                        .collect();
                }
            }
            _ => {}
        }
    }
    let typed = build_late_runtime_data(&catalog, &sources)?;
    let range_bytes: i64 =
        catalog.ranges.iter().map(|range| i64::from(range.end - range.address)).sum();
    let unresolved_bytes: i64 = typed.unresolved.iter().map(|c| c.size() as i64).sum();
    if range_bytes != 7_294
        || typed.typed_bytes != 579
        || unresolved_bytes != 6_715
        || typed.regions.len() != 13
    {
        return err("late runtime catalog totals differ");
    }
    let base = 0x080f_0000u32;
    let mut image = vec![0u8; 0x9000];
    for (start, data) in typed.regions.iter() {
        let offset = (start - base) as usize;
        image[offset..offset + data.len()].copy_from_slice(data);
    }
    for (index, component) in typed.unresolved.iter().enumerate() {
        let value = if index & 1 != 0 { 255u8 } else { 0u8 };
        let first = (component.address - base) as usize;
        image[first..(component.end - base) as usize].fill(value);
    }
    let package_directory = temporary.join("package");
    let packaged = write_package(&image, &catalog_path, &package_directory, base)?;
    if packaged.source_bytes != 7_294
        || packaged.ranges.len() != 3
        || packaged.components.len() != 18
    {
        return err("late runtime package extent differs");
    }
    let index_path = package_directory.join("index.json");
    verify_late_runtime_package_image(&image, &index_path, base, None)?;
    let concatenated: Vec<u8> =
        packaged.ranges.iter().flat_map(|(_, data)| data.iter().copied()).collect();
    let expected: Vec<u8> = catalog
        .ranges
        .iter()
        .flat_map(|range| {
            image[(range.address - base) as usize..(range.end - base) as usize].iter().copied()
        })
        .collect();
    if concatenated.len() != 7_294 || concatenated != expected {
        return err("late runtime full-range round-trip differs");
    }

    let build_directory = temporary.join("build");
    write_late_runtime_build(&index_path, &build_directory)?;
    if !build_directory.join("report.json").exists() || read_dir_names(&build_directory)?.len() != 4
    {
        return err("late runtime build output differs");
    }

    let mut rom = vec![0u8; ROM_SIZE];
    let rom_offset = (base - ROM_BASE) as usize;
    rom[rom_offset..rom_offset + image.len()].copy_from_slice(&image);
    let rom_path = temporary.join("gs1-en.gba");
    write_file(&rom_path, &rom)?;
    let exported = temporary.join("exported");
    create_dir_all(&exported)?;
    write_file(
        &exported.join("catalog.json"),
        &std::fs::read(&catalog_path).map_err(|error| format!("{error}"))?,
    )?;
    export_late_runtime_package(&rom_path, &exported.join("catalog.json"), &exported)?;
    verify_late_runtime_package(&rom_path, &exported.join("index.json"))?;

    let mut reject = |outcome: Result<()>| {
        if outcome.is_err() {
            rejected += 1;
        }
    };

    let mut malformed_catalog = canonical_document(&catalog_path, "test catalog")?;
    malformed_catalog["ranges"][0]["components"][1]["address"] =
        Value::String("0x080f39ac".into());
    reject(parse_late_runtime_catalog(&malformed_catalog).map(|_| ()));

    let first_fill = typed.unresolved[0].clone();
    let wide_fill = typed
        .unresolved
        .iter()
        .find(|component| component.end - component.address > 1)
        .expect("a multi-byte structural fill")
        .clone();

    let mut nonuniform = image.clone();
    nonuniform[(wide_fill.address - base) as usize + 1] = 1;
    reject(
        write_package(&nonuniform, &catalog_path, &temporary.join("nonuniform"), base).map(|_| ()),
    );

    let mut unsupported = image.clone();
    unsupported[(first_fill.address - base) as usize..(first_fill.end - base) as usize].fill(0x7f);
    reject(
        write_package(&unsupported, &catalog_path, &temporary.join("unsupported"), base).map(|_| ()),
    );

    let mut stream_tail = image.clone();
    let stream_component = catalog.single_stream()?.clone();
    stream_tail[(stream_component.end - base) as usize - 1] = 0;
    reject(
        write_package(&stream_tail, &catalog_path, &temporary.join("stream-tail"), base).map(|_| ()),
    );

    let tables_path = package_directory.join(package_file("tables"));
    let mut malformed_tables = canonical_document(&tables_path, "test tables")?;
    malformed_tables["tables"][0]["values"][0] = Value::Number(256.into());
    write_file(&tables_path, pretty(&malformed_tables).as_bytes())?;
    reject(build_late_runtime_package(&index_path).map(|_| ()));
    write_package(&image, &catalog_path, &package_directory, base)?;

    let fills_path = package_directory.join(package_file("fills"));
    let mut malformed_fills = canonical_document(&fills_path, "test fills")?;
    malformed_fills["spans"][0]["value"] = Value::Number(1.into());
    write_file(&fills_path, pretty(&malformed_fills).as_bytes())?;
    reject(build_late_runtime_package(&index_path).map(|_| ()));
    write_package(&image, &catalog_path, &package_directory, base)?;

    let mut escaped_index = canonical_document(&index_path, "test index")?;
    escaped_index["sources"]["fills"] = Value::String("../fills.json".into());
    write_file(&index_path, pretty(&escaped_index).as_bytes())?;
    reject(build_late_runtime_package(&index_path).map(|_| ()));
    write_package(&image, &catalog_path, &package_directory, base)?;

    let outside = temporary.join("outside-fills.json");
    let linked_fills = package_directory.join(package_file("fills"));
    write_file(&outside, &std::fs::read(&linked_fills).map_err(|error| format!("{error}"))?)?;
    std::fs::remove_file(&linked_fills).map_err(|error| format!("{error}"))?;
    std::os::unix::fs::symlink(&outside, &linked_fills).map_err(|error| format!("{error}"))?;
    reject(build_late_runtime_package(&index_path).map(|_| ()));
    std::fs::remove_file(&linked_fills).map_err(|error| format!("{error}"))?;
    write_package(&image, &catalog_path, &package_directory, base)?;

    let mut changed = image.clone();
    changed[(catalog.ranges[0].address - base) as usize] ^= 1;
    reject(verify_late_runtime_package_image(&changed, &index_path, base, None).map(|_| ()));

    let rollback = temporary.join("rollback");
    create_dir_all(&rollback)?;
    write_file(&rollback.join("stale"), b"old")?;
    reject(replace_directory(&rollback, ".late-runtime-test-", &mut |staged| {
        create_dir_all(staged)?;
        write_file(&staged.join("partial"), b"new")?;
        err("synthetic failure")
    }));
    if !rollback.join("stale").exists() || rollback.join("partial").exists() {
        return err("failed late runtime transaction changed its destination");
    }

    let unsafe_directory = temporary.join("unsafe");
    create_dir_all(&unsafe_directory)?;
    write_file(&unsafe_directory.join("foreign"), b"x")?;
    reject(validate_export_destination(&rom_path, &catalog_path, &unsafe_directory));
    reject(validate_build_destination(&unsafe_directory));

    let inside = temporary.join("inside");
    create_dir_all(&inside)?;
    let physical_rom = inside.join("physical.gba");
    let rom_link = temporary.join("rom-link.gba");
    write_file(&physical_rom, b"rom")?;
    std::os::unix::fs::symlink(&physical_rom, &rom_link).map_err(|error| format!("{error}"))?;
    reject(validate_export_destination(&rom_link, &catalog_path, &inside));

    let duplicate = temporary.join("duplicate.json");
    write_file(&duplicate, b"{\"format\":1,\"format\":1}\n")?;
    reject(canonical_document(&duplicate, "duplicate source").map(|_| ()));

    if rejected != 14 {
        return err(format!("late runtime adversarial count differs: {rejected}"));
    }
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;
    use serde_json::json;

    fn repository_root() -> PathBuf {
        Path::new(env!("CARGO_MANIFEST_DIR")).parent().unwrap().parent().unwrap().to_path_buf()
    }

    fn tracked_catalog() -> LateRuntimeCatalog {
        read_late_runtime_catalog(&repository_root().join("assets/data/late_runtime_catalog.json"))
            .expect("the tracked catalog parses")
    }

    #[test]
    fn the_tracked_catalog_matches_the_canonical_layout() {
        let catalog = tracked_catalog();
        assert_eq!(catalog.ranges.len(), 3);
        assert_eq!(catalog.components().len(), 18);
        assert_eq!(catalog.tables().len(), 12);
        assert_eq!(catalog.single_stream().expect("one stream").name, "haikei_stream");
    }

    /// The self-test runs from the repository root in the TypeScript; run it
    /// from there here too, and assert the exact stdout line.
    #[test]
    fn self_test_matches_the_typescript() {
        std::env::set_current_dir(repository_root()).expect("chdir to the repository root");
        assert_eq!(
            self_test().expect("the self-test passes"),
            "self-test=ok source_bytes=7294 typed_json=579 structural_fill=6715 adversarial=14"
        );
    }

    /// Guard against a vacuous pass: count the checks, and fail if a future
    /// edit drops one. Each entry names a rejection the TypeScript performs.
    #[test]
    fn every_validator_rejects_its_own_malformed_input() {
        let mut checked = 0usize;
        let mut expect_error = |outcome: Result<()>, what: &str| {
            assert!(outcome.is_err(), "{what} should have been rejected");
            checked += 1;
        };

        expect_error(exact_keys(&json!({"a": 1}), &["a", "b"], "x"), "missing field");
        expect_error(exact_keys(&json!([1]), &["a"], "x"), "array is not an object");
        expect_error(integer(&json!("1"), 0, 2, "x").map(|_| ()), "string is not a number");
        expect_error(integer(&json!(1.5), 0, 2, "x").map(|_| ()), "non-integer");
        expect_error(integer(&json!(3), 0, 2, "x").map(|_| ()), "out of range");
        expect_error(address(&json!("0x080F38BC"), "x").map(|_| ()), "uppercase hex");
        expect_error(address(&json!("0x0f38bc"), "x").map(|_| ()), "short address");
        expect_error(identifier(&json!("Name"), "x").map(|_| ()), "capitalised identifier");
        expect_error(identifier(&json!("0abc"), "x").map(|_| ()), "leading digit");
        expect_error(consumer_list(&json!([]), "x").map(|_| ()), "empty consumers");
        expect_error(
            consumer_list(&json!(["Func_080f2028", "Func_080f2028"]), "x").map(|_| ()),
            "duplicate consumers",
        );
        expect_error(consumer_list(&json!(["Func_090f2028"]), "x").map(|_| ()), "wrong prefix");
        expect_error(uniform_fill(&[], "x").map(|_| ()), "empty fill");
        expect_error(uniform_fill(&[0x7f, 0x7f], "x").map(|_| ()), "unsupported fill value");
        expect_error(uniform_fill(&[0, 1], "x").map(|_| ()), "non-uniform fill");
        expect_error(
            parse_fill_recipe(&json!({"pattern": "uniform", "value": 0, "bytes": 4}), 5, "x")
                .map(|_| ()),
            "fill byte count mismatch",
        );
        expect_error(
            parse_tokens(&json!([["l", 1]]), 600, "x").map(|_| ()),
            "tokens do not cover the source",
        );
        expect_error(
            parse_tokens(&json!([["c", 2, 1]]), 4, "x").map(|_| ()),
            "copy before any literal",
        );
        expect_error(parse_tokens(&json!([["x", 1]]), 1, "x").map(|_| ()), "unknown opcode");

        assert_eq!(checked, 19, "the validator sweep must run all 19 cases");
    }

    #[test]
    fn json_number_one_point_zero_is_a_safe_integer_like_in_js() {
        // `Number.isSafeInteger(1.0)` is true in JS; `as_i64` on serde_json's
        // f64 would not be reached without the `as_f64` path.
        assert_eq!(safe_integer(&serde_json::from_str::<Value>("1.0").unwrap()), Some(1));
        assert_eq!(safe_integer(&json!(9007199254740992i64)), None);
        assert_eq!(safe_integer(&json!("1")), None);
        assert!(number_is(&serde_json::from_str::<Value>("7294.0").unwrap(), 7294));
    }

    #[test]
    fn ordered_map_set_overwrites_in_place_like_map_set() {
        let mut map: OrderedMap<u32, u8> = OrderedMap::new();
        map.set(2, 20);
        map.set(1, 10);
        map.set(2, 22);
        let keys: Vec<u32> = map.iter().map(|(key, _)| *key).collect();
        assert_eq!(keys, vec![2, 1], "Map#set keeps the original key position");
        assert_eq!(map.get(&2), Some(&22));
    }

    #[test]
    fn table_round_trip_is_exact_for_every_width() {
        let catalog = tracked_catalog();
        let mut checked = 0usize;
        for (component, kind) in catalog.tables() {
            let count = component.count.expect("a table has a count") as usize;
            let values: Vec<i64> = (0..count)
                .map(|index| match kind {
                    TableKind::U8 => (index as i64 * 17) & 0xff,
                    TableKind::S16 => (index as i64 * 999) - 0x4000,
                    TableKind::U16 => (index as i64 * 257) & 0xffff,
                })
                .collect();
            let encoded = encode_table(component, &values).expect("encodes");
            assert_eq!(encoded.len(), component.size());
            assert_eq!(decode_table(kind, &encoded), values, "{}", component.name);
            checked += 1;
        }
        assert_eq!(checked, 12, "all twelve tracked tables must round-trip");
    }

    #[test]
    fn table_bounds_are_enforced_per_width() {
        let catalog = tracked_catalog();
        let (u8_table, _) =
            *catalog.tables().iter().find(|(component, _)| component.name == "haichi_hyou").unwrap();
        assert!(encode_table(u8_table, &[256; 6]).is_err());
        assert!(encode_table(u8_table, &[255; 6]).is_ok());
        assert!(encode_table(u8_table, &[0; 5]).is_err(), "count must match");
        let (s16_table, _) = *catalog
            .tables()
            .iter()
            .find(|(component, _)| component.name == "hantei_dankai")
            .unwrap();
        assert!(encode_table(s16_table, &[-0x8001, 0, 0, 0, 0, 0]).is_err());
        assert!(encode_table(s16_table, &[-0x8000, 0, 0, 0, 0, 0x7fff]).is_ok());
    }

    /// The float trap, pinned: this crate must never emit a non-integral
    /// number, because `serde_json` would then print `1.0` where JS prints `1`.
    #[test]
    fn emitted_json_is_integers_only() {
        fn walk(value: &Value, counted: &mut usize) {
            match value {
                Value::Number(number) => {
                    let raw = number.as_f64().expect("a JSON number is finite");
                    assert!(raw.fract() == 0.0, "non-integral number would diverge from JS: {raw}");
                    *counted += 1;
                }
                Value::Array(items) => items.iter().for_each(|item| walk(item, counted)),
                Value::Object(entries) => entries.values().for_each(|item| walk(item, counted)),
                _ => {}
            }
        }
        let catalog = tracked_catalog();
        let image = synthetic_image(&catalog);
        let base = 0x080f_0000u32;
        let extraction = extract_late_runtime_data(&image, &catalog, base).expect("extracts");
        let component = catalog.single_stream().unwrap().clone();
        let source = extraction.sources.get(&component.name).unwrap();
        let (stream, plan) = stream_documents(source, &component).expect("stream documents");
        let mut counted = 0usize;
        for document in [
            stream,
            plan,
            table_document(&extraction, &catalog).expect("tables"),
            fill_document(&image, &catalog, base).expect("fills"),
            package_index_document(),
        ] {
            walk(&document, &mut counted);
        }
        assert!(counted > 400, "expected to inspect the whole word list, saw {counted}");
    }

    /// The same synthetic image the TypeScript self-test builds.
    fn synthetic_image(catalog: &LateRuntimeCatalog) -> Vec<u8> {
        let base = 0x080f_0000u32;
        let mut sources = zero_sources(catalog).expect("zero sources");
        for component in catalog.components() {
            if let Some(LateRuntimeSource::GeneralLzStream { lookahead, .. }) =
                sources.get_mut(&component.name)
            {
                lookahead.fill(255);
            }
        }
        let typed = build_late_runtime_data(catalog, &sources).expect("builds");
        let mut image = vec![0u8; 0x9000];
        for (start, data) in typed.regions.iter() {
            let offset = (start - base) as usize;
            image[offset..offset + data.len()].copy_from_slice(data);
        }
        for (index, component) in typed.unresolved.iter().enumerate() {
            let value = if index & 1 != 0 { 255u8 } else { 0u8 };
            image[(component.address - base) as usize..(component.end - base) as usize].fill(value);
        }
        image
    }

    #[test]
    fn a_package_child_may_not_be_a_symlink_or_escape() {
        let root = make_temporary_directory(&std::env::temp_dir(), "late-runtime-child-").unwrap();
        let package = root.join("package");
        create_dir_all(&package).unwrap();
        let index = package.join("index.json");
        write_file(&index, b"{}\n").unwrap();
        write_file(&package.join("fills.json"), b"{}\n").unwrap();
        write_file(&root.join("outside.json"), b"{}\n").unwrap();
        std::os::unix::fs::symlink(root.join("outside.json"), package.join("link.json")).unwrap();

        assert!(package_child(&index, "fills.json", "x").is_ok());
        assert!(package_child(&index, "../outside.json", "x").is_err(), "not a leaf");
        assert!(package_child(&index, "link.json", "x").is_err(), "symlinks are not regular files");
        assert!(package_child(&index, "missing.json", "x").is_err());
        std::fs::remove_dir_all(&root).unwrap();
    }

    #[test]
    fn contains_path_is_component_wise() {
        let root = make_temporary_directory(&std::env::temp_dir(), "late-runtime-contains-").unwrap();
        create_dir_all(&root.join("dir")).unwrap();
        create_dir_all(&root.join("dirx")).unwrap();
        write_file(&root.join("dir/a"), b"a").unwrap();
        write_file(&root.join("dirx/b"), b"b").unwrap();
        assert!(contains_path(&root.join("dir"), &root.join("dir/a")));
        assert!(contains_path(&root.join("dir"), &root.join("dir")));
        assert!(!contains_path(&root.join("dir"), &root.join("dirx/b")));
        std::fs::remove_dir_all(&root).unwrap();
    }

    #[test]
    fn a_failed_transaction_restores_the_previous_directory() {
        let root = make_temporary_directory(&std::env::temp_dir(), "late-runtime-txn-").unwrap();
        let destination = root.join("destination");
        create_dir_all(&destination).unwrap();
        write_file(&destination.join("stale"), b"old").unwrap();
        let outcome = replace_directory(&destination, ".test-", &mut |staged| {
            create_dir_all(staged)?;
            write_file(&staged.join("partial"), b"new")?;
            err("synthetic failure")
        });
        assert_eq!(outcome, Err("synthetic failure".to_string()));
        assert!(destination.join("stale").exists());
        assert!(!destination.join("partial").exists());
        assert_eq!(read_dir_names(&root).unwrap(), vec!["destination".to_string()]);
        std::fs::remove_dir_all(&root).unwrap();
    }
}
