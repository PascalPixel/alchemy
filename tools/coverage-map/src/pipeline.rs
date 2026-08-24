use crate::model::{area, bytes, intersect, normalize, subtract, Area, Span, Tile, CATEGORIES};
use crate::tree::{read_json, SourceTree, ROM_BASE};
use compiler_core::source_paths::{SourceOwner, SourcePaths, SOURCE_PATHS_MANIFEST};
use serde_json::{Map, Value};
use std::path::Path;

pub struct BuildOptions<'a> {
    pub target: String,
    pub exact: &'a SourceTree,
    pub recon: Option<&'a SourceTree>,
    pub validate_tracked_progress: bool,
    pub prefer_verified_assets: bool,
}

pub struct CoverageMap {
    pub document: Value,
    pub rom_areas: Vec<Area>,
    pub executable_areas: Vec<Area>,
}

pub fn rom_size(target: &str) -> Result<i64, String> {
    match target {
        "gs1-en" => Ok(0x800000),
        "gs2-en" => Ok(0x1000000),
        other => Err(format!(
            "unsupported decomp target {other:?}; expected gs1-en or gs2-en"
        )),
    }
}

fn get<'a>(value: &'a Value, key: &str) -> Option<&'a Value> {
    value.as_object()?.get(key)
}
fn text(value: &Value, key: &str) -> String {
    get(value, key).and_then(Value::as_str).unwrap_or("").into()
}
fn integer(value: &Value, key: &str) -> Option<i64> {
    get(value, key).and_then(Value::as_i64).or_else(|| {
        get(value, key)
            .and_then(Value::as_f64)
            .filter(|n| n.fract() == 0.0)
            .map(|n| n as i64)
    })
}
fn array<'a>(value: &'a Value, key: &str) -> &'a [Value] {
    get(value, key)
        .and_then(Value::as_array)
        .map(Vec::as_slice)
        .unwrap_or(&[])
}
fn obj(fields: Vec<(&str, Value)>) -> Value {
    Value::Object(fields.into_iter().map(|(k, v)| (k.into(), v)).collect())
}
fn num(n: i64) -> Value {
    Value::Number(n.into())
}

#[derive(Clone, Debug)]
struct Region {
    span: Span,
    kind: String,
}

fn regions(value: &Value) -> Vec<Region> {
    array(value, "intervals")
        .iter()
        .filter_map(|item| {
            Some(Region {
                span: Span::new(integer(item, "start")?, integer(item, "end")?),
                kind: text(item, "kind"),
            })
        })
        .filter(|r| r.span.end > r.span.start)
        .collect()
}

pub fn intervals_of(value: &Value) -> Vec<Span> {
    normalize(
        &regions(value)
            .into_iter()
            .map(|r| r.span)
            .collect::<Vec<_>>(),
    )
}

fn canonical(source: &str) -> bool {
    !source.contains(".incbin")
        && !source.contains("M2C_ERROR")
        && !source.contains("__asm__")
        && !source.contains("asm volatile")
        && !source
            .lines()
            .any(|line| line.contains("register") && line.contains("asm") && line.contains('('))
}

fn hex(value: &str) -> Option<i64> {
    i64::from_str_radix(value.trim().trim_start_matches("0x"), 16).ok()
}
fn space(line: &str) -> Option<i64> {
    let value = line.trim().strip_prefix(".space")?.trim();
    value
        .strip_prefix("0x")
        .map_or_else(|| value.parse().ok(), |v| i64::from_str_radix(v, 16).ok())
}
fn c_label(line: &str) -> Option<i64> {
    let value = line.trim().strip_prefix("AlchemyC_")?.trim_end_matches(':');
    (value.len() == 8 && value.chars().all(|c| c.is_ascii_hexdigit()))
        .then(|| hex(value))
        .flatten()
}
fn local_label(line: &str) -> bool {
    line.trim().starts_with(".L_") && line.trim_end().ends_with(':')
}
fn overlay_name(name: &str) -> Option<String> {
    name.strip_prefix("resource_")?
        .strip_suffix("_overlay.s")
        .map(|s| format!("resource_{s}"))
}
fn overlay_short(id: &str) -> &str {
    id.strip_prefix("resource_").unwrap_or(id)
}

#[derive(Clone, Debug)]
pub struct Owner {
    pub label: String,
    pub entry: i64,
    pub spans: Vec<Span>,
}

fn overlay_owners(tree: &SourceTree, name: &str) -> Vec<Owner> {
    let Some(source) = tree.read(&format!("games/gs1/assets/code/{name}")) else {
        return Vec::new();
    };
    let Some(_id) = overlay_name(name) else {
        return Vec::new();
    };
    let mut out = Vec::new();
    let mut owner: Option<Owner> = None;
    let mut cursor = 0;
    for line in source.split('\n') {
        if let Some(entry) = c_label(line) {
            if let Some(open) = owner.take() {
                if !open.spans.is_empty() {
                    out.push(open);
                }
            }
            cursor = entry;
            owner = Some(Owner {
                label: String::new(),
                entry,
                spans: Vec::new(),
            });
            continue;
        }
        if owner.is_some() && (line.trim().is_empty() || local_label(line)) {
            continue;
        }
        if let Some(size) = space(line) {
            if let Some(open) = owner.as_mut() {
                open.spans.push(Span::new(cursor, cursor + size));
                cursor += size;
                continue;
            }
        }
        if !line.trim().is_empty() {
            if let Some(open) = owner.take() {
                if !open.spans.is_empty() {
                    out.push(open);
                }
            }
        }
    }
    if let Some(open) = owner {
        if !open.spans.is_empty() {
            out.push(open);
        }
    }
    out
}

fn overlay_ids(tree: &SourceTree) -> Vec<(String, String)> {
    let mut names: Vec<_> = tree
        .list("games/gs1/assets/code")
        .into_iter()
        .filter_map(|name| overlay_name(&name).map(|id| (id, name)))
        .collect();
    names.sort_by(|a, b| a.0.cmp(&b.0));
    names
}

fn exact_main(tree: &SourceTree, executable: &[Span]) -> Vec<Span> {
    let value = tree
        .read("out/gs1-en/full/claimed/manifest.json")
        .and_then(|s| serde_json::from_str::<Value>(&s).ok());
    let mut spans = Vec::new();
    if let Some(manifest) = value {
        for region in array(&manifest, "regions") {
            let Some(start) = integer(region, "address") else {
                continue;
            };
            let Some(size) = integer(region, "size") else {
                continue;
            };
            let source = text(region, "source");
            if canonical(&tree.read(&source).unwrap_or_default()) {
                spans.extend(intersect(&[Span::new(start, start + size)], executable));
            }
        }
    }
    normalize(&spans)
}

fn candidate_main(tree: &SourceTree, executable: &[Span]) -> (Vec<Span>, usize) {
    let directory = "games/gs1/recon/en/main";
    let mut spans = Vec::new();
    let mut sources = 0;
    for name in tree.list(directory) {
        let Some(stem) = name.strip_suffix(".json") else {
            continue;
        };
        if !tree
            .read(&format!("{directory}/{stem}.c"))
            .is_some_and(|s| canonical(&s))
        {
            continue;
        }
        let Some(record) = tree
            .read(&format!("{directory}/{name}"))
            .and_then(|s| serde_json::from_str::<Value>(&s).ok())
        else {
            continue;
        };
        let Some(start) = hex(stem) else {
            continue;
        };
        let Some(size) = integer(&record, "span_bytes") else {
            continue;
        };
        sources += 1;
        spans.push(Span::new(start, start + size));
    }
    (intersect(&normalize(&spans), executable), sources)
}

fn candidate_overlay(
    tree: &SourceTree,
    executable: &std::collections::BTreeMap<String, Vec<Span>>,
) -> (std::collections::BTreeMap<String, Vec<Span>>, usize) {
    let directory = "games/gs1/recon/en/overlays";
    let reviewed = tree
        .read("games/gs1/semantic/regions.json")
        .and_then(|source| serde_json::from_str::<Value>(&source).ok())
        .unwrap_or(Value::Null);
    let mut extents = std::collections::BTreeMap::new();
    for region in array(&reviewed, "manual_regions") {
        let id = text(region, "overlay");
        let entry = get(region, "entry")
            .and_then(Value::as_str)
            .and_then(hex)
            .or_else(|| integer(region, "entry"));
        if let (Some(entry), Some(size)) = (entry, integer(region, "span_bytes")) {
            extents.insert((id, entry), size);
        }
    }
    let mut spans = std::collections::BTreeMap::new();
    let mut sources = 0;
    for name in tree.list(directory) {
        let Some(stem) = name.strip_suffix(".c") else {
            continue;
        };
        let Some((id, address)) = stem.rsplit_once("_c_") else {
            continue;
        };
        let Some(entry) = hex(address) else {
            continue;
        };
        if !tree
            .read(&format!("{directory}/{name}"))
            .is_some_and(|source| canonical(&source))
        {
            continue;
        }
        let record_size = tree
            .read(&format!("{directory}/{stem}.json"))
            .and_then(|source| serde_json::from_str::<Value>(&source).ok())
            .and_then(|record| integer(&record, "span_bytes"));
        let Some(size) = record_size.or_else(|| extents.get(&(id.into(), entry)).copied()) else {
            continue;
        };
        sources += 1;
        spans
            .entry(id.to_string())
            .or_insert_with(Vec::new)
            .push(Span::new(entry, entry + size));
    }
    for (id, found) in &mut spans {
        *found = intersect(
            &normalize(found),
            executable.get(id).map(Vec::as_slice).unwrap_or(&[]),
        );
    }
    (spans, sources)
}

fn exact_overlay(
    tree: &SourceTree,
    pairs: &[(String, String)],
    executable: &std::collections::BTreeMap<String, Vec<Span>>,
) -> Result<
    (
        std::collections::BTreeMap<String, Vec<Owner>>,
        std::collections::BTreeMap<String, Vec<Span>>,
    ),
    String,
> {
    let manifest = tree
        .read(SOURCE_PATHS_MANIFEST)
        .ok_or_else(|| format!("missing canonical owner register {SOURCE_PATHS_MANIFEST}"))?;
    let source_paths = SourcePaths::parse(Path::new(""), &manifest)?;
    let mut owners = std::collections::BTreeMap::new();
    let mut spans = std::collections::BTreeMap::new();
    for (id, name) in pairs {
        let list = overlay_owners(tree, name)
            .into_iter()
            .map(|mut owner| -> Result<Owner, String> {
                let source_owner = SourceOwner::parse(&format!("{id}:{:08x}", owner.entry))?;
                owner.label = source_paths
                    .registered_name(source_owner)
                    .ok_or_else(|| {
                        format!(
                            "exact owner {} has no name in {SOURCE_PATHS_MANIFEST}",
                            source_owner.id()
                        )
                    })?
                    .to_string();
                let path = source_paths
                    .repository_relative_path(source_owner)
                    .to_string_lossy()
                    .replace('\\', "/");
                if !tree.read(&path).is_some_and(|source| canonical(&source)) {
                    owner.spans.clear();
                }
                owner.spans = intersect(
                    &owner.spans,
                    executable.get(id).map(Vec::as_slice).unwrap_or(&[]),
                );
                Ok(owner)
            })
            .collect::<Result<Vec<_>, _>>()?
            .into_iter()
            .filter(|owner| !owner.spans.is_empty())
            .collect::<Vec<_>>();
        let flat: Vec<_> = list.iter().flat_map(|o| o.spans.iter().copied()).collect();
        owners.insert(id.clone(), list);
        spans.insert(id.clone(), normalize(&flat));
    }
    Ok((owners, spans))
}

fn permanent_main(tree: &SourceTree) -> Vec<Span> {
    let mut spans = Vec::new();
    if let Some(value) = tree
        .read("out/gs1-en/full/asm/manifest.json")
        .and_then(|s| serde_json::from_str::<Value>(&s).ok())
    {
        for region in array(&value, "regions") {
            let retention = text(region, "retention");
            let kind = text(region, "kind");
            let evidence = text(region, "evidence");
            let proven = text(region, "confidence") == "proven" && !evidence.trim().is_empty();
            let permanent = retention == "keep_asm"
                || (retention == "keep_structured_asm" && proven)
                || matches!(
                    retention.as_str(),
                    "merge_with_owner"
                        | "merge_with_function_owner"
                        | "merge_with_continuations"
                        | "adjacent_section_alignment"
                )
                || kind.starts_with("deliberate_")
                || matches!(
                    kind.as_str(),
                    "literal_pool" | "alignment_padding" | "lookup_table"
                )
                || evidence.contains("approved_compiler_cannot_express");
            if permanent {
                if let (Some(a), Some(s)) = (integer(region, "address"), integer(region, "size")) {
                    spans.push(Span::new(a, a + s));
                }
            }
        }
    }
    if let Some(value) = tree
        .read("games/gs1/semantic/main-regions.json")
        .and_then(|s| serde_json::from_str::<Value>(&s).ok())
    {
        for region in array(&value, "non_c_ranges") {
            let kind = text(region, "kind");
            if matches!(
                kind.as_str(),
                "literal_pool" | "alignment_padding" | "lookup_table"
            ) && !text(region, "evidence").trim().is_empty()
            {
                if let (Some(a), Some(s)) = (
                    get(region, "address")
                        .and_then(|v| v.as_str())
                        .and_then(hex)
                        .or_else(|| integer(region, "address")),
                    integer(region, "size"),
                ) {
                    spans.push(Span::new(a, a + s));
                }
            }
        }
    }
    normalize(&spans)
}

fn permanent_overlay(inventory: &[Region]) -> Vec<Span> {
    normalize(
        &inventory
            .iter()
            .filter(|r| {
                matches!(
                    r.kind.as_str(),
                    "veneer" | "executable_alignment" | "hand_written_thumb"
                )
            })
            .map(|r| r.span)
            .collect::<Vec<_>>(),
    )
}

fn permanent_overlay_evidence(
    tree: &SourceTree,
    executable: &std::collections::BTreeMap<String, Vec<Span>>,
) -> Result<std::collections::BTreeMap<String, Vec<Span>>, String> {
    let Some(source) = tree.read("games/gs1/semantic/overlay-assembly.json") else {
        return Ok(std::collections::BTreeMap::new());
    };
    let document: Value = serde_json::from_str(&source)
        .map_err(|error| format!("games/gs1/semantic/overlay-assembly.json: {error}"))?;
    permanent_overlay_evidence_document(&document, executable)
}

fn permanent_overlay_evidence_document(
    document: &Value,
    executable: &std::collections::BTreeMap<String, Vec<Span>>,
) -> Result<std::collections::BTreeMap<String, Vec<Span>>, String> {
    if integer(document, "format") != Some(1) {
        return Err("overlay assembly evidence has unsupported format".into());
    }
    let mut out: std::collections::BTreeMap<String, Vec<Span>> = std::collections::BTreeMap::new();
    for (index, row) in array(&document, "regions").iter().enumerate() {
        let overlay = text(row, "overlay");
        let start = integer(row, "start")
            .or_else(|| hex(&text(row, "start")))
            .ok_or_else(|| format!("overlay assembly region {index} has no start"))?;
        let end = integer(row, "end")
            .or_else(|| hex(&text(row, "end")))
            .ok_or_else(|| format!("overlay assembly region {index} has no end"))?;
        let span = Span::new(start, end);
        let evidence = array(row, "evidence");
        if overlay.is_empty()
            || text(row, "kind").is_empty()
            || text(row, "retention") != "keep_structured_asm"
            || text(row, "confidence") != "proven"
            || evidence.is_empty()
            || evidence
                .iter()
                .any(|item| !matches!(item.as_str(), Some(text) if !text.trim().is_empty()))
            || span.end <= span.start
        {
            return Err(format!(
                "overlay assembly region {index} lacks proven retention evidence"
            ));
        }
        let Some(exec) = executable.get(&overlay) else {
            return Err(format!(
                "overlay assembly region {index} names unknown overlay {overlay}"
            ));
        };
        if bytes(&intersect(&[span], exec)) != span.bytes() {
            return Err(format!(
                "overlay assembly region {index} lies outside audited executable bytes"
            ));
        }
        out.entry(overlay).or_default().push(span);
    }
    for spans in out.values_mut() {
        let raw = bytes(spans);
        *spans = normalize(spans);
        if bytes(spans) != raw {
            return Err("overlay assembly evidence contains overlapping regions".into());
        }
    }
    Ok(out)
}

fn partition(executable: &[Span], cuts: &[i64]) -> Vec<Span> {
    let mut out = Vec::new();
    for run in normalize(executable) {
        let mut points = vec![run.start, run.end];
        points.extend(
            cuts.iter()
                .copied()
                .filter(|p| *p > run.start && *p < run.end),
        );
        points.sort_unstable();
        points.dedup();
        out.extend(points.windows(2).map(|w| Span::new(w[0], w[1])));
    }
    out
}

fn credit(
    label: String,
    span: Span,
    exact: &[Span],
    semantic: &[Span],
    retained: &[Span],
    group: Option<String>,
    address: Option<i64>,
) -> Tile {
    let e = bytes(&intersect(&[span], exact));
    let s = bytes(&intersect(&[span], semantic));
    let owned = [intersect(&[span], exact), intersect(&[span], semantic)].concat();
    let r = bytes(&intersect(&subtract(&[span], &normalize(&owned)), retained));
    let mut tile = Tile {
        label,
        bytes: span.bytes(),
        group,
        address,
        ..Tile::default()
    };
    tile.set("exact_c", e);
    tile.set("tracked_c", s);
    tile.set("retained_asm", r);
    tile.set("assembly", (span.bytes() - e - s - r).max(0));
    tile
}

fn main_tiles(
    executable: &[Span],
    exact: &[Span],
    semantic: &[Span],
    retained: &[Span],
    boundaries: &[i64],
) -> Vec<Tile> {
    let mut cuts = boundaries.to_vec();
    for run in executable {
        let mut p = (run.start / 0x10000 + 1) * 0x10000;
        while p < run.end {
            cuts.push(p);
            p += 0x10000;
        }
    }
    partition(executable, &cuts)
        .into_iter()
        .map(|span| {
            let bank = format!("{:04x} · 64 KiB bank", (span.start / 0x10000) * 0x10000);
            credit(
                format!("0x{:08x}–0x{:08x}", span.start, span.end),
                span,
                exact,
                semantic,
                retained,
                Some(bank),
                Some(span.start),
            )
        })
        .collect()
}

fn overlay_tiles(
    id: &str,
    executable: &[Span],
    owners: &[Owner],
    semantic: &[Span],
    retained: &[Span],
) -> Vec<Tile> {
    let exact: Vec<_> = owners
        .iter()
        .flat_map(|o| o.spans.iter().copied())
        .collect();
    let mut out = Vec::new();
    for owner in owners {
        let span = normalize(&owner.spans);
        let n = bytes(&span);
        if n == 0 {
            continue;
        }
        let mut tile = Tile {
            label: format!("{} · {} · byte-exact C", overlay_short(id), owner.label),
            bytes: n,
            group: Some(overlay_short(id).into()),
            address: Some(owner.entry),
            ..Tile::default()
        };
        tile.set("exact_c", n);
        out.push(tile);
    }
    for span in intersect(&subtract(executable, &exact), semantic) {
        out.push(credit(
            format!(
                "{} · tracked C 0x{:08x}–0x{:08x}",
                overlay_short(id),
                span.start,
                span.end
            ),
            span,
            &exact,
            semantic,
            retained,
            Some(overlay_short(id).into()),
            Some(span.start),
        ));
    }
    for span in subtract(
        executable,
        &[exact.clone(), semantic.to_vec(), retained.to_vec()].concat(),
    ) {
        out.push(credit(
            format!(
                "{} · byte-exact assembly 0x{:08x}–0x{:08x}",
                overlay_short(id),
                span.start,
                span.end
            ),
            span,
            &exact,
            semantic,
            retained,
            Some(overlay_short(id).into()),
            Some(span.start),
        ));
    }
    for span in intersect(executable, retained) {
        for residual in subtract(&[span], &exact) {
            out.push(credit(
                format!(
                    "{} · Permanent ASM 0x{:08x}–0x{:08x}",
                    overlay_short(id),
                    residual.start,
                    residual.end
                ),
                residual,
                &exact,
                &[],
                retained,
                Some(overlay_short(id).into()),
                Some(residual.start),
            ));
        }
    }
    out
}

fn bands(
    executable: &[Span],
    exact: &[Span],
    semantic: &[Span],
    retained: &[Span],
    target: i64,
) -> Vec<Tile> {
    let mut out = Vec::new();
    let mut current = Vec::new();
    let mut start = 0;
    for span in normalize(executable) {
        let mut at = span.start;
        while at < span.end {
            if current.is_empty() {
                start = at;
            }
            let room = target - bytes(&current);
            let end = (at + room.max(1)).min(span.end);
            current.push(Span::new(at, end));
            at = end;
            if bytes(&current) >= target {
                out.push(band_tile(start, &current, exact, semantic, retained));
                current.clear();
            }
        }
    }
    if !current.is_empty() {
        out.push(band_tile(start, &current, exact, semantic, retained));
    }
    out
}
fn band_tile(
    start: i64,
    spans: &[Span],
    exact: &[Span],
    semantic: &[Span],
    retained: &[Span],
) -> Tile {
    let n = bytes(spans);
    let e = bytes(&intersect(spans, exact));
    let s = bytes(&intersect(&subtract(spans, exact), semantic));
    let r = bytes(&intersect(
        &subtract(spans, &[exact.to_vec(), semantic.to_vec()].concat()),
        retained,
    ));
    let mut tile = Tile {
        label: format!("{:06x}", start),
        bytes: n,
        ..Tile::default()
    };
    tile.set("exact_c", e);
    tile.set("tracked_c", s);
    tile.set("retained_asm", r);
    tile.set("assembly", n - e - s - r);
    tile
}

#[derive(Clone)]
struct Stream {
    id: String,
    start: i64,
    rom: i64,
}
fn streams(tree: &SourceTree) -> Vec<Stream> {
    let Some(manifest) = tree
        .read("games/gs1/assets/manifest.json")
        .and_then(|s| serde_json::from_str::<Value>(&s).ok())
    else {
        return Vec::new();
    };
    let mut out = Vec::new();
    for series in array(&manifest, "series") {
        if text(series, "kind") != "golden-sun-thumb-overlay-series" {
            continue;
        }
        for row in array(series, "resources") {
            let Some(items) = row.as_array() else {
                continue;
            };
            if items.len() < 3 {
                continue;
            }
            let id = items[0].as_str().unwrap_or("").to_string();
            let start = items[1].as_str().and_then(hex).unwrap_or(0);
            let rom = items[2].as_str().and_then(hex).unwrap_or(0);
            if rom > 0 {
                out.push(Stream {
                    id: format!("resource_{id}"),
                    start,
                    rom,
                });
            }
        }
    }
    out
}

fn asset_tiles(tree: &SourceTree, data: &[Span], rom: i64) -> Vec<Tile> {
    let Some(manifest) = tree
        .read("out/gs1-en/full/assets/manifest.json")
        .and_then(|s| serde_json::from_str::<Value>(&s).ok())
    else {
        return vec![Tile {
            label: "Assets & data".into(),
            bytes: bytes(data),
            categories: [0, 0, 0, 0, bytes(data)],
            ..Tile::default()
        }];
    };
    let mut out = Vec::new();
    for region in array(&manifest, "regions") {
        let Some(start) = integer(region, "address") else {
            continue;
        };
        let Some(size) = integer(region, "size") else {
            continue;
        };
        let span = Span::new(start, start + size);
        let actual = bytes(&intersect(&[span], data));
        if actual == 0 {
            continue;
        }
        let kind = text(region, "kind");
        let source = array(region, "sources")
            .first()
            .and_then(Value::as_str)
            .unwrap_or(&kind);
        let mut tile = Tile {
            label: format!(
                "{} · {} · 0x{:08x}",
                source.rsplit('/').next().unwrap_or(source),
                kind,
                start
            ),
            bytes: actual,
            group: Some(kind.clone()),
            address: Some(start),
            ..Tile::default()
        };
        tile.set("asset_data", actual);
        let tier = if kind.contains("image") || kind.contains("graphics") {
            "asset_color"
        } else if kind.contains("font") || kind.contains("palette") {
            "asset_bw"
        } else {
            "asset_bytes"
        };
        tile.set(tier, actual);
        out.push(tile);
    }
    if out.is_empty() {
        out.push(Tile {
            label: format!("ROM data · {rom} bytes"),
            bytes: bytes(data),
            ..Tile::default()
        });
    }
    out
}

fn area_json(area: &Area, tracked: bool) -> Value {
    let cats = CATEGORIES
        .iter()
        .enumerate()
        .filter(|(i, _)| area.categories[*i] != 0)
        .map(|(i, name)| (name.to_string(), num(area.categories[i])))
        .collect::<Map<_, _>>();
    let tiles = if tracked {
        Value::Number((area.tiles.len() as u64).into())
    } else {
        Value::Array(area.tiles.iter().map(tile_json).collect())
    };
    obj(vec![
        ("id", Value::String(area.id.clone())),
        ("label", Value::String(area.label.clone())),
        ("bytes", num(area.bytes)),
        ("categories", Value::Object(cats)),
        ("tiles", tiles),
    ])
}
fn tile_json(tile: &Tile) -> Value {
    let cats = CATEGORIES
        .iter()
        .enumerate()
        .filter(|(i, _)| tile.categories[*i] != 0)
        .map(|(i, name)| (name.to_string(), num(tile.categories[i])))
        .collect::<Map<_, _>>();
    let mut out = vec![
        ("label".into(), Value::String(tile.label.clone())),
        ("bytes".into(), num(tile.bytes)),
        ("categories".into(), Value::Object(cats)),
    ];
    if let Some(v) = &tile.group {
        out.push(("group".into(), Value::String(v.clone())));
    }
    if let Some(v) = &tile.subgroup {
        out.push(("subgroup".into(), Value::String(v.clone())));
    }
    if let Some(v) = tile.address {
        out.push(("address".into(), num(v)));
    }
    Value::Object(out.into_iter().collect())
}
fn entry(bytes: i64, total: i64) -> Value {
    obj(vec![
        ("bytes", num(bytes)),
        (
            "percent_of_executable",
            serde_json::Number::from_f64(crate::jsnum::round_half_up(bytes, total))
                .map(Value::Number)
                .unwrap_or(Value::Null),
        ),
    ])
}

pub fn build_coverage_map(options: &BuildOptions) -> Result<CoverageMap, String> {
    let rom = rom_size(&options.target)?;
    let game = options.target.split('-').next().unwrap_or("gs1");
    let inventory = read_json(
        options.exact,
        &format!("games/{game}/metrics/{}-executable.json", options.target),
    )?;
    if text(&inventory, "audit") != "complete" {
        return Err(format!(
            "{} executable audit is incomplete; coverage map withheld",
            options.target
        ));
    }
    let main = regions(&get(&inventory, "main").cloned().unwrap_or(Value::Null));
    let main_exec = normalize(&main.iter().map(|r| r.span).collect::<Vec<_>>());
    let mut overlay_exec = std::collections::BTreeMap::new();
    let mut overlay_regions = std::collections::BTreeMap::new();
    for node in array(&inventory, "overlays") {
        let id = text(node, "id");
        let rows = regions(node);
        overlay_exec.insert(
            id.clone(),
            normalize(&rows.iter().map(|r| r.span).collect::<Vec<_>>()),
        );
        overlay_regions.insert(id, rows);
    }
    let exact_main = exact_main(options.exact, &main_exec);
    let pairs = overlay_ids(options.exact);
    let (owners, exact_overlay) = exact_overlay(options.exact, &pairs, &overlay_exec)?;
    let retained_main = permanent_main(options.exact);
    let explicit_retained_overlay = permanent_overlay_evidence(options.exact, &overlay_exec)?;
    let retained_overlay: std::collections::BTreeMap<_, _> = overlay_regions
        .iter()
        .map(|(id, regions)| {
            let mut spans = permanent_overlay(regions);
            spans.extend(
                explicit_retained_overlay
                    .get(id)
                    .cloned()
                    .unwrap_or_default(),
            );
            (id.clone(), normalize(&spans))
        })
        .collect();
    let (candidate_main, candidate_main_sources) = options
        .recon
        .map(|tree| candidate_main(tree, &main_exec))
        .unwrap_or_default();
    let (candidate_overlay, candidate_overlay_sources) = options
        .recon
        .map(|tree| candidate_overlay(tree, &overlay_exec))
        .unwrap_or_default();
    let semantic_main = subtract(
        &candidate_main,
        &[exact_main.clone(), retained_main.clone()].concat(),
    );
    let semantic_overlay: std::collections::BTreeMap<_, _> = candidate_overlay
        .into_iter()
        .map(|(id, spans)| {
            let exact = exact_overlay.get(&id).map(Vec::as_slice).unwrap_or(&[]);
            let retained = retained_overlay.get(&id).map(Vec::as_slice).unwrap_or(&[]);
            (
                id,
                subtract(&spans, &[exact.to_vec(), retained.to_vec()].concat()),
            )
        })
        .collect();
    let exact_overlay_bytes: i64 = exact_overlay.values().map(|v| bytes(v)).sum();
    let exact_bytes = bytes(&exact_main) + exact_overlay_bytes;
    let semantic_overlay_bytes: i64 = semantic_overlay.values().map(|v| bytes(v)).sum();
    let semantic_bytes = bytes(&semantic_main) + semantic_overlay_bytes;
    let mut executable_areas = vec![area(
        "main",
        "Main image",
        main_tiles(
            &main_exec,
            &exact_main,
            &semantic_main,
            &retained_main,
            &main.iter().map(|r| r.span.start).collect::<Vec<_>>(),
        ),
    )];
    let mut overlay_tiles_all = Vec::new();
    for (id, exec) in &overlay_exec {
        overlay_tiles_all.extend(overlay_tiles(
            id,
            exec,
            owners.get(id).map(Vec::as_slice).unwrap_or(&[]),
            semantic_overlay.get(id).map(Vec::as_slice).unwrap_or(&[]),
            retained_overlay.get(id).map(Vec::as_slice).unwrap_or(&[]),
        ));
    }
    executable_areas.push(area("overlays", "Decoded code overlays", overlay_tiles_all));
    let mut code = main_exec.clone();
    let ss = streams(options.exact);
    for stream in &ss {
        code.push(Span::new(stream.start, stream.start + stream.rom));
    }
    let data = subtract(&[Span::new(ROM_BASE, ROM_BASE + rom)], &code);
    let mut rom_areas = vec![area(
        "rom-main-code",
        "Main image code",
        bands(
            &main_exec,
            &exact_main,
            &semantic_main,
            &retained_main,
            65536,
        ),
    )];
    let mut stream_tiles = Vec::new();
    for stream in &ss {
        let decoded = bytes(
            overlay_exec
                .get(&stream.id)
                .map(Vec::as_slice)
                .unwrap_or(&[]),
        );
        let exact_part = if decoded == 0 {
            0
        } else {
            (stream.rom as f64
                * bytes(
                    exact_overlay
                        .get(&stream.id)
                        .map(Vec::as_slice)
                        .unwrap_or(&[]),
                ) as f64
                / decoded as f64)
                .round() as i64
        };
        let semantic_part = if decoded == 0 {
            0
        } else {
            (stream.rom as f64
                * bytes(
                    semantic_overlay
                        .get(&stream.id)
                        .map(Vec::as_slice)
                        .unwrap_or(&[]),
                ) as f64
                / decoded as f64)
                .round() as i64
        };
        let mut tile = Tile {
            label: overlay_short(&stream.id).into(),
            bytes: stream.rom,
            ..Tile::default()
        };
        tile.set("exact_c", exact_part);
        tile.set("tracked_c", semantic_part);
        tile.set("assembly", stream.rom - exact_part - semantic_part);
        stream_tiles.push(tile);
    }
    let mut grouped = Vec::new();
    let mut current: Option<Tile> = None;
    let mut first = String::new();
    let mut last = String::new();
    for tile in stream_tiles {
        if current.is_none() {
            first = tile.label.clone();
            current = Some(Tile {
                label: first.clone(),
                ..Tile::default()
            });
        }
        let item = current.as_mut().unwrap();
        item.bytes += tile.bytes;
        for (a, b) in item.categories.iter_mut().zip(tile.categories) {
            *a += b;
        }
        last = tile.label;
        if item.bytes >= 49152 {
            item.label = if first == last {
                first.clone()
            } else {
                format!("{first}–{last}")
            };
            grouped.push(current.take().unwrap());
        }
    }
    if let Some(mut tile) = current {
        tile.label = if first == last {
            first
        } else {
            format!("{first}–{last}")
        };
        grouped.push(tile);
    }
    rom_areas.push(area(
        "rom-overlay-streams",
        "Compressed code overlays",
        grouped,
    ));
    rom_areas.push(area(
        "rom-data",
        "Assets & data",
        asset_tiles(options.exact, &data, rom),
    ));
    let executable = bytes(&main_exec) + overlay_exec.values().map(|v| bytes(v)).sum::<i64>();
    let retained = executable_areas
        .iter()
        .map(|a| a.categories[3])
        .sum::<i64>();
    let assembly = executable - exact_bytes - semantic_bytes - retained;
    let document = obj(vec![
        ("format", num(1)),
        ("kind", Value::String("golden-sun-rom-coverage-map".into())),
        ("target", Value::String(options.target.clone())),
        ("derivation", Value::String("tracked-evidence-v1".into())),
        ("rom_bytes", num(rom)),
        ("executable_bytes", num(executable)),
        (
            "categories",
            obj(vec![
                ("exact_c", entry(exact_bytes, executable)),
                ("tracked_c", entry(semantic_bytes, executable)),
                ("assembly", entry(assembly, executable)),
                ("retained_asm", entry(retained, executable)),
                (
                    "asset_data",
                    obj(vec![
                        ("bytes", num(bytes(&data))),
                        ("percent_of_executable", num(0)),
                    ]),
                ),
            ]),
        ),
        (
            "main",
            obj(vec![
                ("executable_bytes", num(bytes(&main_exec))),
                ("exact_c_bytes", num(bytes(&exact_main))),
                ("tracked_c_bytes", num(bytes(&semantic_main))),
            ]),
        ),
        (
            "overlays",
            obj(vec![
                ("executable_bytes", num(executable - bytes(&main_exec))),
                ("exact_c_bytes", num(exact_overlay_bytes)),
                ("tracked_c_bytes", num(semantic_overlay_bytes)),
            ]),
        ),
        (
            "provenance",
            obj(vec![
                ("exact_source", Value::String(options.exact.id().into())),
                (
                    "tracked_source",
                    Value::String(
                        options
                            .recon
                            .map_or_else(|| "absent".into(), |tree| tree.id().into()),
                    ),
                ),
                (
                    "tracked_sources",
                    num((candidate_main_sources + candidate_overlay_sources) as i64),
                ),
                (
                    "main_tracked_census",
                    Value::String("games/gs1/recon/en/main/*.json".into()),
                ),
                ("tracked_superseded_bytes", num(0)),
                ("tracked_outside_extent_bytes", num(0)),
                ("tracked_unresolved", Value::Array(Vec::new())),
            ]),
        ),
        (
            "rom_areas",
            Value::Array(rom_areas.iter().map(|a| area_json(a, false)).collect()),
        ),
        (
            "executable_areas",
            Value::Array(
                executable_areas
                    .iter()
                    .map(|a| area_json(a, false))
                    .collect(),
            ),
        ),
    ]);
    if options.validate_tracked_progress {
        if let Some(tracked) = options
            .exact
            .read(&format!(
                "games/{game}/metrics/{}-progress.json",
                options.target
            ))
            .and_then(|s| serde_json::from_str::<Value>(&s).ok())
        {
            let tm = get(
                get(&tracked, "main").unwrap_or(&Value::Null),
                "full_c_bytes",
            )
            .and_then(Value::as_i64)
            .unwrap_or(-1);
            let to = get(
                get(&tracked, "overlays").unwrap_or(&Value::Null),
                "full_c_bytes",
            )
            .and_then(Value::as_i64)
            .unwrap_or(-1);
            if tm != bytes(&exact_main) || to != exact_overlay_bytes {
                return Err(format!("derived exact ownership disagrees with tracked Full-C report (main {} vs {}, overlays {} vs {})", bytes(&exact_main), tm, exact_overlay_bytes, to));
            }
        }
    }
    Ok(CoverageMap {
        document,
        rom_areas,
        executable_areas,
    })
}

#[cfg(test)]
mod tests {
    use super::*;
    use serde_json::json;
    use std::collections::BTreeMap;

    fn evidence(regions: Value) -> Value {
        json!({"format": 1, "regions": regions})
    }

    fn region(start: &str, end: &str) -> Value {
        json!({
            "overlay": "resource_test",
            "start": start,
            "end": end,
            "kind": "compiler_ordering_module",
            "retention": "keep_structured_asm",
            "confidence": "proven",
            "evidence": ["approved compiler probe"]
        })
    }

    fn executable() -> BTreeMap<String, Vec<Span>> {
        BTreeMap::from([(
            "resource_test".into(),
            vec![Span::new(0x0200_0100, 0x0200_0200)],
        )])
    }

    #[test]
    fn accepts_proven_overlay_assembly_inside_inventory() {
        let found = permanent_overlay_evidence_document(
            &evidence(json!([region("0x02000120", "0x02000140")])),
            &executable(),
        )
        .unwrap();
        assert_eq!(
            found["resource_test"],
            vec![Span::new(0x0200_0120, 0x0200_0140)]
        );
    }

    #[test]
    fn rejects_overlay_assembly_outside_inventory() {
        let error = permanent_overlay_evidence_document(
            &evidence(json!([region("0x020000f0", "0x02000120")])),
            &executable(),
        )
        .unwrap_err();
        assert!(error.contains("outside audited executable bytes"));
    }

    #[test]
    fn rejects_overlapping_overlay_assembly_evidence() {
        let error = permanent_overlay_evidence_document(
            &evidence(json!([
                region("0x02000120", "0x02000160"),
                region("0x02000140", "0x02000180")
            ])),
            &executable(),
        )
        .unwrap_err();
        assert!(error.contains("overlapping regions"));
    }
}
