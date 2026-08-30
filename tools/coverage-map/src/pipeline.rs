use crate::model::{
    area, bytes, intersect, normalize, subtract, Area, Category, Span, Tile, CATEGORIES,
};
use crate::tree::{read_json, SourceTree, ROM_BASE};
use compiler_core::source_paths::{SourceOwner, SourcePaths, SOURCE_PATHS_MANIFEST};
use serde_json::{Map, Value};
use std::{collections::BTreeMap, path::Path};
type SpanMap = BTreeMap<String, Vec<Span>>;
type OwnerMap = BTreeMap<String, Vec<Owner>>;

fn mapped<'a, T>(map: &'a BTreeMap<String, Vec<T>>, id: &str) -> &'a [T] {
    map.get(id).map(Vec::as_slice).unwrap_or(&[])
}
fn mapped_bytes(map: &SpanMap) -> i64 {
    map.values().map(|spans| bytes(spans)).sum()
}
pub struct BuildOptions<'a> {
    pub target: String,
    pub exact: &'a SourceTree,
    pub recon: Option<&'a SourceTree>,
    pub prefer_verified_assets: bool,
}
pub struct CoverageMap {
    pub document: Value,
    pub rom_areas: Vec<Area>,
    pub executable_areas: Vec<Area>,
}
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct ProgressTally {
    pub main_exact: i64,
    pub main_executable: i64,
    pub overlay_exact: i64,
    pub overlay_executable: i64,
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
fn json(tree: &SourceTree, path: &str) -> Option<Value> {
    tree.read(path)
        .and_then(|source| serde_json::from_str(&source).ok())
}
struct Region {
    span: Span,
    kind: String,
    evidence: String,
}
fn regions(value: &Value) -> Vec<Region> {
    array(value, "intervals")
        .iter()
        .filter_map(|item| {
            Some(Region {
                span: Span::new(integer(item, "start")?, integer(item, "end")?),
                kind: text(item, "kind"),
                evidence: text(item, "evidence"),
            })
        })
        .filter(|r| r.span.end > r.span.start)
        .collect()
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
fn address(value: &Value, key: &str) -> Option<i64> {
    integer(value, key).or_else(|| get(value, key).and_then(Value::as_str).and_then(hex))
}
fn space(line: &str) -> Option<i64> {
    let value = line.trim().strip_prefix(".space")?.trim();
    if value.starts_with('-') {
        return None;
    }
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
pub fn overlay_name(name: &str) -> Option<String> {
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
pub fn overlay_owners(tree: &SourceTree, name: &str) -> Vec<Owner> {
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
pub fn overlay_ids(tree: &SourceTree) -> Vec<(String, String)> {
    let mut names: Vec<_> = tree
        .list("games/gs1/assets/code")
        .into_iter()
        .filter_map(|name| overlay_name(&name).map(|id| (id, name)))
        .collect();
    names.sort_by(|a, b| a.0.cmp(&b.0));
    names
}
fn exact_main(tree: &SourceTree, target: &str, executable: &[Span]) -> Result<Vec<Span>, String> {
    let path = format!("out/{target}/full/claimed/manifest.json");
    let value = read_json(tree, &path)?;
    let mut spans = Vec::new();
    for region in array(&value, "regions") {
        let start = integer(region, "address").ok_or_else(|| format!("{path}: bad address"))?;
        let size = integer(region, "size").ok_or_else(|| format!("{path}: bad size"))?;
        let source = text(region, "source");
        let code = tree
            .read(&source)
            .ok_or_else(|| format!("{} is missing {source}", tree.id()))?;
        if size > 0 && canonical(&code) {
            spans.push(Span::new(start, start + size));
        }
    }
    exact_spans(spans, executable, "main")
}
fn exact_spans(mut spans: Vec<Span>, executable: &[Span], id: &str) -> Result<Vec<Span>, String> {
    spans.sort_by_key(|span| (span.start, span.end));
    if spans
        .windows(2)
        .any(|pair| pair[1].start < pair[0].end && pair[0] != pair[1])
    {
        return Err(format!("{id} has overlapping C ownership"));
    }
    if spans
        .iter()
        .any(|span| intersect(&[*span], executable).is_empty())
    {
        return Err(format!(
            "{id} C ownership is outside audited executable intervals"
        ));
    }
    Ok(intersect(&spans, executable))
}

fn validated_executable(value: &Value) -> Result<Vec<Span>, String> {
    let id = text(value, "id");
    let expected = integer(value, "executable_bytes")
        .ok_or_else(|| "executable inventory has a non-integer byte count".to_string())?;
    let mut spans: Vec<_> = regions(value).into_iter().map(|row| row.span).collect();
    spans.sort_by_key(|span| (span.start, span.end));
    if spans.windows(2).any(|pair| pair[1].start < pair[0].end) {
        return Err(format!("{id} has overlapping executable intervals"));
    }
    let spans = normalize(&spans);
    let measured = bytes(&spans);
    if measured != expected {
        return Err(format!(
            "{id} executable total is stale: {expected} != {measured}"
        ));
    }
    if let Some(decoded) = integer(value, "decoded_bytes") {
        if integer(value, "excluded_bytes") != Some(decoded - expected) {
            return Err(format!("{id} decoded byte classification is incomplete"));
        }
    }
    Ok(spans)
}

/// Exact/executable totals without constructing treemaps, asset bands, or
/// semantic-candidate coverage. Progress reporting and commit hooks use this
/// narrow view of the same audited intervals and exact-owner model.
pub fn progress_tally(options: &BuildOptions) -> Result<ProgressTally, String> {
    let game = options.target.split('-').next().unwrap_or("gs1");
    let inventory = read_json(
        options.exact,
        &format!("games/{game}/metrics/{}-executable.json", options.target),
    )?;
    if integer(&inventory, "format") != Some(1)
        || text(&inventory, "metric") != "full-c-byte-share"
        || text(&inventory, "target") != options.target
    {
        return Err("unsupported executable inventory format or target".into());
    }
    let main_node = get(&inventory, "main").ok_or("executable inventory has no main")?;
    if text(&inventory, "audit") != "complete" || text(main_node, "audit") != "complete" {
        return Err(format!(
            "Full-C Byte Share withheld: {} executable audit is incomplete",
            options.target
        ));
    }
    let main = validated_executable(main_node)?;
    let mut overlays = SpanMap::new();
    for node in array(&inventory, "overlays") {
        let id = text(node, "id");
        if text(node, "audit") != "complete" {
            return Err(format!("Full-C Byte Share withheld: {id} is incomplete"));
        }
        overlays.insert(id, validated_executable(node)?);
    }
    let executable = bytes(&main) + mapped_bytes(&overlays);
    if integer(&inventory, "total_union_bytes") != Some(executable) {
        return Err("executable inventory total is stale".into());
    }
    let exact_main = exact_main(options.exact, &options.target, &main)?;
    let (_, exact_overlays) = exact_overlay(options.exact, &overlay_ids(options.exact), &overlays)?;
    Ok(ProgressTally {
        main_exact: bytes(&exact_main),
        main_executable: bytes(&main),
        overlay_exact: mapped_bytes(&exact_overlays),
        overlay_executable: mapped_bytes(&overlays),
    })
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
        let Some(record) = json(tree, &format!("{directory}/{name}")) else {
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
fn candidate_overlay(tree: &SourceTree, executable: &SpanMap) -> (SpanMap, usize) {
    let directory = "games/gs1/recon/en/overlays";
    let reviewed = json(tree, "games/gs1/semantic/regions.json").unwrap_or(Value::Null);
    let mut extents = BTreeMap::new();
    for region in array(&reviewed, "manual_regions") {
        let id = text(region, "overlay");
        let entry = address(region, "entry");
        if let (Some(entry), Some(size)) = (entry, integer(region, "span_bytes")) {
            extents.insert((id, entry), size);
        }
    }
    let mut spans = SpanMap::new();
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
        let record_size = json(tree, &format!("{directory}/{stem}.json"))
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
        *found = intersect(&normalize(found), mapped(executable, id));
    }
    (spans, sources)
}
fn exact_overlay(
    tree: &SourceTree,
    pairs: &[(String, String)],
    executable: &SpanMap,
) -> Result<(OwnerMap, SpanMap), String> {
    let manifest = tree
        .read(SOURCE_PATHS_MANIFEST)
        .ok_or_else(|| format!("missing canonical owner register {SOURCE_PATHS_MANIFEST}"))?;
    let source_paths = SourcePaths::parse(Path::new(""), &manifest)?;
    let mut owners = OwnerMap::new();
    let mut spans = SpanMap::new();
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
                owner.spans = intersect(&owner.spans, mapped(executable, id));
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
fn main_assembly_classification(tree: &SourceTree) -> (Vec<Span>, Vec<Span>) {
    let mut proven = Vec::new();
    let mut draft = Vec::new();
    if let Some(value) = json(tree, "out/gs1-en/full/asm/manifest.json") {
        for region in array(&value, "regions") {
            let retention = text(region, "retention");
            let kind = text(region, "kind");
            let evidence = text(region, "evidence");
            let classified = retention == "keep_asm"
                || retention == "keep_structured_asm"
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
            if classified && !evidence.trim().is_empty() {
                if let (Some(address), Some(size)) =
                    (integer(region, "address"), integer(region, "size"))
                {
                    let span = Span::new(address, address + size);
                    if text(region, "confidence") == "proven" {
                        proven.push(span);
                    } else {
                        draft.push(span);
                    }
                }
            }
        }
    }
    if let Some(value) = json(tree, "games/gs1/semantic/main-regions.json") {
        for region in array(&value, "non_c_ranges") {
            if matches!(
                text(region, "kind").as_str(),
                "literal_pool" | "alignment_padding" | "lookup_table"
            ) && !text(region, "evidence").trim().is_empty()
            {
                if let (Some(address), Some(size)) =
                    (address(region, "address"), integer(region, "size"))
                {
                    proven.push(Span::new(address, address + size));
                }
            }
        }
    }
    (normalize(&proven), normalize(&draft))
}
fn overlay_assembly_classification(
    tree: &SourceTree,
    inventory: &BTreeMap<String, Vec<Region>>,
    executable: &SpanMap,
) -> Result<(SpanMap, SpanMap), String> {
    let source = tree
        .read("games/gs1/semantic/overlay-assembly.json")
        .ok_or_else(|| "overlay assembly classification is missing".to_string())?;
    let document: Value = serde_json::from_str(&source)
        .map_err(|error| format!("games/gs1/semantic/overlay-assembly.json: {error}"))?;
    overlay_assembly_classification_document(&document, inventory, executable)
}
fn overlay_assembly_classification_document(
    document: &Value,
    inventory: &BTreeMap<String, Vec<Region>>,
    executable: &SpanMap,
) -> Result<(SpanMap, SpanMap), String> {
    let mut proven: SpanMap = inventory
        .iter()
        .map(|(id, regions)| {
            (
                id.clone(),
                normalize(
                    &regions
                        .iter()
                        .filter(|region| {
                            !region.evidence.trim().is_empty()
                                && matches!(
                                    region.kind.as_str(),
                                    "veneer" | "executable_alignment" | "hand_written_thumb"
                                )
                        })
                        .map(|region| region.span)
                        .collect::<Vec<_>>(),
                ),
            )
        })
        .collect();
    let mut draft = SpanMap::new();
    if integer(document, "format") != Some(1) {
        return Err("overlay assembly classification has unsupported format".into());
    }
    for (index, row) in array(document, "regions").iter().enumerate() {
        let overlay = text(row, "overlay");
        let start = address(row, "start")
            .ok_or_else(|| format!("assembly classification {index} has no start"))?;
        let end = address(row, "end")
            .ok_or_else(|| format!("assembly classification {index} has no end"))?;
        let span = Span::new(start, end);
        let evidence = array(row, "evidence");
        if text(row, "retention") != "keep_structured_asm"
            || evidence.is_empty()
            || evidence
                .iter()
                .any(|item| !matches!(item.as_str(), Some(text) if !text.trim().is_empty()))
        {
            return Err(format!(
                "assembly classification {index} lacks retention reasoning"
            ));
        }
        let Some(exec) = executable.get(&overlay) else {
            return Err(format!(
                "assembly classification {index} names unknown overlay {overlay}"
            ));
        };
        if span.end <= span.start || bytes(&intersect(&[span], exec)) != span.bytes() {
            return Err(format!(
                "assembly classification {index} lies outside audited executable bytes"
            ));
        }
        if text(row, "confidence") == "proven" {
            proven.entry(overlay).or_default().push(span);
        } else {
            draft.entry(overlay).or_default().push(span);
        }
    }
    for spans in proven.values_mut() {
        *spans = normalize(spans);
    }
    for spans in draft.values_mut() {
        *spans = normalize(spans);
    }
    Ok((proven, draft))
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
fn code_tile(
    label: String,
    spans: &[Span],
    exact: &[Span],
    semantic: &[Span],
    draft_assembly: &[Span],
    retained: &[Span],
    group: Option<String>,
    address: Option<i64>,
) -> Tile {
    let n = bytes(spans);
    let e = bytes(&intersect(spans, exact));
    let s = bytes(&intersect(&subtract(spans, exact), semantic));
    let r = bytes(&intersect(
        &subtract(spans, &[exact.to_vec(), semantic.to_vec()].concat()),
        retained,
    ));
    let d = bytes(&intersect(
        &subtract(
            spans,
            &[exact.to_vec(), semantic.to_vec(), retained.to_vec()].concat(),
        ),
        draft_assembly,
    ));
    Tile {
        label,
        bytes: n,
        categories: [e, s, (n - e - s - d - r).max(0), d, r, 0],
        group,
        address,
        ..Tile::default()
    }
}
fn main_tiles(
    executable: &[Span],
    exact: &[Span],
    semantic: &[Span],
    draft_assembly: &[Span],
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
            code_tile(
                format!("0x{:08x}–0x{:08x}", span.start, span.end),
                &[span],
                exact,
                semantic,
                draft_assembly,
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
    draft_assembly: &[Span],
    retained: &[Span],
) -> Vec<Tile> {
    let short = overlay_short(id);
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
        out.push(code_tile(
            format!("{short} · {} · Proven C", owner.label),
            &span,
            &span,
            &[],
            &[],
            &[],
            Some(short.into()),
            Some(owner.entry),
        ));
    }
    let residuals = [
        (
            "Draft C",
            intersect(&subtract(executable, &exact), semantic),
            semantic,
        ),
        (
            "Unknown",
            subtract(
                executable,
                &[
                    exact.clone(),
                    semantic.to_vec(),
                    draft_assembly.to_vec(),
                    retained.to_vec(),
                ]
                .concat(),
            ),
            semantic,
        ),
        (
            "Draft ASM",
            subtract(
                &intersect(executable, draft_assembly),
                &[exact.clone(), semantic.to_vec(), retained.to_vec()].concat(),
            ),
            &[] as &[Span],
        ),
        (
            "Proven ASM",
            subtract(&intersect(executable, retained), &exact),
            &[] as &[Span],
        ),
    ];
    for (kind, spans, tracked) in residuals {
        for span in spans {
            out.push(code_tile(
                format!("{short} · {kind} 0x{:08x}–0x{:08x}", span.start, span.end),
                &[span],
                &exact,
                tracked,
                draft_assembly,
                retained,
                Some(short.into()),
                Some(span.start),
            ));
        }
    }
    out
}
fn bands(
    executable: &[Span],
    exact: &[Span],
    semantic: &[Span],
    draft_assembly: &[Span],
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
                out.push(code_tile(
                    format!("{:06x}", start),
                    &current,
                    exact,
                    semantic,
                    draft_assembly,
                    retained,
                    None,
                    None,
                ));
                current.clear();
            }
        }
    }
    if !current.is_empty() {
        out.push(code_tile(
            format!("{:06x}", start),
            &current,
            exact,
            semantic,
            draft_assembly,
            retained,
            None,
            None,
        ));
    }
    out
}
struct Stream {
    id: String,
    start: i64,
    rom: i64,
}
fn streams(tree: &SourceTree) -> Vec<Stream> {
    let Some(manifest) = json(tree, "games/gs1/assets/manifest.json") else {
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
fn scaled_bytes(rom: i64, decoded: i64, spans: &[Span]) -> i64 {
    if decoded == 0 {
        0
    } else {
        (rom as f64 * bytes(spans) as f64 / decoded as f64).round() as i64
    }
}
fn asset_tiles(tree: &SourceTree, data: &[Span], rom: i64) -> Vec<Tile> {
    let Some(manifest) = json(tree, "out/gs1-en/full/assets/manifest.json") else {
        return vec![Tile {
            label: "Assets & data".into(),
            bytes: bytes(data),
            categories: [0, 0, 0, 0, 0, bytes(data)],
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
        out.push(Tile {
            label: format!(
                "{} · {} · 0x{:08x}",
                source.rsplit('/').next().unwrap_or(source),
                kind,
                start
            ),
            bytes: actual,
            categories: [0, 0, 0, 0, 0, actual],
            group: Some(kind.clone()),
            address: Some(start),
            ..Tile::default()
        });
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
fn categories_json(values: &[i64; 6]) -> Value {
    Value::Object(
        CATEGORIES
            .iter()
            .filter(|(category, _, _)| values[*category as usize] != 0)
            .map(|(category, name, _)| (name.to_string(), num(values[*category as usize])))
            .collect::<Map<_, _>>(),
    )
}
fn area_json(area: &Area) -> Value {
    obj(vec![
        ("id", Value::String(area.id.clone())),
        ("label", Value::String(area.label.clone())),
        ("bytes", num(area.bytes)),
        ("categories", categories_json(&area.categories)),
        (
            "tiles",
            Value::Array(area.tiles.iter().map(tile_json).collect()),
        ),
    ])
}
fn tile_json(tile: &Tile) -> Value {
    let mut out = vec![
        ("label".into(), Value::String(tile.label.clone())),
        ("bytes".into(), num(tile.bytes)),
        ("categories".into(), categories_json(&tile.categories)),
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
    let mut overlay_exec = SpanMap::new();
    let mut overlay_regions = BTreeMap::new();
    for node in array(&inventory, "overlays") {
        let id = text(node, "id");
        let rows = regions(node);
        overlay_exec.insert(
            id.clone(),
            normalize(&rows.iter().map(|r| r.span).collect::<Vec<_>>()),
        );
        overlay_regions.insert(id, rows);
    }
    let exact_main = exact_main(options.exact, &options.target, &main_exec)?;
    let pairs = overlay_ids(options.exact);
    let (owners, exact_overlay) = exact_overlay(options.exact, &pairs, &overlay_exec)?;
    let (retained_main, draft_main) = main_assembly_classification(options.exact);
    let (retained_overlay, draft_overlay) =
        overlay_assembly_classification(options.exact, &overlay_regions, &overlay_exec)?;
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
    let semantic_overlay: SpanMap = candidate_overlay
        .into_iter()
        .map(|(id, spans)| {
            let exact = mapped(&exact_overlay, &id);
            let retained = mapped(&retained_overlay, &id);
            (
                id,
                subtract(&spans, &[exact.to_vec(), retained.to_vec()].concat()),
            )
        })
        .collect();
    let exact_overlay_bytes = mapped_bytes(&exact_overlay);
    let exact_bytes = bytes(&exact_main) + exact_overlay_bytes;
    let semantic_overlay_bytes = mapped_bytes(&semantic_overlay);
    let semantic_bytes = bytes(&semantic_main) + semantic_overlay_bytes;
    let mut executable_areas = vec![area(
        "main",
        "Main image",
        main_tiles(
            &main_exec,
            &exact_main,
            &semantic_main,
            &draft_main,
            &retained_main,
            &main.iter().map(|r| r.span.start).collect::<Vec<_>>(),
        ),
    )];
    let mut overlay_tiles_all = Vec::new();
    for (id, exec) in &overlay_exec {
        overlay_tiles_all.extend(overlay_tiles(
            id,
            exec,
            mapped(&owners, id),
            mapped(&semantic_overlay, id),
            mapped(&draft_overlay, id),
            mapped(&retained_overlay, id),
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
            &draft_main,
            &retained_main,
            65536,
        ),
    )];
    let mut stream_tiles = Vec::new();
    for stream in &ss {
        let decoded = bytes(mapped(&overlay_exec, &stream.id));
        let exact_part = scaled_bytes(stream.rom, decoded, mapped(&exact_overlay, &stream.id));
        let semantic_part =
            scaled_bytes(stream.rom, decoded, mapped(&semantic_overlay, &stream.id));
        let retained_part =
            scaled_bytes(stream.rom, decoded, mapped(&retained_overlay, &stream.id));
        let draft_part = scaled_bytes(
            stream.rom,
            decoded,
            &subtract(
                mapped(&draft_overlay, &stream.id),
                &[
                    mapped(&exact_overlay, &stream.id).to_vec(),
                    mapped(&semantic_overlay, &stream.id).to_vec(),
                    mapped(&retained_overlay, &stream.id).to_vec(),
                ]
                .concat(),
            ),
        );
        stream_tiles.push(Tile {
            label: overlay_short(&stream.id).into(),
            bytes: stream.rom,
            categories: [
                exact_part,
                semantic_part,
                stream.rom - exact_part - semantic_part - draft_part - retained_part,
                draft_part,
                retained_part,
                0,
            ],
            ..Tile::default()
        });
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
    let executable = bytes(&main_exec) + mapped_bytes(&overlay_exec);
    let retained = executable_areas
        .iter()
        .map(|a| a.categories[Category::ProvenAsm as usize])
        .sum::<i64>();
    let draft_assembly = executable_areas
        .iter()
        .map(|area| area.categories[Category::DraftAsm as usize])
        .sum::<i64>();
    let assembly = executable - exact_bytes - semantic_bytes - draft_assembly - retained;
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
                ("proven_c", entry(exact_bytes, executable)),
                ("draft_c", entry(semantic_bytes, executable)),
                ("draft_asm", entry(draft_assembly, executable)),
                ("unknown", entry(assembly, executable)),
                ("proven_asm", entry(retained, executable)),
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
                ("proven_c_bytes", num(bytes(&exact_main))),
                ("draft_c_bytes", num(bytes(&semantic_main))),
            ]),
        ),
        (
            "overlays",
            obj(vec![
                ("executable_bytes", num(executable - bytes(&main_exec))),
                ("proven_c_bytes", num(exact_overlay_bytes)),
                ("draft_c_bytes", num(semantic_overlay_bytes)),
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
                    "main_draft_census",
                    Value::String("games/gs1/recon/en/main/*.json".into()),
                ),
                (
                    "proven_assembly_standard",
                    Value::String("approved-compiler-non-emittable-with-reasoning".into()),
                ),
                (
                    "main_assembly_classification",
                    Value::String("out/gs1-en/full/asm/manifest.json".into()),
                ),
                (
                    "overlay_assembly_classification",
                    Value::String("games/gs1/semantic/overlay-assembly.json".into()),
                ),
                ("tracked_superseded_bytes", num(0)),
                ("tracked_outside_extent_bytes", num(0)),
                ("tracked_unresolved", Value::Array(Vec::new())),
            ]),
        ),
        (
            "rom_areas",
            Value::Array(rom_areas.iter().map(area_json).collect()),
        ),
        (
            "executable_areas",
            Value::Array(executable_areas.iter().map(area_json).collect()),
        ),
    ]);
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

    fn classification(regions: Value) -> Value {
        json!({"format": 1, "regions": regions})
    }
    fn region(start: &str, end: &str, confidence: &str, evidence: Value) -> Value {
        json!({
            "overlay": "resource_test",
            "start": start,
            "end": end,
            "kind": "thumb_multi_register_module",
            "retention": "keep_structured_asm",
            "confidence": confidence,
            "evidence": evidence
        })
    }
    fn executable() -> BTreeMap<String, Vec<Span>> {
        BTreeMap::from([(
            "resource_test".into(),
            vec![Span::new(0x0200_0100, 0x0200_0200)],
        )])
    }
    fn no_inventory() -> BTreeMap<String, Vec<Region>> {
        BTreeMap::new()
    }
    #[test]
    fn exact_ownership_fails_closed() {
        let executable = [Span::new(10, 20)];
        assert!(exact_spans(vec![Span::new(10, 16), Span::new(14, 18)], &executable, "x").is_err());
        assert!(exact_spans(vec![Span::new(0, 4)], &executable, "x").is_err());
        let missing = SourceTree::Work {
            id: "fixture".into(),
            root: std::env::temp_dir()
                .join(format!("alchemy-coverage-missing-{}", std::process::id())),
        };
        assert!(exact_main(&missing, "gs1-en", &executable).is_err());
    }
    #[test]
    fn evidence_backed_proven_assembly_is_counted() {
        let (found, draft) = overlay_assembly_classification_document(
            &classification(json!([region(
                "0x02000120",
                "0x02000140",
                "proven",
                json!(["approved compiler cannot emit multi-register Thumb transfer"])
            )])),
            &no_inventory(),
            &executable(),
        )
        .unwrap();
        assert!(draft.is_empty());
        assert_eq!(
            found["resource_test"],
            vec![Span::new(0x0200_0120, 0x0200_0140)]
        );
    }
    #[test]
    fn strong_assembly_reasoning_remains_draft() {
        let (proven, draft) = overlay_assembly_classification_document(
            &classification(json!([region(
                "0x02000120",
                "0x02000140",
                "strong",
                json!(["instruction shape strongly suggests assembly"])
            )])),
            &no_inventory(),
            &executable(),
        )
        .unwrap();
        assert!(proven.values().all(Vec::is_empty));
        assert_eq!(
            draft["resource_test"],
            vec![Span::new(0x0200_0120, 0x0200_0140)]
        );
    }
    #[test]
    fn rejects_assembly_classification_outside_inventory() {
        let error = overlay_assembly_classification_document(
            &classification(json!([region(
                "0x020000f0",
                "0x02000120",
                "proven",
                json!(["approved compiler cannot emit instruction form"])
            )])),
            &no_inventory(),
            &executable(),
        )
        .unwrap_err();
        assert!(error.contains("outside audited executable bytes"));
    }
    #[test]
    fn rejects_proven_assembly_without_reasoning() {
        let error = overlay_assembly_classification_document(
            &classification(json!([region(
                "0x02000120",
                "0x02000140",
                "proven",
                json!([])
            )])),
            &no_inventory(),
            &executable(),
        )
        .unwrap_err();
        assert!(error.contains("lacks retention reasoning"));
    }
}
