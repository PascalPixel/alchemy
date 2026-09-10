use super::model::{
    area, bytes, intersect, normalize, subtract, Area, Category, Span, Tile, CATEGORIES,
};
use crate::compiler::source_paths::{SourceOwner, SourcePaths, SOURCE_PATHS_MANIFEST};
use crate::coverage::tree::{read_json, SourceTree, ROM_BASE};
use serde_json::{json, Map, Value};
use std::{
    collections::{BTreeMap, BTreeSet},
    path::Path,
};
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
    pub source: String,
    pub entry: i64,
    pub spans: Vec<Span>,
}
pub fn overlay_owners(tree: &SourceTree, name: &str) -> Vec<Owner> {
    let Some(source) = tree.read(&format!("games/gs1/asm/overlays/{name}")) else {
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
                source: String::new(),
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
        .list("games/gs1/asm/overlays")
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
    let dossiers = json(tree, "games/gs1/recon/en/dossiers.json").unwrap_or(Value::Null);
    let records = dossiers.get("records").and_then(Value::as_object);
    let mut spans = Vec::new();
    let mut sources = 0;
    for name in tree.list(directory) {
        let Some(stem) = name.strip_suffix(".c") else {
            continue;
        };
        if !tree
            .read(&format!("{directory}/{stem}.c"))
            .is_some_and(|s| canonical(&s))
        {
            continue;
        }
        let Some(record) = records.and_then(|records| records.get(&format!("main:{stem}"))) else {
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
    let dossiers = json(tree, "games/gs1/recon/en/dossiers.json").unwrap_or(Value::Null);
    let records = dossiers.get("records").and_then(Value::as_object);
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
    let mut sources = BTreeSet::new();
    let units = json(tree, "games/gs1/recon/translation-units.json").unwrap_or(Value::Null);
    let mut registered = BTreeSet::new();
    for unit in array(&units, "units") {
        let source = text(unit, "source");
        registered.insert(source.clone());
        let id = text(unit, "overlay");
        if text(unit, "game") != "gs1"
            || !executable.contains_key(&id)
            || !source.starts_with(&format!("{directory}/"))
            || !source.ends_with(".c")
            || !tree.read(&source).is_some_and(|code| canonical(&code))
        {
            continue;
        }
        for owner in array(unit, "owners") {
            if text(owner, "state") != "retained-assembly" {
                continue;
            }
            let (Some(entry), Some(size)) = (address(owner, "address"), integer(owner, "extent"))
            else {
                continue;
            };
            let Some(end) = entry.checked_add(size).filter(|end| *end > entry) else {
                continue;
            };
            sources.insert(source.clone());
            spans
                .entry(id.clone())
                .or_default()
                .push(Span::new(entry, end));
        }
    }
    for name in tree.list(directory) {
        let source = format!("{directory}/{name}");
        if registered.contains(&source) {
            continue;
        }
        let Some(stem) = name.strip_suffix(".c") else {
            continue;
        };
        let Some((id, address)) = stem.rsplit_once("_c_") else {
            continue;
        };
        let Some(entry) = hex(address) else {
            continue;
        };
        if !tree.read(&source).is_some_and(|source| canonical(&source)) {
            continue;
        }
        let record_size = records
            .and_then(|records| records.get(&format!("{id}:{address}")))
            .and_then(|record| integer(record, "span_bytes"));
        let Some(size) = record_size.or_else(|| extents.get(&(id.into(), entry)).copied()) else {
            continue;
        };
        sources.insert(source);
        spans
            .entry(id.to_string())
            .or_insert_with(Vec::new)
            .push(Span::new(entry, entry + size));
    }
    for (id, found) in &mut spans {
        *found = intersect(&normalize(found), mapped(executable, id));
    }
    (spans, sources.len())
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
                owner.source = path;
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
/// Kinds whose retained bytes count as proven assembly: the register credits
/// them as library with proof beside the claim. A bare label credits
/// nothing, and handwritten credit awaits Pascal's ruling.
fn credited_kinds(classification: &Value) -> BTreeSet<String> {
    // `groups` is an array of kind entries; an object of entries reads the same.
    let groups: Vec<Value> = match &classification["groups"] {
        Value::Array(entries) => entries.clone(),
        Value::Object(map) => map.values().cloned().collect(),
        _ => Vec::new(),
    };
    array(classification, "structural")
        .iter()
        .chain(groups.iter())
        .filter(|entry| {
            let provenance = &entry["provenance"];
            text(provenance, "credit") == "library"
                && array(entry, "evidence")
                    .iter()
                    .any(|item| item.as_str().is_some_and(|s| !s.trim().is_empty()))
                && (!text(provenance, "proof").trim().is_empty()
                    || !text(provenance, "object").trim().is_empty())
        })
        .map(|entry| text(entry, "kind"))
        .collect()
}
/// Withdrawn, draft, and credited main assembly: credited spans are the
/// proven regions whose kind the register credits, and they leave the
/// withdrawn list so they are counted once.
fn main_assembly_classification(tree: &SourceTree) -> (Vec<Span>, Vec<Span>, Vec<Span>) {
    let mut proven = Vec::new();
    let mut draft = Vec::new();
    let mut credited = Vec::new();
    let credited_kinds = json(tree, "games/gs1/asm/classification.json")
        .map(|document| credited_kinds(&document))
        .unwrap_or_default();
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
                    if text(region, "confidence") != "proven" {
                        draft.push(span);
                    } else if credited_kinds.contains(&kind) {
                        credited.push(span);
                    } else {
                        proven.push(span);
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
    (normalize(&proven), normalize(&draft), normalize(&credited))
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
        if text(row, "confidence") == "proven" && text(row, "kind") == "structured_scene_module" {
            return Err(format!(
                "assembly classification {index} promotes a scene reconstruction without compiler-impossibility proof"
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
    sources: &[(Span, String)],
) -> Vec<Tile> {
    let mut cuts = boundaries.to_vec();
    cuts.extend(sources.iter().flat_map(|(span, _)| [span.start, span.end]));
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
            let mut tile = code_tile(
                format!("0x{:08x}–0x{:08x}", span.start, span.end),
                &[span],
                exact,
                semantic,
                draft_assembly,
                retained,
                Some(bank),
                Some(span.start),
            );
            tile.source = sources
                .iter()
                .find(|(owner, _)| owner.start <= span.start && owner.end >= span.end)
                .map(|(_, source)| source.clone());
            tile
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
        let mut tile = code_tile(
            format!("{short} · {} · C", owner.label),
            &span,
            &span,
            &[],
            &[],
            &[],
            Some(short.into()),
            Some(owner.entry),
        );
        tile.source = Some(owner.source.clone());
        out.push(tile);
    }
    let residuals = [
        (
            "Drafted",
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
            "Assembly",
            subtract(
                &intersect(executable, draft_assembly),
                &[exact.clone(), semantic.to_vec(), retained.to_vec()].concat(),
            ),
            &[] as &[Span],
        ),
        (
            "Assembly",
            subtract(&intersect(executable, retained), &exact),
            &[] as &[Span],
        ),
    ];
    for (kind, spans, tracked) in residuals {
        for span in spans {
            let mut tile = code_tile(
                format!("{short} · {kind} 0x{:08x}–0x{:08x}", span.start, span.end),
                &[span],
                &exact,
                tracked,
                draft_assembly,
                retained,
                Some(short.into()),
                Some(span.start),
            );
            tile.source = Some(format!("games/gs1/asm/overlays/{id}_overlay.s"));
            out.push(tile);
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
fn sound_sequence_classes(source: &str) -> BTreeMap<i64, String> {
    source
        .lines()
        .filter_map(|line| {
            let mut fields = line.split('\t');
            fields.next()?;
            let class = fields.next()?;
            if !matches!(class, "music" | "sfx") {
                return None;
            }
            let address = i64::from_str_radix(fields.next()?.strip_prefix("0x")?, 16).ok()?;
            Some((address, class.into()))
        })
        .collect()
}
fn asset_number(value: &Value, key: &str) -> Option<i64> {
    integer(value, key).or_else(|| {
        get(value, key)?
            .as_str()?
            .strip_prefix("0x")
            .and_then(|value| i64::from_str_radix(value, 16).ok())
    })
}
pub(crate) fn source_container(source: String, children: Vec<Tile>) -> Tile {
    let mut tile = Tile {
        label: source
            .trim_end_matches('/')
            .rsplit('/')
            .next()
            .unwrap_or(&source)
            .into(),
        group: children.first().and_then(|child| child.group.clone()),
        source: Some(source),
        children,
        ..Tile::default()
    };
    for child in &tile.children {
        tile.bytes += child.bytes;
        for (total, bytes) in tile.categories.iter_mut().zip(child.categories) {
            *total += bytes;
        }
    }
    tile
}
/// Per-package tiles of a static sprite series: the region's index document
/// declares the series layout and lists one component per package with its
/// address, size and sheet image.
fn sprite_children(tree: &SourceTree, source: &str, span: Span, data: &[Span]) -> Vec<Tile> {
    let Some(index) = json(tree, source) else {
        return Vec::new();
    };
    if text(&index, "layout") != "golden-sun-static-sprite-series" {
        return Vec::new();
    }
    let directory = Path::new(source).parent().unwrap_or(Path::new(""));
    let mut children = Vec::new();
    let mut covered = Vec::new();
    for package in array(&index, "components")
        .iter()
        .filter(|package| text(package, "kind") == "components")
    {
        let (Some(start), Some(size)) = (
            asset_number(package, "address"),
            asset_number(package, "size"),
        ) else {
            return Vec::new();
        };
        let Some(end) = start.checked_add(size) else {
            return Vec::new();
        };
        let package_span = Span::new(start, end);
        if size <= 0
            || start < span.start
            || end > span.end
            || bytes(&intersect(&[package_span], &covered)) != 0
        {
            return Vec::new();
        }
        covered.push(package_span);
        let size = bytes(&intersect(&[package_span], data));
        if size == 0 {
            continue;
        }
        let file = text(package, "image");
        if file.is_empty() {
            return Vec::new();
        }
        children.push(Tile {
            label: format!("{file} · 0x{start:08x}"),
            bytes: size,
            categories: [0, 0, 0, 0, 0, size],
            group: Some("golden-sun-static-sprite-series".into()),
            source: Some(directory.join(&file).to_string_lossy().into_owned()),
            address: Some(start),
            ..Tile::default()
        });
    }
    if children.is_empty() {
        return children;
    }
    for gap in subtract(&intersect(&[span], data), &covered) {
        children.push(Tile {
            label: format!("Container data · 0x{:08x}", gap.start),
            bytes: gap.bytes(),
            categories: [0, 0, 0, 0, 0, gap.bytes()],
            group: Some("asset-padding".into()),
            address: Some(gap.start),
            ..Tile::default()
        });
    }
    children
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
    let sequence_classes = sound_sequence_classes(
        &tree
            .read("games/gs1/assets/sound/sequences.tsv")
            .unwrap_or_default(),
    );
    let mut groups: BTreeMap<String, Vec<Tile>> = BTreeMap::new();
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
        let sources = array(region, "sources");
        let source = sources
            .iter()
            .filter_map(Value::as_str)
            .find(|source| kind == "golden-sun-pcm-wave" && source.ends_with(".wav"))
            .or_else(|| sources.first().and_then(Value::as_str))
            .unwrap_or(&kind);
        let owner = if kind == "golden-sun-sound-sequence" {
            "games/gs1/assets/sound/sequences.tsv"
        } else {
            sources.first().and_then(Value::as_str).unwrap_or(source)
        };
        let children = if kind == "components" {
            sprite_children(tree, owner, span, data)
        } else {
            Vec::new()
        };
        let group = match children.first().and_then(|child| child.group.clone()) {
            Some(group) => group,
            None => kind.clone(),
        };
        groups.entry(owner.into()).or_default().push(Tile {
            label: format!(
                "{} · {} · 0x{:08x}",
                source.rsplit('/').next().unwrap_or(source),
                kind,
                start
            ),
            bytes: actual,
            categories: [0, 0, 0, 0, 0, actual],
            group: Some(group),
            subgroup: sequence_classes.get(&start).cloned(),
            address: Some(start),
            source: Some(source.into()),
            children,
        });
    }
    if groups.is_empty() {
        return vec![Tile {
            label: format!("ROM data · {rom} bytes"),
            bytes: bytes(data),
            ..Tile::default()
        }];
    }
    groups
        .into_iter()
        .map(|(source, mut children)| {
            if children.len() == 1 {
                children.pop().unwrap()
            } else {
                source_container(source, children)
            }
        })
        .collect()
}
fn categories_json(values: &[i64; 6]) -> Value {
    Value::Object(
        CATEGORIES
            .iter()
            .filter(|(category, _, _)| values[*category as usize] != 0)
            .map(|(category, name, _)| (name.to_string(), values[*category as usize].into()))
            .collect::<Map<_, _>>(),
    )
}
fn area_json(area: &Area) -> Value {
    json!({
        "id": area.id,
        "label": area.label,
        "bytes": area.bytes,
        "categories": categories_json(&area.categories),
        "tiles": area.tiles.iter().map(tile_json).collect::<Vec<_>>()
    })
}
fn tile_json(tile: &Tile) -> Value {
    let mut out = json!({
        "label": tile.label,
        "bytes": tile.bytes,
        "categories": categories_json(&tile.categories)
    });
    if let Some(v) = &tile.group {
        out["group"] = v.clone().into();
    }
    if let Some(v) = &tile.subgroup {
        out["subgroup"] = v.clone().into();
    }
    if let Some(v) = tile.address {
        out["address"] = v.into();
    }
    if let Some(source) = &tile.source {
        out["source"] = source.clone().into();
    }
    if !tile.children.is_empty() {
        out["children"] = tile.children.iter().map(tile_json).collect();
    }
    out
}
fn entry(bytes: i64, total: i64) -> Value {
    json!({
        "bytes": bytes,
        "percent_of_executable": crate::coverage::jsnum::round_half_up(bytes, total)
    })
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
    // Retained assembly is credited only when proven handwritten or library.
    // The register credits library kinds with their proof; those proven
    // spans count as assembly. Everything else the old standard marked is
    // withdrawn: it returns to Unknown, and its total is published as
    // withdrawn_assembly_bytes, so no byte leaves the map silently. The
    // overlay register carries no credit field yet, so no overlay assembly is
    // credited.
    let (withdrawn_main, withdrawn_draft_main, retained_main) =
        main_assembly_classification(options.exact);
    let (withdrawn_overlay, withdrawn_draft_overlay) =
        overlay_assembly_classification(options.exact, &overlay_regions, &overlay_exec)?;
    let withdrawn_assembly = bytes(&withdrawn_main)
        + bytes(&withdrawn_draft_main)
        + mapped_bytes(&withdrawn_overlay)
        + mapped_bytes(&withdrawn_draft_overlay);
    let draft_main: Vec<Span> = Vec::new();
    let (retained_overlay, draft_overlay): (SpanMap, SpanMap) = (SpanMap::new(), SpanMap::new());
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
    let mut main_sources = Vec::new();
    for path in [
        format!("out/{}/full/claimed/manifest.json", options.target),
        format!("out/{}/full/asm/manifest.json", options.target),
    ] {
        if let Some(manifest) = json(options.exact, &path) {
            for region in array(&manifest, "regions") {
                if let (Some(start), Some(size), Some(source)) = (
                    integer(region, "address"),
                    integer(region, "size"),
                    get(region, "source").and_then(Value::as_str),
                ) {
                    main_sources.push((Span::new(start, start + size), source.into()));
                }
            }
        }
    }
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
            &main_sources,
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
    let document = json!({
        "format": 1,
        "kind": "golden-sun-rom-coverage-map",
        "target": options.target,
        "derivation": "tracked-evidence-v1",
        "rom_bytes": rom,
        "executable_bytes": executable,
        "categories": {
            "proven_c": entry(exact_bytes, executable),
            "draft_c": entry(semantic_bytes, executable),
            "draft_asm": entry(draft_assembly, executable),
            "unknown": entry(assembly, executable),
            "proven_asm": entry(retained, executable),
            "asset_data": { "bytes": bytes(&data), "percent_of_executable": 0 }
        },
        "main": {
            "executable_bytes": bytes(&main_exec),
            "proven_c_bytes": bytes(&exact_main),
            "draft_c_bytes": bytes(&semantic_main)
        },
        "overlays": {
            "executable_bytes": executable - bytes(&main_exec),
            "proven_c_bytes": exact_overlay_bytes,
            "draft_c_bytes": semantic_overlay_bytes
        },
        "provenance": {
            "proven_source": options.exact.id(),
            "draft_source": options.recon.map_or("absent", |tree| tree.id()),
            "draft_sources": (candidate_main_sources + candidate_overlay_sources) as i64,
            "main_draft_census": "games/gs1/recon/en/dossiers.json",
            "proven_assembly_standard": "library-proven; handwritten pending ruling",
            "credited_assembly_bytes": bytes(&retained_main),
            "withdrawn_assembly_bytes": withdrawn_assembly,
            "main_assembly_classification": "out/gs1-en/full/asm/manifest.json",
            "overlay_assembly_classification": "games/gs1/semantic/overlay-assembly.json",
            "draft_superseded_bytes": 0,
            "draft_outside_extent_bytes": 0,
            "draft_unresolved": []
        },
        "rom_areas": rom_areas.iter().map(area_json).collect::<Vec<_>>(),
        "executable_areas": executable_areas.iter().map(area_json).collect::<Vec<_>>()
    });
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
    #[test]
    fn only_library_credit_with_evidence_and_proof_is_credited() {
        let entry = |kind: &str, credit: &str, evidence: Value, proof: &str| json!({"kind": kind, "evidence": evidence, "provenance": {"credit": credit, "proof": proof}});
        let document = json!({
            "structural": [entry("thunks", "library", json!(["lib1funcs_asm_950_990"]), "byte identical")],
            "groups": [
                entry("bare_label", "library", json!([]), "byte identical"),
                entry("no_proof", "library", json!(["tag"]), ""),
                entry("pending", "library_pending_identification", json!(["tag"]), "x"),
                entry("hand", "handwritten", json!(["tag"]), "x"),
                entry("grouped", "library", json!(["tag"]), "x")
            ]
        });
        let credited = credited_kinds(&document);
        assert_eq!(
            credited.into_iter().collect::<Vec<_>>(),
            ["grouped", "thunks"]
        );
        // The live register credits the libgcc call_via thunks, and the
        // pipeline counts exactly that region from the built manifest.
        let tree = crate::coverage::tree::work_tree();
        let live = json(&tree, "games/gs1/asm/classification.json").unwrap();
        assert!(credited_kinds(&live).contains("runtime_thunk_bundle"));
        let (_, _, credited) = main_assembly_classification(&tree);
        // Exactly the thunk bundle at 0x080072e4 (56 bytes until the register's
        // extent correction lands, 60 after); nothing else is credited.
        assert_eq!(credited.len(), 1, "credited spans: {credited:?}");
        assert_eq!(credited[0].start, 0x0800_72e4);
        assert!(matches!(bytes(&credited), 56 | 60));
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
    fn overlay_drafts_follow_units_with_legacy_filename_fallback() {
        let root = tempfile::tempdir().unwrap();
        let directory = "games/gs1/recon/en/overlays";
        let write = |path: &str, source: &str| {
            let path = root.path().join(path);
            std::fs::create_dir_all(path.parent().unwrap()).unwrap();
            std::fs::write(path, source).unwrap();
        };
        for name in [
            "actor_sequence",
            "unregistered",
            "resource_test_c_02000160",
            "resource_test_c_02000180",
        ] {
            write(
                &format!("{directory}/{name}.c"),
                "void Actor_Run(void) {}\n",
            );
        }
        write(&format!("{directory}/uncanonical.c"), "M2C_ERROR\n");
        let owner =
            |entry, extent, state| json!({"address": entry, "extent": extent, "state": state});
        let unit = |name, owners| {
            json!({
                "game": "gs1", "overlay": "resource_test",
                "source": format!("{directory}/{name}.c"), "owners": owners
            })
        };
        let named = unit(
            "actor_sequence",
            json!([
                owner("0x02000100", 0x20, "retained-assembly"),
                owner("0x02000120", 0x20, "retained-assembly"),
                owner("0x02000140", 0x20, "exact-c")
            ]),
        );
        write("games/gs1/recon/translation-units.json", &json!({"units": [
            named.clone(), named,
            unit("resource_test_c_02000160", json!([owner("0x02000160", 0x10, "retained-assembly")])),
            unit("resource_test_c_02000180", json!([owner("0x02000180", 0x10, "exact-c")])),
            unit("missing", json!([owner("0x02000190", 0x10, "retained-assembly")])),
            unit("uncanonical", json!([owner("0x020001a0", 0x10, "retained-assembly")]))
        ]}).to_string());
        write("games/gs1/recon/en/dossiers.json", &json!({"records": {
            "resource_test:02000100": {"span_bytes": 0x20},
            "resource_test:02000160": {"span_bytes": 0x20},
            "resource_test:02000180": {"span_bytes": 0x10},
            "resource_test:020001c0": {"span_bytes": 0x10},
            "resource_test:020001e0": {"span_bytes": 0x10, "source": format!("{directory}/unregistered.c")}
        }}).to_string());
        let tree = crate::coverage::tree::work_tree_at(root.path().to_path_buf());
        let expected = vec![
            Span::new(0x02000100, 0x02000140),
            Span::new(0x02000160, 0x02000170),
        ];
        assert_eq!(
            candidate_overlay(&tree, &executable()),
            (
                BTreeMap::from([("resource_test".into(), expected.clone())]),
                2
            )
        );
        write(
            &format!("{directory}/resource_test_c_020001c0.c"),
            "void Legacy_Run(void) {}\n",
        );
        let (found, sources) = candidate_overlay(&tree, &executable());
        assert_eq!(sources, 3);
        assert_eq!(
            found["resource_test"],
            [expected, vec![Span::new(0x020001c0, 0x020001d0)]].concat()
        );
        let tile = code_tile(
            String::new(),
            &executable()["resource_test"],
            &[Span::new(0x02000100, 0x02000110)],
            &found["resource_test"],
            &[],
            &[],
            None,
            None,
        );
        assert_eq!(tile.categories[Category::ProvenC as usize], 0x10);
        assert_eq!(tile.categories[Category::DraftC as usize], 0x50);
    }
    #[test]
    fn sprite_series_exposes_packages_without_double_counting() {
        let tree = crate::coverage::tree::work_tree();
        let span = Span::new(0x081a7020, 0x081e120c);
        let children = sprite_children(
            &tree,
            "games/gs1/assets/graphics/characters_chr_081a_index.json",
            span,
            &[span],
        );
        assert_eq!(children.len(), 22);
        assert_eq!(children.iter().map(|tile| tile.bytes).sum::<i64>(), 238060);
        assert_eq!(children[0].address, Some(span.start));
        let parent = source_container("sprites.json".into(), children);
        assert_eq!(parent.bytes, span.bytes());
        assert_eq!(
            parent.categories[Category::AssetData as usize],
            span.bytes()
        );
        assert_eq!(parent.address, None);
        assert_eq!(tile_json(&parent)["children"].as_array().unwrap().len(), 22);
        assert!(sprite_children(
            &tree,
            "games/gs1/assets/graphics/characters_chr_081a_index.json",
            Span::new(span.start, span.end - 1),
            &[span]
        )
        .is_empty());
    }
    #[test]
    fn sound_sequence_roles_follow_metadata_not_filenames() {
        let classes = sound_sequence_classes("sound_id\tclass\taddress\tsize\tsource\n0\tmusic\t0x0815fb78\t1\tse_000.mid\n197\tsfx\t0x08182830\t1\tbgm_197.mid\n1\tunknown\t0x08182834\n2\tmusic\tnot-an-address\n");
        assert_eq!(
            classes,
            BTreeMap::from([(0x0815fb78, "music".into()), (0x08182830, "sfx".into())])
        );
        assert!(sound_sequence_classes("").is_empty());
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
    #[test]
    fn scene_reconstruction_cannot_claim_proven_assembly() {
        let mut row = region(
            "0x02000120",
            "0x02000140",
            "proven",
            json!(["complete reconstructed scene body"]),
        );
        row["kind"] = json!("structured_scene_module");
        let error = overlay_assembly_classification_document(
            &classification(json!([row])),
            &no_inventory(),
            &executable(),
        )
        .unwrap_err();
        assert!(error.contains("without compiler-impossibility proof"));
    }
}
