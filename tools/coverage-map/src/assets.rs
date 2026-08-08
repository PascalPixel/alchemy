// ROM-data labelling: what the cartridge holds outside audited code.
//
// WHY: the map must never invent a byte. Asset sizes are always the complement
// of the audited code areas, so an unlabelled or mis-labelled package can only
// change the bucket a byte is *described* under, never the byte count. Every
// walk in this file is therefore a labelling walk, and each one is allowed to
// fail quietly (an unreadable nested plan contributes no evidence) without
// putting the conservation checks at risk.

use crate::jsnum::{commas, hex8, is_safe_integer};
use crate::json::Value;
use crate::model::Tile;
use crate::ordered::OrderedMap;
use crate::spans::{intersect, normalize, span_bytes, subtract, Span};
use crate::tree::{read_json, root, SourceTree, ROM_BASE};

const OVERLAY_SERIES: &str = "golden-sun-thumb-overlay-series";

// ---------------------------------------------------------------- primitives

/// Case-insensitive "does any of these literals appear" -- the shape every
/// bucket and tier regex in the TypeScript actually has.
fn any_of(haystack: &str, needles: &[&str]) -> bool {
    let lowered = haystack.to_ascii_lowercase();
    needles.iter().any(|needle| lowered.contains(needle))
}

/// `/\.[a-z0-9]{2,N}$/i`
pub fn has_extension(name: &str, max: usize) -> bool {
    let lowered = name.to_ascii_lowercase();
    let Some(dot) = lowered.rfind('.') else {
        return false;
    };
    let extension = &lowered[dot + 1..];
    let length = extension.chars().count();
    // PORT NOTE: JS `\d` and character classes here are ASCII-only, so a
    // non-ASCII digit must not count. `is_ascii_alphanumeric` matches that.
    (2..=max).contains(&length) && extension.chars().all(|c| c.is_ascii_alphanumeric())
}

fn ends_with_ci(name: &str, suffix: &str) -> bool {
    name.to_ascii_lowercase().ends_with(suffix)
}

/// `hexValue`: a safe-integer number, or a `0x`-prefixed hex string.
///
/// PORT NOTE: JavaScript has one number type, so a manifest value of `1.0`
/// satisfies `Number.isSafeInteger` and is accepted. The check below matches
/// that -- an integral `f64` passes, a fractional one does not.
pub fn hex_value(value: &Value) -> Option<i64> {
    match value {
        Value::Num(number) => {
            if is_safe_integer(*number) {
                Some(*number as i64)
            } else {
                None
            }
        }
        Value::Str(text) => {
            let body = text
                .strip_prefix("0x")
                .or_else(|| text.strip_prefix("0X"))?;
            if body.is_empty() || !body.chars().all(|c| c.is_ascii_hexdigit()) {
                return None;
            }
            i64::from_str_radix(body, 16).ok()
        }
        _ => None,
    }
}

fn members(value: &Value) -> &[(String, Value)] {
    match value {
        Value::Obj(entries) => entries,
        _ => &[],
    }
}

fn is_object(value: &Value) -> bool {
    matches!(value, Value::Obj(_))
}

// ------------------------------------------------------------------ ROM walk

#[derive(Clone, Debug)]
pub struct RomRange {
    pub start: i64,
    pub end: i64,
    pub kind: String,
    pub family: String,
    pub label: String,
    pub sources: Vec<String>,
}

/// Address ranges named by the tracked asset manifest.
pub fn manifest_ranges(tree: &SourceTree, rom_size: i64) -> Result<Vec<RomRange>, String> {
    let manifest = read_json(tree, "assets/manifest.json")?;
    let mut walk = ManifestWalk {
        tree,
        rom_size,
        ranges: Vec::new(),
        visited: Vec::new(),
        source_cache: OrderedMap::new(),
    };
    for section in ["series", "regions", "closure_packages"] {
        let Some(entries) = manifest.get(section).and_then(|value| value.as_array()) else {
            continue;
        };
        for entry in entries {
            if !is_object(entry) {
                continue;
            }
            let family = entry
                .get("kind")
                .and_then(|kind| kind.as_str())
                .unwrap_or("asset")
                .to_string();
            walk.visit(entry, &family, &family);
        }
    }
    Ok(walk.ranges)
}

struct ManifestWalk<'a> {
    tree: &'a SourceTree,
    rom_size: i64,
    ranges: Vec<RomRange>,
    visited: Vec<String>,
    source_cache: OrderedMap<String, Vec<String>>,
}

impl ManifestWalk<'_> {
    /// `sourcesOf`: every source-like string reachable from a node, following
    /// nested `.json` plans but never revisiting one inside its own chain.
    fn sources_of(&mut self, node: &Value) -> Vec<String> {
        let mut found: Vec<String> = Vec::new();
        let mut chain: Vec<String> = Vec::new();
        self.gather(node, &mut chain, &mut found);
        // `[...new Set(found)].sort()`: JS sorts strings by UTF-16 code unit.
        found.sort_by(|left, right| crate::jsnum::utf16_cmp(left, right));
        found.dedup();
        found
    }

    fn gather(&mut self, node: &Value, chain: &mut Vec<String>, found: &mut Vec<String>) {
        match node {
            Value::Arr(items) => {
                for child in items {
                    self.gather(child, chain, found);
                }
            }
            Value::Obj(entries) => {
                let children: Vec<Value> = entries.iter().map(|(_, value)| value.clone()).collect();
                for child in &children {
                    self.gather(child, chain, found);
                }
            }
            Value::Str(text) => {
                let text = text.clone();
                if ends_with_ci(&text, ".json") {
                    if chain.contains(&text) {
                        return;
                    }
                    if let Some(cached) = self.source_cache.get(&text).cloned() {
                        for source in cached {
                            push_unique(found, source);
                        }
                        return;
                    }
                    let Some(body) = self.tree.read(&text) else {
                        return;
                    };
                    let Ok(document) = crate::json::parse(&body) else {
                        // An unreadable nested plan contributes no evidence.
                        return;
                    };
                    chain.push(text.clone());
                    let mut nested: Vec<String> = Vec::new();
                    self.gather(&document, chain, &mut nested);
                    chain.pop();
                    nested.sort_by(|left, right| crate::jsnum::utf16_cmp(left, right));
                    nested.dedup();
                    self.source_cache.insert(text, nested.clone());
                    for source in nested {
                        push_unique(found, source);
                    }
                    return;
                }
                if has_extension(&text, 5) {
                    push_unique(found, text);
                }
            }
            _ => {}
        }
    }

    fn push(
        &mut self,
        start: Option<i64>,
        size: Option<i64>,
        kind: &str,
        family: &str,
        label: String,
        sources: Vec<String>,
    ) {
        let (Some(start), Some(size)) = (start, size) else {
            return;
        };
        if size <= 0 || start < ROM_BASE || start + size > ROM_BASE + self.rom_size {
            return;
        }
        self.ranges.push(RomRange {
            start,
            end: start + size,
            kind: kind.to_string(),
            family: family.to_string(),
            label,
            sources,
        });
    }

    fn visit(&mut self, node: &Value, kind: &str, family: &str) {
        if let Value::Arr(items) = node {
            // Manifest families use several tuple schemas; recognise every
            // ROM-address pair rather than teaching this walk each schema.
            for index in 0..items.len().saturating_sub(1) {
                let start = hex_value(&items[index]);
                let size = hex_value(&items[index + 1]);
                let (Some(start_value), Some(size_value)) = (start, size) else {
                    continue;
                };
                if start_value < ROM_BASE || size_value <= 0 {
                    continue;
                }
                let identity = if index > 0
                    && items[index - 1].as_str().is_some()
                    && hex_value(&items[index - 1]).is_none()
                {
                    items[index - 1].as_str().unwrap_or("").to_string()
                } else {
                    kind.to_string()
                };
                let sources = self.sources_of(node);
                self.push(
                    start,
                    size,
                    kind,
                    family,
                    format!("{identity} · {kind}"),
                    sources,
                );
            }
            for child in items.clone() {
                self.visit(&child, kind, family);
            }
            return;
        }
        if !is_object(node) {
            return;
        }
        let local = node
            .get("kind")
            .and_then(|value| value.as_str())
            .unwrap_or(kind)
            .to_string();
        let identity = ["id", "name", "source", "index"]
            .iter()
            .find_map(|key| node.get(key))
            .map(js_string)
            .unwrap_or_else(|| local.clone());
        let sources = self.sources_of(node);
        self.push(
            node.get("address").and_then(hex_value),
            node.get("size").and_then(hex_value),
            &local,
            family,
            format!("{identity} · {local}"),
            sources,
        );
        for (key, value) in members(node).to_vec() {
            // Components describe decoded payloads, not ROM ranges.
            if key != "components" {
                self.visit(&value, &local, family);
            }
        }
        for key in ["index", "source", "plan"] {
            let Some(reference) = node.get(key).and_then(|value| value.as_str()) else {
                continue;
            };
            if !reference.ends_with(".json") {
                continue;
            }
            let reference = reference.to_string();
            if self.visited.contains(&reference) {
                continue;
            }
            self.visited.push(reference.clone());
            let Some(body) = self.tree.read(&reference) else {
                continue;
            };
            let Ok(document) = crate::json::parse(&body) else {
                continue;
            };
            self.visit(&document, &local, family);
        }
    }
}

fn push_unique(found: &mut Vec<String>, value: String) {
    if !found.contains(&value) {
        found.push(value);
    }
}

/// `String(value)` for the identity fields, which may be a number or a string.
fn js_string(value: &Value) -> String {
    match value {
        Value::Str(text) => text.clone(),
        Value::Num(number) => crate::jsnum::js_number_string(*number),
        Value::Bool(true) => "true".to_string(),
        Value::Bool(false) => "false".to_string(),
        Value::Null => "null".to_string(),
        Value::Arr(_) | Value::Obj(_) => "[object Object]".to_string(),
    }
}

// ------------------------------------------------------------------- streams

#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct Stream {
    pub start: i64,
    pub rom_bytes: i64,
    pub decoded_bytes: i64,
}

/// Compressed ROM footprint and decoded size of every Thumb code overlay.
pub fn overlay_streams(tree: &SourceTree) -> Result<OrderedMap<String, Stream>, String> {
    let manifest = read_json(tree, "assets/manifest.json")?;
    let mut streams: OrderedMap<String, Stream> = OrderedMap::new();
    let series = manifest
        .get("series")
        .and_then(|value| value.as_array())
        .unwrap_or(&[])
        .to_vec();
    for entry in &series {
        if entry.get("kind").and_then(|value| value.as_str()) != Some(OVERLAY_SERIES) {
            continue;
        }
        for resource in entry
            .get("resources")
            .and_then(|value| value.as_array())
            .unwrap_or(&[])
        {
            let Some(row) = resource.as_array() else {
                continue;
            };
            let id = row.first().map(js_string).unwrap_or_default();
            let start = row.get(1).and_then(hex_value);
            let rom_bytes = row.get(2).and_then(hex_value);
            let decoded = row.get(3).and_then(hex_value).unwrap_or(0);
            let (Some(start), Some(rom_bytes)) = (start, rom_bytes) else {
                continue;
            };
            streams.insert(
                format!("resource_{id}"),
                Stream {
                    start,
                    rom_bytes,
                    decoded_bytes: decoded,
                },
            );
        }
    }
    // The final-battle module is selected through a dedicated layout rather
    // than the ordinary series table. It is still compressed code and must not
    // be painted as asset data merely because its schema differs.
    let mut tail = series;
    tail.extend(
        manifest
            .get("closure_packages")
            .and_then(|value| value.as_array())
            .unwrap_or(&[])
            .to_vec(),
    );
    for entry in &tail {
        if entry.get("kind").and_then(|value| value.as_str())
            != Some("golden-sun-final-battle-overlay-series")
        {
            continue;
        }
        let Some(source) = entry.get("source").and_then(|value| value.as_str()) else {
            continue;
        };
        let Some(body) = tree.read(source) else {
            continue;
        };
        let layout = crate::json::parse(&body)
            .map_err(|error| format!("{source} is not valid JSON: {error}"))?;
        let start = layout.get("address").and_then(hex_value);
        let rom_bytes = layout.get("stream_size").and_then(hex_value);
        let decoded = layout.get("decoded_size").and_then(hex_value).unwrap_or(0);
        let resource = layout.get("resource_id").and_then(hex_value);
        let (Some(start), Some(rom_bytes), Some(resource)) = (start, rom_bytes, resource) else {
            continue;
        };
        streams.insert(
            format!("resource_{:x}", resource),
            Stream {
                start,
                rom_bytes,
                decoded_bytes: decoded,
            },
        );
    }
    Ok(streams)
}

// ------------------------------------------------------------------- buckets

pub struct Bucket {
    pub id: &'static str,
    pub label: &'static str,
}

const BUCKET_MATCHES: [(&str, &str, &[&str]); 6] = [
    (
        "sprites",
        "Sprites & battle art",
        &["sprite", "character-catalog", "mtf4", "chr", "enemy"],
    ),
    (
        "backgrounds",
        "Backgrounds & stills",
        &[
            "still",
            "background",
            "title",
            "tile",
            "palette",
            "canvas",
            "4bpp",
            "8bpp",
        ],
    ),
    (
        "maps",
        "Maps & world",
        &["map", "grid", "charblock", "chiiki", "tokushu", "world"],
    ),
    (
        "audio",
        "Music & sound",
        &["sound", "audio", "wave", "pcm", "music", "midi", "sequence"],
    ),
    (
        "text",
        "Text & fonts",
        &[
            "message",
            "font",
            "localization",
            "kana",
            "glyph",
            "namae",
            "staff",
        ],
    ),
    (
        "tables",
        "Tables & databases",
        &[
            "data",
            "table",
            "database",
            "directory",
            "runtime",
            "encounter",
            "header",
            "resource",
        ],
    ),
];

/// First matching bucket wins -- JS alternation is ordered, leftmost-first.
pub fn asset_bucket(kind: &str) -> Bucket {
    for (id, label, needles) in BUCKET_MATCHES {
        if any_of(kind, needles) {
            return Bucket { id, label };
        }
    }
    Bucket {
        id: "other",
        label: "Other data",
    }
}

fn tier_rank(tier: &str) -> usize {
    crate::model::ASSET_TIERS
        .iter()
        .position(|name| *name == tier)
        .unwrap_or(0)
}

/// Representation-form tier for one package.
pub fn asset_tier_of(sources: &[String], kind: &str) -> &'static str {
    let lowered = kind.to_ascii_lowercase();
    if any_of(
        &lowered,
        &["music", "sequence", "midi", "wave", "pcm", "audio"],
    ) || lowered.contains("soundfont")
        || lowered.contains("sound-font")
    {
        return "asset_objects";
    }
    if any_of(&lowered, &["sprite", "character-catalog", "chr"])
        && sources
            .iter()
            .any(|name| any_of(name, &["koma_", "frame_"]))
    {
        return "asset_objects";
    }
    let mut tier = "asset_bytes";
    for name in sources {
        let lowered = name.to_ascii_lowercase();
        let seen = if ends_with_ci(&lowered, ".mid")
            || ends_with_ci(&lowered, ".wav")
            || ends_with_ci(&lowered, ".sf2")
            || ends_with_ci(&lowered, ".pcm")
            // The TypeScript keeps this as a separate `else if` arm; the two
            // arms produce the same tier and neither test has a side effect, so
            // merging them is behaviour-preserving.
            || object_png(&lowered)
        {
            Some("asset_objects")
        } else if bw_png(&lowered) {
            Some("asset_bw")
        } else if ends_with_ci(&lowered, ".png") {
            Some("asset_color")
        } else if ends_with_ci(&lowered, ".bin") || ends_with_ci(&lowered, ".tilemap") {
            Some("asset_bytes")
        } else {
            None
        };
        if let Some(seen) = seen {
            if tier_rank(seen) > tier_rank(tier) {
                tier = seen;
            }
        }
    }
    tier
}

/// `/koma_\d+\.png$|frame_\d+\.png$|object[^/]*\.png$/i`
fn object_png(lowered: &str) -> bool {
    let Some(stem) = lowered.strip_suffix(".png") else {
        return false;
    };
    for prefix in ["koma_", "frame_"] {
        if let Some(at) = stem.rfind(prefix) {
            let digits = &stem[at + prefix.len()..];
            // `\d` is ASCII-only in JavaScript.
            if !digits.is_empty() && digits.chars().all(|c| c.is_ascii_digit()) {
                return true;
            }
        }
    }
    // `object[^/]*` -- "object" followed by anything that is not a separator.
    match stem.rfind("object") {
        Some(at) => !stem[at..].contains('/'),
        None => false,
    }
}

/// `/(1bpp|mask|value_low|value_high|grid_sentinels)[^/]*\.png$/i`
fn bw_png(lowered: &str) -> bool {
    let Some(stem) = lowered.strip_suffix(".png") else {
        return false;
    };
    for needle in ["1bpp", "mask", "value_low", "value_high", "grid_sentinels"] {
        if let Some(at) = stem.rfind(needle) {
            if !stem[at..].contains('/') {
                return true;
            }
        }
    }
    false
}

// --------------------------------------------------------------------- tiles

/// Natural ROM-data leaves, children first so a parent package can claim only
/// its still-unclaimed footprint and nested manifests cannot double-count.
pub fn manifest_asset_tiles(
    tree: &SourceTree,
    rom_size: i64,
    data_spans: &[Span],
) -> Result<Vec<Tile>, String> {
    let mut by_extent: OrderedMap<String, RomRange> = OrderedMap::new();
    for range in manifest_ranges(tree, rom_size)? {
        let key = format!("{}:{}", range.start, range.end);
        match by_extent.get(&key).cloned() {
            None => {
                by_extent.insert(key, range);
            }
            Some(mut previous) => {
                for source in &range.sources {
                    push_unique(&mut previous.sources, source.clone());
                }
                previous
                    .sources
                    .sort_by(|l, r| crate::jsnum::utf16_cmp(l, r));
                previous.sources.dedup();
                if previous.kind == "asset" && range.kind != "asset" {
                    previous.kind = range.kind.clone();
                    previous.family = range.family.clone();
                    previous.label = range.label.clone();
                }
                by_extent.insert(key, previous);
            }
        }
    }

    let mut ranges: Vec<RomRange> = by_extent.iter().map(|(_, range)| range.clone()).collect();
    // PORT NOTE: `Array#sort` is stable in JavaScript, and `sort_by` is stable
    // in Rust, so the tie-break chain below is enough to reproduce the order.
    // The final `localeCompare` is ICU collation; a UTF-16 code-unit compare is
    // used instead -- see the crate report for why this is only an
    // approximation and how the parity run confirms it.
    ranges.sort_by(|left, right| {
        (left.end - left.start)
            .cmp(&(right.end - right.start))
            .then(left.start.cmp(&right.start))
            .then(left.end.cmp(&right.end))
            .then(crate::jsnum::utf16_cmp(&left.label, &right.label))
    });

    let mut tiles: Vec<Tile> = Vec::new();
    let mut claimed: Vec<Span> = Vec::new();
    for range in &ranges {
        let extent = [Span::new(range.start, range.end)];
        let fresh = subtract(&intersect(&extent, data_spans), &claimed);
        let bytes = span_bytes(&fresh);
        if bytes <= 0 {
            continue;
        }
        let mut merged = claimed.clone();
        merged.extend(fresh);
        claimed = normalize(&merged);
        let tier = asset_tier_of(&range.sources, &range.kind);
        let mut tile = Tile {
            label: format!(
                "{} · 0x{} · {} bytes",
                range.label,
                hex8(range.start),
                commas(bytes as f64)
            ),
            bytes,
            group: Some(range.family.clone()),
            subgroup: if range.family == range.kind {
                None
            } else {
                Some(range.kind.clone())
            },
            address: Some(range.start),
            ..Tile::default()
        };
        tile.set_category("asset_data", bytes);
        tile.set_category(tier, bytes);
        tiles.push(tile);
    }

    // The remainder is stated, never absorbed into a neighbouring package.
    let unclassified = span_bytes(data_spans) - span_bytes(&claimed);
    if unclassified > 0 {
        let mut tile = Tile {
            label: "Unclassified ROM-image data".to_string(),
            bytes: unclassified,
            group: Some("unclassified-rom-data".to_string()),
            ..Tile::default()
        };
        tile.set_category("asset_data", unclassified);
        tile.set_category("asset_unclassified", unclassified);
        tiles.push(tile);
    }
    Ok(tiles)
}

// --------------------------------------------------------------- retained asm

const PERMANENT_KINDS: [&str; 5] = [
    "linker_veneer",
    "alignment_padding",
    "relocated_arm_runtime_module",
    "armv4t_helper_bank",
    "iwram_runtime_veneer",
];

// -------------------------------------------------------- maturity ladder

fn maturity_gather_sources(
    tree: &SourceTree,
    node: &Value,
    out: &mut Vec<String>,
    visited: &mut Vec<String>,
) {
    match node {
        Value::Arr(items) => {
            for child in items {
                maturity_gather_sources(tree, child, out, visited);
            }
        }
        Value::Obj(members) => {
            for (_, value) in members {
                maturity_gather_sources(tree, value, out, visited);
            }
        }
        Value::Str(text) => {
            if ends_with_ci(text, ".json") {
                if visited.iter().any(|seen| seen == text) {
                    return;
                }
                visited.push(text.clone());
                if let Some(body) = tree.read(text) {
                    if let Ok(document) = crate::json::parse(&body) {
                        maturity_gather_sources(tree, &document, out, visited);
                    }
                }
                return;
            }
            // PORT NOTE: this call site spells the extension bound `{2,4}`
            // while `assetFamiliesBySource` spells it `{2,5}`. The drift is
            // upstream and observable: a five-character extension such as
            // `.4bpp` is seen by `assetFamiliesBySource` and not by this walk.
            // (`.tilemap` is seven characters and is invisible to both.) Both
            // spellings are preserved verbatim rather than unified.
            if has_extension(text, 4) {
                out.push(text.clone());
            }
        }
        _ => {}
    }
}

fn maturity_record(
    buckets: &mut OrderedMap<String, Tile>,
    kind: &str,
    size: i64,
    sources: &[String],
    raw_kind: &str,
) {
    if size <= 0 {
        return;
    }
    let bucket = asset_bucket(kind);
    // Music, samples and soundfonts live as per-object corpora even where the
    // package index names them by prefix, so the audio bucket is object-tier
    // by construction.
    let tier = if bucket.id == "audio" {
        "asset_objects"
    } else {
        asset_tier_of(sources, if raw_kind.is_empty() { kind } else { raw_kind })
    };
    let mut tile = buckets
        .get(&bucket.id.to_string())
        .cloned()
        .unwrap_or(Tile {
            label: bucket.label.to_string(),
            bytes: 0,
            ..Tile::default()
        });
    tile.bytes += size;
    tile.add_category(tier, size);
    buckets.insert(bucket.id.to_string(), tile);
}

fn maturity_visit(
    tree: &SourceTree,
    node: &Value,
    kind: &str,
    buckets: &mut OrderedMap<String, Tile>,
    visited: &mut Vec<String>,
) {
    if let Value::Arr(items) = node {
        for child in items {
            maturity_visit(tree, child, kind, buckets, visited);
        }
        return;
    }
    let Value::Obj(members) = node else { return };
    let local = node
        .get("kind")
        .and_then(|value| value.as_str())
        .unwrap_or(kind)
        .to_string();
    let size = node.get("size").and_then(hex_value);
    if let Some(size) = size {
        if size > 0 {
            let mut sources: Vec<String> = Vec::new();
            maturity_gather_sources(tree, node, &mut sources, visited);
            for key in ["index", "source", "plan"] {
                let Some(Value::Str(reference)) = node.get(key) else {
                    continue;
                };
                if !reference.ends_with(".json") {
                    continue;
                }
                if visited.iter().any(|seen| seen == reference) {
                    continue;
                }
                visited.push(reference.clone());
                if let Some(body) = tree.read(reference) {
                    if let Ok(document) = crate::json::parse(&body) {
                        maturity_gather_sources(tree, &document, &mut sources, visited);
                    }
                }
            }
            maturity_record(buckets, &local, size, &sources, &local);
        }
    }
    for (key, value) in members {
        if key != "components" {
            maturity_visit(tree, value, &local, buckets, visited);
        }
    }
}

/// `assetMaturityTiles(tree)`: the same buckets as the ROM card, with
/// categories keyed by representation tier rather than by coverage category.
pub fn asset_maturity_tiles(tree: &SourceTree) -> Result<Vec<Tile>, String> {
    let manifest = read_json(tree, "assets/manifest.json")?;
    let mut buckets: OrderedMap<String, Tile> = OrderedMap::new();
    let mut visited: Vec<String> = Vec::new();
    maturity_visit(tree, &manifest, "asset", &mut buckets, &mut visited);
    Ok(buckets
        .values()
        .filter(|tile| tile.bytes > 0)
        .cloned()
        .collect())
}

/// Main-image regions that will never become C by design.
///
/// PORT NOTE (reproduced bug): the TypeScript builds this path as
/// `join(dirname(dirname(dirname(import.meta.url))), "..", "out", "full",
/// "asm", "manifest.json")`. Three `dirname` calls already reach the repository
/// root, so the extra `".."` resolves to the repository's *parent*. The
/// full-build manifest is therefore never found and the function always falls
/// back to the explicit `non_c_ranges` evidence alone. The same off-by-one
/// parent hop is reproduced here rather than fixed; see the crate report.
pub fn retained_main_spans() -> Vec<Span> {
    let repo = root();
    let manifest_path = repo
        .join("..")
        .join("out")
        .join("full")
        .join("asm")
        .join("manifest.json");

    let mut explicit_non_c: Vec<Span> = Vec::new();
    let non_c_path = repo.join("semantic").join("main-regions.json");
    if let Ok(text) = std::fs::read_to_string(&non_c_path) {
        if let Ok(document) = crate::json::parse(&text) {
            for region in document
                .get("non_c_ranges")
                .and_then(|v| v.as_array())
                .unwrap_or(&[])
            {
                // PORT NOTE: `Number.parseInt(text, 16)` accepts a leading
                // `0x`, and every address in `semantic/main-regions.json` is
                // written that way. A strict whole-string hex parse rejected
                // all sixteen, dropping 980 bytes of retained evidence into
                // the `assembly` bucket.
                let start = match region.get("address") {
                    Some(Value::Str(text)) => Some(crate::jsnum::parse_hex(text) as f64),
                    Some(Value::Num(number)) => Some(*number),
                    _ => None,
                };
                let size = region.get("size").and_then(|v| v.as_f64());
                let (Some(start), Some(size)) = (start, size) else {
                    continue;
                };
                if !is_safe_integer(start) {
                    continue;
                }
                let start = start as i64;
                let kind = region.get("kind").and_then(|v| v.as_str()).unwrap_or("");
                let evidence = region
                    .get("evidence")
                    .and_then(|v| v.as_str())
                    .unwrap_or("");
                if !is_safe_integer(size)
                    || size <= 0.0
                    || !["literal_pool", "alignment_padding", "lookup_table"].contains(&kind)
                    || evidence.trim_matches(crate::js::is_js_space).is_empty()
                {
                    continue;
                }
                explicit_non_c.push(Span::new(start, start + size as i64));
            }
        }
        // An invalid registry never becomes orange evidence.
    }

    let Ok(text) = std::fs::read_to_string(&manifest_path) else {
        return normalize(&explicit_non_c);
    };
    let Ok(manifest) = crate::json::parse(&text) else {
        return normalize(&explicit_non_c);
    };
    let mut spans: Vec<Span> = Vec::new();
    for region in manifest
        .get("regions")
        .and_then(|v| v.as_array())
        .unwrap_or(&[])
    {
        let (Some(address), Some(size)) = (
            region.get("address").and_then(|v| v.as_f64()),
            region.get("size").and_then(|v| v.as_f64()),
        ) else {
            continue;
        };
        if size <= 0.0 {
            continue;
        }
        let retention = region
            .get("retention")
            .and_then(|v| v.as_str())
            .unwrap_or("");
        let kind = region.get("kind").and_then(|v| v.as_str()).unwrap_or("");
        let evidence_text = region
            .get("evidence")
            .and_then(|v| v.as_str())
            .unwrap_or("");
        let evidence_string = region
            .get("evidence")
            .map(js_string)
            .unwrap_or_else(|| "".into());
        let permanent = retention == "keep_asm"
            || (retention == "keep_structured_asm"
                && region.get("confidence").and_then(|v| v.as_str()) == Some("proven")
                && !evidence_text.trim_matches(crate::js::is_js_space).is_empty())
            // The merge-with-owner family is literal pools, alignment and data
            // the owner registration deliberately excludes. Permanent.
            || retention == "merge_with_owner"
            || retention == "merge_with_function_owner"
            || retention == "merge_with_continuations"
            || kind.starts_with("deliberate_")
            || retention == "adjacent_section_alignment"
            || PERMANENT_KINDS.contains(&kind)
            || evidence_string.contains("approved_compiler_cannot_express");
        if permanent {
            spans.push(Span::new(address as i64, address as i64 + size as i64));
        }
    }
    spans.extend(explicit_non_c);
    normalize(&spans)
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::json::parse;

    #[test]
    fn hex_value_accepts_integral_floats_and_hex_strings() {
        assert_eq!(hex_value(&parse("1.0").expect("literal")), Some(1));
        assert_eq!(hex_value(&parse("1.5").expect("literal")), None);
        assert_eq!(
            hex_value(&parse("\"0x08000000\"").expect("literal")),
            Some(0x0800_0000)
        );
        assert_eq!(hex_value(&parse("\"0X10\"").expect("literal")), Some(16));
        assert_eq!(
            hex_value(&parse("\"10\"").expect("literal")),
            None,
            "no 0x prefix, no value"
        );
        assert_eq!(hex_value(&parse("\"0x\"").expect("literal")), None);
        assert_eq!(hex_value(&parse("null").expect("literal")), None);
    }

    #[test]
    fn asset_bucket_is_ordered_leftmost_alternation() {
        // "sound" is in audio and "data" is in tables; "sound_data" must take
        // the earlier bucket because JS alternation is ordered.
        assert_eq!(asset_bucket("sound_data").id, "audio");
        assert_eq!(
            asset_bucket("TILE-map").id,
            "backgrounds",
            "case-insensitive, and tile wins"
        );
        assert_eq!(asset_bucket("mystery").id, "other");
    }

    #[test]
    fn asset_tier_takes_the_highest_rank_seen() {
        let sources: Vec<String> = [
            "a/koma_12.png".into(),
            "b/plain.png".into(),
            "c/data.bin".into(),
        ]
        .to_vec();
        assert_eq!(asset_tier_of(&sources, "tiles"), "asset_objects");
        assert_eq!(
            asset_tier_of(&["b/plain.png".to_string()], "tiles"),
            "asset_color"
        );
        assert_eq!(
            asset_tier_of(&["b/mask_a.png".to_string()], "tiles"),
            "asset_bw"
        );
        assert_eq!(
            asset_tier_of(&["b/data.bin".to_string()], "tiles"),
            "asset_bytes"
        );
        assert_eq!(
            asset_tier_of(&[], "midi-sequences"),
            "asset_objects",
            "kind alone decides"
        );
        assert_eq!(asset_tier_of(&[], "sound-font"), "asset_objects");
        // A sprite kind needs per-frame sources before it counts as objects.
        assert_eq!(
            asset_tier_of(&["x/sheet.png".to_string()], "sprite"),
            "asset_color"
        );
        assert_eq!(
            asset_tier_of(&["x/koma_1.png".to_string()], "sprite"),
            "asset_objects"
        );
    }

    #[test]
    fn extension_test_is_ascii_only_and_length_bounded() {
        assert!(has_extension("a.png", 5));
        assert!(has_extension("a.tilemap".to_string().as_str(), 7));
        assert!(
            !has_extension("a.tilemap", 5),
            "8 characters exceeds the bound"
        );
        assert!(!has_extension("a.x", 5), "one character is below the bound");
        assert!(
            !has_extension("a.pn\u{0660}", 5),
            "a non-ASCII digit is not \\d"
        );
    }

    #[test]
    fn object_and_bw_png_matchers_follow_the_js_classes() {
        assert!(object_png("a/koma_7.png"));
        assert!(!object_png("a/koma_.png"), "\\d+ needs at least one digit");
        assert!(object_png("a/object_two.png"));
        assert!(
            !object_png("object/two.png"),
            "[^/]* cannot cross a separator"
        );
        assert!(bw_png("a/mask_left.png"));
        assert!(!bw_png("mask/left.png"));
    }
}
