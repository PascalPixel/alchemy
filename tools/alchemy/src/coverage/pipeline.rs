use super::model::{
    area, bytes, intersect, normalize, subtract, Area, Category, Span, Tile, CATEGORIES,
    UNIDENTIFIED,
};
#[cfg(test)]
use crate::compiler::source_paths::SOURCE_PATHS_MANIFEST;
use crate::compiler::source_paths::{SourceOwner, SourcePaths};
use crate::coverage::tree::{read_json, SourceTree, ROM_BASE};
use crate::targets::DecompTarget;
use serde_json::{json, Map, Value};
use std::{
    collections::{BTreeMap, BTreeSet},
    path::Path,
};
type SpanMap = BTreeMap<String, Vec<Span>>;
type OwnerMap = BTreeMap<String, Vec<Owner>>;
const OVERLAY_BASE: i64 = 0x0200_0000;

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
pub fn rom_size(target: &str) -> Result<i64, String> {
    match target {
        "tbs-en" => Ok(0x800000),
        "tla-en" => Ok(0x1000000),
        other => Err(format!(
            "unsupported decomp target {other:?}; expected tbs-en or tla-en"
        )),
    }
}
fn text(value: &Value, key: &str) -> String {
    value.get(key).and_then(Value::as_str).unwrap_or("").into()
}
fn integer(value: &Value, key: &str) -> Option<i64> {
    let value = value.get(key)?;
    value.as_i64().or_else(|| {
        value
            .as_f64()
            .filter(|n| n.fract() == 0.0)
            .map(|n| n as i64)
    })
}
fn array<'a>(value: &'a Value, key: &str) -> &'a [Value] {
    match value.get(key) {
        Some(Value::Array(values)) => values,
        _ => &[],
    }
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
    integer(value, key).or_else(|| value.get(key).and_then(Value::as_str).and_then(hex))
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
    let stem = name.strip_prefix("resource_")?;
    let stem = stem
        .strip_suffix("_overlay.s")
        .or_else(|| stem.strip_suffix("_overlay.S"))?;
    Some(format!("resource_{stem}"))
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
fn overlay_owners_for(tree: &SourceTree, directory: &str, name: &str) -> Vec<Owner> {
    let Some(source) = tree.read(&format!("{directory}/{name}")) else {
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
#[cfg(test)]
pub fn overlay_ids(tree: &SourceTree) -> Vec<(String, String)> {
    overlay_ids_for(tree, "games/THE BROKEN SEAL/raw/overlays")
}
fn overlay_ids_for(tree: &SourceTree, directory: &str) -> Vec<(String, String)> {
    let mut names: Vec<_> = tree
        .list(directory)
        .into_iter()
        .filter_map(|name| overlay_name(&name).map(|id| (id, name)))
        .collect();
    names.sort_by(|a, b| a.0.cmp(&b.0));
    names
}
fn exact_main(tree: &SourceTree, target: &str, executable: &[Span]) -> Result<Vec<Span>, String> {
    if target == "tla-en" {
        let proof = read_json(tree, &format!("out/{target}/reports/verified-code.json"))?;
        let spans = array(&proof, "credits")
            .iter()
            .filter(|row| text(row, "image") == "main" && text(row, "kind") == "c")
            .map(|row| {
                Ok(Span::new(
                    integer(row, "start").ok_or("credit start missing")?,
                    integer(row, "end").ok_or("credit end missing")?,
                ))
            })
            .collect::<Result<Vec<_>, String>>()?;
        return exact_spans(spans, executable, "main");
    }
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
        .any(|span| bytes(&intersect(&[*span], executable)) != span.bytes())
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
    let mut spans = value["intervals"]
        .as_array()
        .ok_or_else(|| format!("{id} has no executable intervals"))?
        .iter()
        .map(|row| {
            let start = integer(row, "start").ok_or("executable interval start is invalid")?;
            let end = integer(row, "end").ok_or("executable interval end is invalid")?;
            if start < 0 || end <= start {
                return Err("invalid executable interval".to_string());
            }
            Ok(Span::new(start, end))
        })
        .collect::<Result<Vec<_>, String>>()?;
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
        if decoded <= 0 || decoded > 0x40000 {
            return Err(format!("{id} invalid decoded image size"));
        }
        let image = Span::new(OVERLAY_BASE, OVERLAY_BASE + decoded);
        if spans
            .iter()
            .any(|span| span.start < image.start || span.end > image.end)
        {
            return Err(format!(
                "{id} executable interval lies outside its decoded image"
            ));
        }
        if integer(value, "excluded_bytes") != Some(decoded - expected) {
            return Err(format!("{id} decoded byte classification is incomplete"));
        }
    }
    Ok(spans)
}

pub(super) fn validated_inventory(
    inventory: &Value,
    target: &str,
) -> Result<(Vec<Span>, SpanMap), String> {
    if integer(inventory, "format") != Some(1)
        || text(inventory, "metric") != "full-c-byte-share"
        || text(inventory, "target") != target
    {
        return Err("unsupported executable inventory format or target".into());
    }
    let state = text(inventory, "state");
    if !matches!(state.as_str(), "pending" | "audited" | "verified") {
        return Err("executable inventory has invalid state".into());
    }
    let audit = text(inventory, "audit");
    if (state == "pending" && audit != "incomplete") || (state != "pending" && audit != "complete")
    {
        return Err("executable inventory state and audit disagree".into());
    }
    let main_node = inventory
        .get("main")
        .ok_or("executable inventory has no main")?;
    if state == "pending" || text(main_node, "audit") != "complete" {
        return Err(format!(
            "Full-C Byte Share withheld: {target} executable audit is incomplete"
        ));
    }
    let main = validated_executable(main_node)?;
    let mut overlays = SpanMap::new();
    let overlay_nodes = array(inventory, "overlays");
    let overlay_count =
        integer(inventory, "overlay_count").ok_or("executable inventory has no overlay_count")?;
    if overlay_count != overlay_nodes.len() as i64 {
        return Err(format!(
            "executable inventory overlay count is stale: {overlay_count} != {}",
            overlay_nodes.len()
        ));
    }
    for node in overlay_nodes {
        let id = text(node, "id");
        if id.is_empty() || overlays.contains_key(&id) {
            return Err("executable inventory has an empty or duplicate overlay id".into());
        }
        if text(node, "audit") != "complete" {
            return Err(format!("Full-C Byte Share withheld: {id} is incomplete"));
        }
        overlays.insert(id, validated_executable(node)?);
    }
    let executable = bytes(&main) + mapped_bytes(&overlays);
    if integer(inventory, "total_union_bytes") != Some(executable) {
        return Err("executable inventory total is stale".into());
    }
    Ok((main, overlays))
}

fn candidate_main(
    tree: &SourceTree,
    target: &DecompTarget,
    executable: &[Span],
) -> (Vec<Span>, usize) {
    let directory = format!("{}/recon/en/main", target.game_dir());
    let dossiers = json(
        tree,
        &format!("{}/recon/en/dossiers.json", target.game_dir()),
    )
    .unwrap_or(Value::Null);
    let records = dossiers.get("records").and_then(Value::as_object);
    let mut spans = Vec::new();
    let mut sources = 0;
    for name in tree.list(&directory) {
        let Some(stem) = name.strip_suffix(".c").or_else(|| name.strip_suffix(".C")) else {
            continue;
        };
        if !tree
            .read(&format!("{directory}/{name}"))
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
fn candidate_overlay(
    tree: &SourceTree,
    target: &DecompTarget,
    executable: &SpanMap,
) -> (SpanMap, usize) {
    let directory = format!("{}/recon/en/overlays", target.game_dir());
    let dossiers = json(
        tree,
        &format!("{}/recon/en/dossiers.json", target.game_dir()),
    )
    .unwrap_or(Value::Null);
    let records = dossiers.get("records").and_then(Value::as_object);
    let reviewed = json(
        tree,
        &format!("{}/semantic/regions.json", target.game_dir()),
    )
    .unwrap_or(Value::Null);
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
    let units = json(
        tree,
        &format!("{}/recon/translation-units.json", target.game_dir()),
    )
    .unwrap_or(Value::Null);
    let mut registered = BTreeSet::new();
    for unit in array(&units, "units") {
        let source = text(unit, "source");
        registered.insert(source.clone());
        let id = text(unit, "overlay");
        if text(unit, "game") != target.compiler.as_str()
            || !executable.contains_key(&id)
            || !source.starts_with(&format!("{directory}/"))
            || !(source.ends_with(".c") || source.ends_with(".C"))
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
    for name in tree.list(&directory) {
        let source = format!("{directory}/{name}");
        if registered.contains(&source) {
            continue;
        }
        let Some(stem) = name.strip_suffix(".c").or_else(|| name.strip_suffix(".C")) else {
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
#[cfg(test)]
fn exact_overlay(
    tree: &SourceTree,
    pairs: &[(String, String)],
    executable: &SpanMap,
) -> Result<(OwnerMap, SpanMap), String> {
    let target = crate::targets::target_for(crate::targets::DEFAULT_TARGET);
    exact_overlay_for(tree, &target, pairs, executable)
}
fn exact_overlay_for(
    tree: &SourceTree,
    target: &DecompTarget,
    pairs: &[(String, String)],
    executable: &SpanMap,
) -> Result<(OwnerMap, SpanMap), String> {
    let manifest_path = format!("{}/source-paths.json", target.game_dir());
    let manifest = tree
        .read(&manifest_path)
        .ok_or_else(|| format!("missing canonical owner register {manifest_path}"))?;
    let source_paths =
        SourcePaths::parse_for_game(Path::new(""), target.compiler.as_str(), &manifest)?;
    let overlay_dir = target.overlay_dir();
    let mut owners = OwnerMap::new();
    let mut spans = SpanMap::new();
    for (id, name) in pairs {
        let mut list = overlay_owners_for(tree, &overlay_dir, name)
            .into_iter()
            .map(|mut owner| -> Result<Owner, String> {
                let source_owner = SourceOwner::parse(&format!("{id}:{:08x}", owner.entry))?;
                owner.label = source_paths
                    .registered_name(source_owner)
                    .ok_or_else(|| {
                        format!(
                            "exact owner {} has no name in {manifest_path}",
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
                } else {
                    owner.spans =
                        exact_spans(owner.spans, mapped(executable, id), &source_owner.id())?;
                }
                owner.source = path;
                Ok(owner)
            })
            .collect::<Result<Vec<_>, _>>()?
            .into_iter()
            .filter(|owner| !owner.spans.is_empty())
            .collect::<Vec<_>>();
        let units_path = format!("{}/recon/translation-units.json", target.game_dir());
        if let Some(units) = json(tree, &units_path) {
            // Each placeholder already credits its owner once, in its own
            // overlay; an instance adds only the fill it declares there.
            let linked = array(&units, "units")
                .iter()
                .filter(|unit| text(unit, "game") == target.compiler.as_str())
                .filter_map(|unit| match text(unit, "overlay") == *id {
                    true => Some((unit, unit)),
                    false => unit
                        .get("instances")?
                        .get(id)
                        .map(|instance| (unit, instance)),
                })
                .collect::<Vec<_>>();
            for (unit, layout) in linked {
                for gap in array(layout, "compiler_gaps") {
                    let (Some(start), Some(end)) = (address(gap, "start"), address(gap, "end"))
                    else {
                        return Err("compiler alignment gap has invalid bounds".into());
                    };
                    let source = text(unit, "source");
                    let preceding = list.iter().any(|owner| {
                        owner.source == source && owner.spans.iter().any(|span| span.end == start)
                    });
                    let following = list.iter().any(|owner| owner.entry == end);
                    let span = Span::new(start, end);
                    if end - start != 2
                        || end & 3 != 0
                        || !preceding
                        || !following
                        || bytes(&intersect(&[span], mapped(executable, id))) != 2
                    {
                        return Err(format!("{id}: compiler gap lacks exact adjacent owners"));
                    }
                    list.push(Owner {
                        label: format!("{} compiler alignment", text(unit, "id")),
                        source,
                        entry: start,
                        spans: vec![span],
                    });
                }
            }
        }
        let flat: Vec<_> = list.iter().flat_map(|o| o.spans.iter().copied()).collect();
        owners.insert(id.clone(), list);
        spans.insert(id.clone(), normalize(&flat));
    }
    Ok((owners, spans))
}
/// Compiler runtime links built from the licensed container count as proven,
/// as pret counts linked libgcc. Main links are credited only as the built
/// asm manifest placed them; overlay links only where the tracked listing
/// reserves their window. Data windows lie outside executable intervals.
#[cfg(test)]
fn runtime_credit(
    tree: &SourceTree,
    main_exec: &[Span],
    overlay_exec: &SpanMap,
) -> Result<(Vec<Span>, SpanMap), String> {
    let target = crate::targets::target_for(crate::targets::DEFAULT_TARGET);
    runtime_credit_for(tree, &target, main_exec, overlay_exec)
}
fn runtime_credit_for(
    tree: &SourceTree,
    target: &DecompTarget,
    main_exec: &[Span],
    overlay_exec: &SpanMap,
) -> Result<(Vec<Span>, SpanMap), String> {
    // The runtime registry currently describes the TBS linked container.
    // A TLA registry must be independently evidenced before it earns credit.
    if target.compiler != crate::compiler::routing::CompilerTarget::Tbs {
        return Ok((Vec::new(), SpanMap::new()));
    }
    let Some(registry) = json(tree, crate::compiler::runtime::REGISTRY) else {
        return Ok((Vec::new(), SpanMap::new()));
    };
    let placed = json(
        tree,
        &format!("{}/full/asm/manifest.json", target.output_dir),
    )
    .map(|manifest| {
        array(&manifest, "regions")
            .iter()
            .filter(|region| {
                text(region, "source") == crate::compiler::runtime::REGISTRY
                    && text(region, "retention") == "container_runtime"
            })
            .filter_map(|region| Some((integer(region, "address")?, integer(region, "size")?)))
            .collect::<BTreeMap<_, _>>()
    })
    .unwrap_or_default();
    let mut main = Vec::new();
    let mut overlays = SpanMap::new();
    for link in array(&registry, "links") {
        let image = text(link, "image");
        let start = address(link, "text").ok_or_else(|| {
            format!(
                "{}: invalid runtime link",
                crate::compiler::runtime::REGISTRY
            )
        })?;
        if image == "main" {
            if let Some(size) = placed.get(&start) {
                let span = Span::new(start, start + size);
                if bytes(&intersect(&[span], main_exec)) != *size {
                    return Err(format!(
                        "main compiler runtime at 0x{start:08x} lies outside audited executable intervals"
                    ));
                }
                main.push(span);
            }
            continue;
        }
        let listing = tree
            .read(&target.overlay_assembly(&image))
            .unwrap_or_default();
        for (window, size) in crate::compiler::runtime::listing_windows(&listing)? {
            if i64::from(window) == start {
                let span = Span::new(start, start + size as i64);
                if bytes(&intersect(&[span], mapped(overlay_exec, &image))) != size as i64 {
                    return Err(format!(
                        "{image} compiler runtime at 0x{start:08x} lies outside audited executable intervals"
                    ));
                }
                overlays.entry(image.clone()).or_default().push(span);
            }
        }
    }
    for spans in overlays.values_mut() {
        *spans = normalize(spans);
    }
    Ok((intersect(&normalize(&main), main_exec), overlays))
}
#[cfg(test)]
const OVERLAY_VENEER_MACRO: &str = "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC";

/// Original assembly credit requires evidence and a proof or object.
fn assembly_credit(entry: &Value) -> bool {
    let provenance = &entry["provenance"];
    matches!(
        text(provenance, "credit").as_str(),
        "library" | "handwritten"
    ) && array(entry, "evidence")
        .iter()
        .any(|item| item.as_str().is_some_and(|s| !s.trim().is_empty()))
        && (!text(provenance, "proof").trim().is_empty()
            || !text(provenance, "object").trim().is_empty())
}
/// Main assembly can only be credited by its own range provenance emitted by
/// the assembler, never by its classification kind.
fn manifest_assembly_credit(region: &Value) -> bool {
    let provenance = &region["provenance"];
    matches!(
        text(provenance, "credit").as_str(),
        "library" | "handwritten"
    ) && !text(region, "source").trim().is_empty()
        && array(provenance, "evidence")
            .iter()
            .any(|item| item.as_str().is_some_and(|item| !item.trim().is_empty()))
        && (!text(provenance, "proof").trim().is_empty()
            || !text(provenance, "object").trim().is_empty())
}
/// Withdrawn, draft, and credited main assembly. Credited spans are proven
/// manifest rows with their own validated range provenance.
#[cfg(test)]
fn main_assembly_classification(tree: &SourceTree) -> (Vec<Span>, Vec<Span>, Vec<Span>) {
    let target = crate::targets::target_for(crate::targets::DEFAULT_TARGET);
    main_assembly_classification_for(tree, &target)
}
fn main_assembly_classification_for(
    tree: &SourceTree,
    target: &DecompTarget,
) -> (Vec<Span>, Vec<Span>, Vec<Span>) {
    let mut proven = Vec::new();
    let mut draft = Vec::new();
    let mut credited = Vec::new();
    if let Some(value) = json(
        tree,
        &format!("{}/full/asm/manifest.json", target.output_dir),
    ) {
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
                    } else if manifest_assembly_credit(region) {
                        credited.push(span);
                    } else {
                        proven.push(span);
                    }
                }
            }
        }
    }
    if let Some(value) = json(
        tree,
        &format!("{}/semantic/main-regions.json", target.game_dir()),
    ) {
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
fn overlay_assembly_classification_for(
    tree: &SourceTree,
    target: &DecompTarget,
    inventory: &BTreeMap<String, Vec<Region>>,
    executable: &SpanMap,
) -> Result<(SpanMap, SpanMap, SpanMap), String> {
    let classification_path = format!("{}/semantic/overlay-assembly.json", target.game_dir());
    let veneer_macro = target.overlay_macro();
    let source = tree
        .read(&classification_path)
        .ok_or_else(|| "overlay assembly classification is missing".to_string())?;
    let document: Value =
        serde_json::from_str(&source).map_err(|error| format!("{classification_path}: {error}"))?;
    for row in array(&document, "regions")
        .iter()
        .filter(|row| text(&row["provenance"], "credit") == "reconstructed_veneer")
    {
        for source in [veneer_macro.clone(), text(&row["provenance"], "source")] {
            if tree.read(&source).is_none() {
                return Err(format!("reconstructed veneer source is missing: {source}"));
            }
        }
    }
    overlay_assembly_classification_document_for(
        &document,
        inventory,
        executable,
        &veneer_macro,
        target.source_dir,
    )
}
#[cfg(test)]
fn overlay_assembly_classification_document(
    document: &Value,
    inventory: &BTreeMap<String, Vec<Region>>,
    executable: &SpanMap,
) -> Result<(SpanMap, SpanMap, SpanMap), String> {
    overlay_assembly_classification_document_for(
        document,
        inventory,
        executable,
        OVERLAY_VENEER_MACRO,
        "games/THE BROKEN SEAL/SRC",
    )
}
fn overlay_assembly_classification_document_for(
    document: &Value,
    inventory: &BTreeMap<String, Vec<Region>>,
    executable: &SpanMap,
    veneer_macro: &str,
    source_dir: &str,
) -> Result<(SpanMap, SpanMap, SpanMap), String> {
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
    let mut credited = SpanMap::new();
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
        if text(&row["provenance"], "credit") == "reconstructed_veneer" {
            let veneer_spans = mapped(inventory, &overlay)
                .iter()
                .filter(|region| region.kind == "veneer" && !region.evidence.trim().is_empty())
                .map(|region| region.span)
                .collect::<Vec<_>>();
            if text(row, "confidence") != "proven"
                || text(row, "kind") != "veneer"
                || text(&row["provenance"], "proof") != veneer_macro
                || !text(&row["provenance"], "source").starts_with(&format!("{source_dir}/"))
                || span.start % 4 != 0
                || span.bytes() % 8 != 0
                || bytes(&intersect(&[span], &veneer_spans)) != span.bytes()
            {
                return Err(format!(
                    "assembly classification {index} has invalid reconstructed veneer credit"
                ));
            }
            credited.entry(overlay).or_default().push(span);
        } else if text(row, "confidence") == "proven" && assembly_credit(row) {
            credited.entry(overlay).or_default().push(span);
        } else if text(row, "confidence") == "proven" {
            proven.entry(overlay).or_default().push(span);
        } else {
            draft.entry(overlay).or_default().push(span);
        }
    }
    for spans in credited.values_mut() {
        *spans = normalize(spans);
    }
    for (overlay, spans) in &mut proven {
        *spans = subtract(spans, mapped(&credited, overlay));
    }
    for spans in draft.values_mut() {
        *spans = normalize(spans);
    }
    Ok((proven, draft, credited))
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
    assembly_source: &str,
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
            UNIDENTIFIED,
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
            tile.source = Some(format!("{assembly_source}/{id}_overlay.s"));
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
    source: Option<String>,
}
fn atlas_source(game_dir: &str, locations: &str, id: &str) -> Option<String> {
    let resource_id = format!("resource_{id}");
    locations.lines().find_map(|line| {
        let fields: Vec<_> = line.split('\t').collect();
        (fields.first().copied() == Some(resource_id.as_str()))
            .then(|| fields.get(6).copied())
            .flatten()
            .filter(|path| !path.is_empty())
            .map(|path| format!("{game_dir}/{path}/"))
    })
}
fn streams(tree: &SourceTree, target: &DecompTarget) -> Vec<Stream> {
    let Some(manifest) = json(tree, target.asset_manifest) else {
        return Vec::new();
    };
    let mut out = Vec::new();
    let locations = tree
        .read(&format!("{}/locations.tsv", target.game_dir()))
        .unwrap_or_default();
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
                let source = atlas_source(target.game_dir(), &locations, &id);
                out.push(Stream {
                    id: format!("resource_{id}"),
                    start,
                    rom,
                    source,
                });
            }
        }
    }
    out
}
fn shared_map_assets(tree: &SourceTree, areas: &[Area]) -> Result<Value, String> {
    let read = |path| json(tree, path).ok_or_else(|| format!("missing Atlas input: {path}"));
    let scenes = read("games/THE BROKEN SEAL/SRC/FIELD/COMMON/SCENE_TABLE.JSON")?;
    let maps = read("games/THE BROKEN SEAL/SRC/FIELD/COMMON/LOAD_TABLE.JSON")?;
    let directory = read("games/THE BROKEN SEAL/SRC/SYSTEM/RESOURCE/DIRECTORY.JSON")?;
    let locations = tree
        .read("games/THE BROKEN SEAL/locations.tsv")
        .ok_or("missing Atlas locations")?;
    let scenes = array(&scenes, "segments")
        .iter()
        .find(|row| text(row, "address") == "0x0809f1a8")
        .ok_or("missing Atlas scene table")?;
    let mut users: BTreeMap<i64, BTreeSet<String>> = BTreeMap::new();
    // The loader indexes 201 scene records into the map loading table.
    for scene in array(scenes, "records").iter().take(201) {
        let resource = integer(scene, "resource_id").ok_or("invalid scene resource")?;
        let Some(area) = atlas_source(
            "games/THE BROKEN SEAL",
            &locations,
            &format!("{resource:x}"),
        ) else {
            continue;
        };
        let map = array(&maps, "records")
            .iter()
            .find(|row| integer(row, "map_index") == integer(scene, "map_index"))
            .ok_or("scene has no map loading record")?;
        for field in array(&maps, "fields") {
            let index = field
                .as_str()
                .and_then(|field| map.get(field))
                .and_then(Value::as_str)
                .and_then(hex)
                .ok_or("invalid map resource")?;
            let address = array(&directory, "slots")
                .get(index as usize)
                .and_then(Value::as_str)
                .and_then(hex)
                .ok_or("unresolved map resource pointer")?;
            users.entry(address).or_default().insert(area.clone());
        }
    }
    let mut links: BTreeMap<String, BTreeSet<String>> = BTreeMap::new();
    fn visit(
        tiles: &[Tile],
        users: &BTreeMap<i64, BTreeSet<String>>,
        links: &mut BTreeMap<String, BTreeSet<String>>,
    ) {
        for tile in tiles {
            if let (Some(address), Some(source)) = (tile.address, &tile.source) {
                for area in users.get(&address).into_iter().flatten() {
                    if !source.starts_with(area) {
                        links
                            .entry(area.clone())
                            .or_default()
                            .insert(source.clone());
                    }
                }
            }
            visit(&tile.children, users, links);
        }
    }
    for area in areas.iter().filter(|area| area.id == "rom-data") {
        visit(&area.tiles, &users, &mut links);
    }
    Ok(json!(links))
}
fn scaled_bytes(rom: i64, decoded: i64, spans: &[Span]) -> i64 {
    if decoded == 0 {
        0
    } else {
        (rom as f64 * bytes(spans) as f64 / decoded as f64).round() as i64
    }
}

fn validate_streams(streams: &[Stream], rom_span: Span, main_exec: &[Span]) -> Result<(), String> {
    let mut seen = Vec::new();
    for stream in streams {
        let end = stream
            .start
            .checked_add(stream.rom)
            .ok_or_else(|| format!("compressed stream {} overflows", stream.id))?;
        let span = Span::new(stream.start, end);
        if span.start < rom_span.start || span.end > rom_span.end {
            return Err(format!("compressed stream {} lies outside ROM", stream.id));
        }
        if bytes(&intersect(&[span], main_exec)) != 0 {
            return Err(format!(
                "compressed stream {} overlaps main executable",
                stream.id
            ));
        }
        if bytes(&intersect(&[span], &seen)) != 0 {
            return Err(format!(
                "compressed stream {} overlaps another stream",
                stream.id
            ));
        }
        seen.push(span);
    }
    Ok(())
}

fn stream_categories(
    rom: i64,
    decoded: i64,
    exact: &[Span],
    semantic: &[Span],
    draft: &[Span],
    retained: &[Span],
) -> [i64; 5] {
    let raw = [
        scaled_bytes(rom, decoded, exact),
        scaled_bytes(rom, decoded, semantic),
        scaled_bytes(rom, decoded, draft),
        scaled_bytes(rom, decoded, retained),
    ];
    let mut remaining = rom.max(0);
    let mut out = [0; 5];
    for (index, value) in [0usize, 1, 3, 4].into_iter().zip(raw) {
        let value = value.clamp(0, remaining);
        out[index] = value;
        remaining -= value;
    }
    out[2] = remaining;
    out
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
        value
            .get(key)?
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
    let Some(mut index) = json(tree, source) else {
        return Vec::new();
    };
    if let Some(regions) = index["regions"].as_object() {
        index = regions
            .values()
            .find(|region| asset_number(region, "address") == Some(span.start))
            .cloned()
            .unwrap_or(Value::Null);
    }
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
            source: Some(source.into()),
            address: Some(gap.start),
            ..Tile::default()
        });
    }
    children
}
fn component_children(region: &Value, data: &[Span]) -> Vec<Tile> {
    let mut cursor = integer(region, "address").unwrap_or(0);
    let Some(end) = cursor.checked_add(integer(region, "size").unwrap_or(0)) else {
        return vec![];
    };
    let mut children = Vec::new();
    for part in array(&region["details"], "components") {
        let Some(size) = integer(part, "size").filter(|size| *size > 0) else {
            return vec![];
        };
        if asset_number(part, "address") != Some(cursor) || size > end - cursor {
            return vec![];
        }
        let bytes = bytes(&intersect(&[Span::new(cursor, cursor + size)], data));
        let source = array(part, "sources")
            .first()
            .or_else(|| array(region, "sources").first());
        if bytes > 0 {
            children.push(Tile {
                label: text(part, "kind"),
                bytes,
                categories: [0, 0, 0, 0, 0, bytes],
                group: Some(text(part, "kind")),
                source: source.and_then(Value::as_str).map(String::from),
                address: Some(cursor),
                ..Tile::default()
            });
        }
        cursor += size;
    }
    if cursor == end {
        children
    } else {
        vec![]
    }
}
/// The Lost Age ROM by source. The executable audit separates code that still
/// lives as assembly from cartridge data; only independently exact intervals
/// earn C credit. Empty until its asset manifest has been built.
fn lost_age_tiles(tree: &SourceTree, credits: &[super::proof::Credit]) -> Vec<Tile> {
    let Some(manifest) = json(tree, "out/tla-en/assets/manifest.json") else {
        return Vec::new();
    };
    let rom = rom_size("tla-en").unwrap_or(0x1000000);
    let mut tiles = Vec::new();
    let mut covered: Vec<Span> = Vec::new();
    let claim = |start: i64, end: i64, covered: &mut Vec<Span>| {
        let spans = subtract(&[Span::new(start, end)], covered);
        covered.extend_from_slice(&spans);
        *covered = normalize(covered);
        bytes(&spans)
    };
    let inventory = json(tree, "games/THE LOST AGE/metrics/executable.json").unwrap_or(Value::Null);
    let executable: Vec<Span> = regions(&inventory["main"])
        .into_iter()
        .map(|region| region.span)
        .collect();
    for credit in credits.iter().filter(|credit| credit.image == "main") {
        for span in intersect(&[Span::new(credit.start, credit.end)], &executable) {
            let (start, end, source) = (span.start, span.end, credit.source.as_str());
            let actual = claim(start, end, &mut covered);
            if actual == 0 {
                continue;
            }
            tiles.push(Tile {
                label: format!(
                    "{} · 0x{start:08x}",
                    source.rsplit('/').next().unwrap_or(source)
                ),
                bytes: actual,
                categories: if credit.kind == "c" {
                    [actual, 0, 0, 0, 0, 0]
                } else {
                    [0, 0, 0, 0, actual, 0]
                },
                address: Some(start),
                source: Some(source.into()),
                ..Tile::default()
            });
        }
    }
    if !inventory.is_null() {
        let mut main_assembly = 0;
        for interval in array(&inventory["main"], "intervals") {
            let (Some(start), Some(end)) = (integer(interval, "start"), integer(interval, "end"))
            else {
                continue;
            };
            let actual = claim(start, end, &mut covered);
            if actual == 0 {
                continue;
            }
            main_assembly += actual;
        }
        if main_assembly > 0 {
            tiles.push(Tile {
                label: "Main image assembly".into(),
                bytes: main_assembly,
                categories: [0, 0, 0, main_assembly, 0, 0],
                group: Some("main-image-code".into()),
                source: Some("games/THE LOST AGE/raw/main.s".into()),
                ..Tile::default()
            });
        }
        for overlay in array(&inventory, "overlays") {
            let (Some(start), Some(end)) =
                (integer(overlay, "rom_start"), integer(overlay, "rom_end"))
            else {
                continue;
            };
            let actual = claim(start, end, &mut covered);
            if actual == 0 {
                continue;
            }
            let id = text(overlay, "id");
            let source = format!("games/THE LOST AGE/raw/overlays/{id}_overlay.s");
            tiles.push(Tile {
                label: format!("{id} · compressed code overlay"),
                bytes: actual,
                categories: [0, 0, 0, actual, 0, 0],
                group: Some("compressed-code-overlay".into()),
                address: Some(start),
                source: Some(source),
                ..Tile::default()
            });
        }
    }
    for region in array(&manifest, "regions") {
        let (Some(start), Some(size)) = (integer(region, "address"), integer(region, "size"))
        else {
            continue;
        };
        let actual = claim(start, start + size, &mut covered);
        if actual == 0 {
            continue;
        }
        let kind = text(region, "kind");
        let source = array(region, "sources")
            .first()
            .and_then(Value::as_str)
            .unwrap_or("games/THE LOST AGE/");
        tiles.push(Tile {
            label: format!(
                "{} · {} · 0x{:08x}",
                source.rsplit('/').next().unwrap_or(source),
                kind,
                start
            ),
            bytes: actual,
            categories: [0, 0, 0, 0, 0, actual],
            group: Some(kind),
            address: Some(start),
            source: Some(source.into()),
            ..Tile::default()
        });
    }
    if let Some(index) = super::audit::index::current(tree, "tla-en") {
        let mut groups: BTreeMap<String, Tile> = BTreeMap::new();
        for row in array(&index, "regions") {
            let (Some(start), Some(end)) = (integer(row, "start"), integer(row, "end")) else {
                continue;
            };
            let actual = claim(start, end, &mut covered);
            if actual == 0 {
                continue;
            }
            let kind = text(row, "kind");
            let tile = groups.entry(kind.clone()).or_insert_with(|| Tile {
                label: text(row, "label"),
                group: Some(format!("indexed-{kind}")),
                // These bytes have an identity, but no reconstructed file yet.
                source: Some("games/THE LOST AGE/".into()),
                ..Tile::default()
            });
            tile.bytes += actual;
            tile.categories[5] += actual;
        }
        tiles.extend(groups.into_values());
    } else if let Some(audit) = json(tree, "out/tla-en/reports/executable-audit-candidate.json") {
        for resource in array(&audit, "resources") {
            if text(resource, "role") != "data-resource" {
                continue;
            }
            let (Some(start), Some(end)) =
                (integer(resource, "rom_start"), integer(resource, "rom_end"))
            else {
                continue;
            };
            let actual = claim(start, end, &mut covered);
            if actual == 0 {
                continue;
            }
            let id = text(resource, "id");
            tiles.push(Tile {
                label: format!("{id} · compressed data resource"),
                bytes: actual,
                categories: [0, 0, 0, 0, 0, actual],
                group: Some("unreconstructed-data".into()),
                address: Some(start),
                source: Some("games/THE LOST AGE/".into()),
                ..Tile::default()
            });
        }
    }
    let rest = bytes(&subtract(&[Span::new(ROM_BASE, ROM_BASE + rom)], &covered));
    if rest > 0 {
        tiles.push(Tile {
            label: UNIDENTIFIED.into(),
            bytes: rest,
            categories: [0, 0, 0, 0, 0, rest],
            group: Some("unreconstructed-data".into()),
            // Keep cartridge bytes inside their game, without inventing a file owner.
            source: Some("games/THE LOST AGE/".into()),
            ..Tile::default()
        });
    }
    tiles
}
fn asset_tiles(tree: &SourceTree, target: &DecompTarget, data: &[Span], rom: i64) -> Vec<Tile> {
    let Some(manifest) = json(
        tree,
        &format!("{}/full/assets/manifest.json", target.output_dir),
    ) else {
        return vec![Tile {
            label: UNIDENTIFIED.into(),
            bytes: bytes(data),
            categories: [0, 0, bytes(data), 0, 0, 0],
            ..Tile::default()
        }];
    };
    let sequence_classes = sound_sequence_classes(
        &tree
            .read(&format!(
                "{}/SOUND/SEQUENCE/SEQUENCES.TSV",
                target.game_dir()
            ))
            .unwrap_or_default(),
    );
    let mut groups: BTreeMap<String, Vec<Tile>> = BTreeMap::new();
    let mut covered = Vec::new();
    for region in array(&manifest, "regions") {
        let Some(start) = integer(region, "address") else {
            continue;
        };
        let Some(size) = integer(region, "size") else {
            continue;
        };
        let span = Span::new(start, start + size);
        let actual_spans = subtract(&intersect(&[span], data), &covered);
        let actual = bytes(&actual_spans);
        if actual == 0 {
            continue;
        }
        covered.extend_from_slice(&actual_spans);
        let kind = text(region, "kind");
        let sources = array(region, "sources");
        let source = sources
            .iter()
            .filter_map(Value::as_str)
            .find(|source| {
                kind == "golden-sun-pcm-wave" && source.to_ascii_lowercase().ends_with(".wav")
            })
            .or_else(|| sources.first().and_then(Value::as_str))
            .unwrap_or(target.asset_manifest);
        let owner = if kind == "golden-sun-sound-sequence" {
            format!("{}/SOUND/SEQUENCE/SEQUENCES.TSV", target.game_dir())
        } else {
            sources
                .first()
                .and_then(Value::as_str)
                .unwrap_or(source)
                .to_string()
        };
        let mut children = if kind == "components" {
            sprite_children(tree, &owner, span, &actual_spans)
        } else {
            Vec::new()
        };
        if kind == "components" && children.is_empty() {
            children = component_children(region, &actual_spans);
        }
        let group = match children.first().and_then(|child| child.group.clone()) {
            Some(group) => group,
            None => kind.clone(),
        };
        groups.entry(owner).or_default().push(Tile {
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
            label: format!("{UNIDENTIFIED} · {rom} bytes"),
            bytes: bytes(data),
            categories: [0, 0, bytes(data), 0, 0, 0],
            ..Tile::default()
        }];
    }
    let mut result: Vec<Tile> = groups
        .into_iter()
        .map(|(source, mut children)| {
            if children.len() == 1 {
                children.pop().unwrap()
            } else {
                source_container(source, children)
            }
        })
        .collect();
    for gap in subtract(data, &covered) {
        result.push(Tile {
            label: format!("{UNIDENTIFIED} · 0x{:08x}", gap.start),
            bytes: gap.bytes(),
            categories: [0, 0, gap.bytes(), 0, 0, 0],
            group: Some("unclassified".into()),
            address: Some(gap.start),
            ..Tile::default()
        });
    }
    result
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
/// Every image's audited executable spans, classified exactly as the coverage
/// map counts them. Overlay spans use resource coordinates.
pub struct Classification {
    main: Vec<Region>,
    pub main_exec: Vec<Span>,
    pub overlay_exec: SpanMap,
    pub exact_main: Vec<Span>,
    owners: OwnerMap,
    pub exact_overlay: SpanMap,
    /// Assembly the register classifies without crediting it.
    pub withdrawn_main: Vec<Span>,
    pub withdrawn_draft_main: Vec<Span>,
    pub retained_main: Vec<Span>,
    pub withdrawn_overlay: SpanMap,
    pub withdrawn_draft_overlay: SpanMap,
    pub retained_overlay: SpanMap,
    /// Draft C that neither exact C nor credited assembly already explains.
    pub semantic_main: Vec<Span>,
    pub semantic_overlay: SpanMap,
    draft_sources: usize,
}
pub fn classify(options: &BuildOptions) -> Result<Classification, String> {
    let target = crate::targets::decomp_target(Some(&options.target))?;
    let game = crate::compiler::routing::game_directory(target.compiler.as_str());
    let inventory = read_json(
        options.exact,
        &format!("games/{game}/metrics/executable.json"),
    )?;
    validated_inventory(&inventory, &options.target)?;
    let main = regions(&inventory["main"]);
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
    let overlay_dir = target.overlay_dir();
    let pairs = overlay_ids_for(options.exact, &overlay_dir);
    let (owners, exact_overlay) = exact_overlay_for(options.exact, &target, &pairs, &overlay_exec)?;
    // Overlay linkage also admits Pascal-authorized, audited veneer reconstruction.
    // Other assembly still needs handwritten/library provenance per range.
    let (withdrawn_main, withdrawn_draft_main, retained_main) =
        main_assembly_classification_for(options.exact, &target);
    let (mut withdrawn_overlay, withdrawn_draft_overlay, mut retained_overlay) =
        overlay_assembly_classification_for(
            options.exact,
            &target,
            &overlay_regions,
            &overlay_exec,
        )?;
    let (runtime_main, runtime_overlay) =
        runtime_credit_for(options.exact, &target, &main_exec, &overlay_exec)?;
    let retained_main = normalize(&[retained_main, runtime_main].concat());
    let withdrawn_main = subtract(&withdrawn_main, &retained_main);
    for (id, spans) in runtime_overlay {
        let retained = retained_overlay.entry(id.clone()).or_default();
        *retained = normalize(&[retained.clone(), spans].concat());
        let retained = retained.clone();
        if let Some(withdrawn) = withdrawn_overlay.get_mut(&id) {
            *withdrawn = subtract(withdrawn, &retained);
        }
    }
    let (candidate_main, candidate_main_sources) = options
        .recon
        .map(|tree| candidate_main(tree, &target, &main_exec))
        .unwrap_or_default();
    let (candidate_overlay, candidate_overlay_sources) = options
        .recon
        .map(|tree| candidate_overlay(tree, &target, &overlay_exec))
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
    Ok(Classification {
        main,
        main_exec,
        overlay_exec,
        exact_main,
        owners,
        exact_overlay,
        withdrawn_main,
        withdrawn_draft_main,
        retained_main,
        withdrawn_overlay,
        withdrawn_draft_overlay,
        retained_overlay,
        semantic_main,
        semantic_overlay,
        draft_sources: candidate_main_sources + candidate_overlay_sources,
    })
}

/// Called only after the complete production image has compared byte-exact.
/// Preserve verified source attribution before presentation constructs tiles.
pub fn verified_credits(options: &BuildOptions) -> Result<Vec<super::proof::Credit>, String> {
    let classified = classify(options)?;
    let mut credits = Vec::new();
    for (category, ranges, stage) in [
        ("c", &classified.exact_main, "claimed"),
        ("assembly", &classified.retained_main, "asm"),
    ] {
        let manifest = read_json(
            options.exact,
            &format!("out/{}/full/{stage}/manifest.json", options.target),
        )?;
        let mut covered = Vec::new();
        for region in array(&manifest, "regions") {
            let (Some(start), Some(size)) = (integer(region, "address"), integer(region, "size"))
            else {
                continue;
            };
            for span in intersect(&[Span::new(start, start + size)], ranges) {
                covered.push(span);
                credits.push(super::proof::Credit {
                    image: "main".into(),
                    start: span.start,
                    end: span.end,
                    source: text(region, "source"),
                    kind: category.into(),
                });
            }
        }
        if bytes(&normalize(&covered)) != bytes(ranges) {
            return Err("main credit has no build source".into());
        }
    }
    for (id, owners) in classified.owners {
        for owner in owners {
            for span in owner.spans {
                credits.push(super::proof::Credit {
                    image: id.clone(),
                    start: span.start,
                    end: span.end,
                    source: owner.source.clone(),
                    kind: "c".into(),
                });
            }
        }
    }
    let target = crate::targets::decomp_target(Some(&options.target))?;
    for (id, spans) in classified.retained_overlay {
        for span in spans {
            credits.push(super::proof::Credit {
                source: target.overlay_assembly(&id),
                image: id.clone(),
                start: span.start,
                end: span.end,
                kind: "assembly".into(),
            });
        }
    }
    Ok(credits)
}
pub fn build_coverage_map(options: &BuildOptions) -> Result<CoverageMap, String> {
    let root = match options.exact {
        SourceTree::Work { root, .. } => root,
        SourceTree::Ref { .. } => {
            return Err("historical coverage needs verification in that checkout; today's build receipt cannot score another revision".into());
        }
    };
    let done = super::progress::measured(root, &options.target)?
        .ok_or("coverage requires a complete executable inventory")?;
    let mut game_scores = Map::new();
    game_scores.insert(options.target.clone(), serde_json::to_value(done).unwrap());
    if options.target == "tbs-en" {
        game_scores.insert(
            "tla-en".into(),
            serde_json::to_value(
                super::progress::measured(root, "tla-en")?
                    .ok_or("combined coverage requires TLA's executable inventory")?,
            )
            .unwrap(),
        );
    }
    let rom = rom_size(&options.target)?;
    let target = crate::targets::decomp_target(Some(&options.target))?;
    let Classification {
        main,
        main_exec,
        overlay_exec,
        exact_main,
        owners,
        exact_overlay,
        withdrawn_main,
        withdrawn_draft_main,
        retained_main,
        withdrawn_overlay,
        withdrawn_draft_overlay,
        retained_overlay,
        semantic_main,
        semantic_overlay,
        draft_sources,
    } = classify(options)?;
    let withdrawn_assembly = bytes(&withdrawn_main)
        + bytes(&withdrawn_draft_main)
        + mapped_bytes(&withdrawn_overlay)
        + mapped_bytes(&withdrawn_draft_overlay);
    let draft_main: Vec<Span> = Vec::new();
    let draft_overlay = SpanMap::new();
    let exact_overlay_bytes = mapped_bytes(&exact_overlay);
    let exact_bytes = bytes(&exact_main) + exact_overlay_bytes;
    let semantic_overlay_bytes = mapped_bytes(&semantic_overlay);
    let semantic_bytes = bytes(&semantic_main) + semantic_overlay_bytes;
    let mut main_sources = Vec::new();
    if let SourceTree::Work { root, .. } = options.exact {
        for credit in super::proof::read(root, &options.target)?.credits {
            if credit.image == "main" {
                main_sources.push((Span::new(credit.start, credit.end), credit.source));
            }
        }
    }
    for path in [
        format!("out/{}/full/claimed/manifest.json", options.target),
        format!("out/{}/full/asm/manifest.json", options.target),
    ] {
        if let Some(manifest) = json(options.exact, &path) {
            for region in array(&manifest, "regions") {
                if let (Some(start), Some(size), Some(source)) = (
                    integer(region, "address"),
                    integer(region, "size"),
                    region.get("source").and_then(Value::as_str),
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
            &target.overlay_dir(),
        ));
    }
    executable_areas.push(area("overlays", "Decoded code overlays", overlay_tiles_all));
    let mut code = main_exec.clone();
    let ss = streams(options.exact, &target);
    let rom_span = Span::new(ROM_BASE, ROM_BASE + rom);
    validate_streams(&ss, rom_span, &main_exec)?;
    for stream in &ss {
        code.push(Span::new(stream.start, stream.start + stream.rom));
    }
    let data = subtract(&[Span::new(ROM_BASE, ROM_BASE + rom)], &code);
    let main_tiles = executable_areas
        .first()
        .map(|area| area.tiles.clone())
        .unwrap_or_else(|| {
            bands(
                &main_exec,
                &exact_main,
                &semantic_main,
                &draft_main,
                &retained_main,
                65536,
            )
        });
    let mut rom_areas = vec![area("rom-main-code", "Main image code", main_tiles)];
    let mut stream_tiles = Vec::new();
    for stream in &ss {
        let decoded = bytes(mapped(&overlay_exec, &stream.id));
        let categories = stream_categories(
            stream.rom,
            decoded,
            mapped(&exact_overlay, &stream.id),
            mapped(&semantic_overlay, &stream.id),
            &subtract(
                mapped(&draft_overlay, &stream.id),
                &[
                    mapped(&exact_overlay, &stream.id).to_vec(),
                    mapped(&semantic_overlay, &stream.id).to_vec(),
                    mapped(&retained_overlay, &stream.id).to_vec(),
                ]
                .concat(),
            ),
            mapped(&retained_overlay, &stream.id),
        );
        stream_tiles.push(Tile {
            label: overlay_short(&stream.id).into(),
            bytes: stream.rom,
            categories: [
                categories[0],
                categories[1],
                categories[2],
                categories[3],
                categories[4],
                0,
            ],
            group: Some("compressed-overlay".into()),
            address: Some(stream.start),
            source: stream.source.clone(),
            ..Tile::default()
        });
    }
    rom_areas.push(area(
        "rom-overlay-streams",
        "Compressed code overlays",
        stream_tiles,
    ));
    rom_areas.push(area(
        "rom-data",
        "Assets & data",
        asset_tiles(options.exact, &target, &data, rom),
    ));
    let physical_total = rom_areas.iter().map(|area| area.bytes).sum::<i64>();
    if physical_total != rom {
        return Err(format!(
            "ROM treemap covers {physical_total} bytes, expected {rom}"
        ));
    }
    // games/ holds both ROMs; The Lost Age joins the contents tree beside The
    // Broken Seal without entering its DONE totals.
    if options.target == "tbs-en" {
        let SourceTree::Work { root, .. } = options.exact else {
            return Err("combined coverage requires current source verification".into());
        };
        let receipt = super::proof::read(root, "tla-en")?;
        rom_areas.push(area(
            "rom-lost-age",
            "The Lost Age ROM",
            lost_age_tiles(options.exact, &receipt.credits),
        ));
    }
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
    if done.executable != executable
        || done.common_c + done.game_c != exact_bytes
        || done.common_asm + done.game_asm != retained
    {
        return Err(
            "coverage categories disagree with verified build credit; regenerate the build receipt"
                .into(),
        );
    }
    let document = json!({
        "format": 1,
        "kind": "golden-sun-rom-coverage-map",
        "done": done,
        "games": game_scores,
        "target": options.target,
        "derivation": "tracked-evidence-v1",
        "rom_bytes": rom,
        "asset_verification": json(options.exact, &format!("out/{}/full/assets/manifest.json", options.target))
            .and_then(|manifest| manifest.get("verification").cloned()),
        "shared_map_assets": if options.target == "tbs-en" {
            shared_map_assets(options.exact, &rom_areas)?
        } else { json!({}) },
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
            "draft_sources": draft_sources as i64,
            "main_draft_census": format!("{}/recon/en/dossiers.json", target.game_dir()),
            "proven_assembly_standard": "handwritten-or-library-proven; audited-overlay-veneer-reconstruction; container-built-compiler-runtime",
            "credited_assembly_bytes": bytes(&retained_main) + mapped_bytes(&retained_overlay),
            "withdrawn_assembly_bytes": withdrawn_assembly,
            "main_assembly_classification": format!("{}/full/asm/manifest.json", target.output_dir),
            "overlay_assembly_classification": format!("{}/semantic/overlay-assembly.json", target.game_dir()),
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

    fn inventory_fixture() -> Value {
        json!({
            "format": 1,
            "metric": "full-c-byte-share",
            "target": "tla-en",
            "state": "audited",
            "audit": "complete",
            "total_union_bytes": 8,
            "overlay_count": 1,
            "main": {
                "id": "main", "audit": "complete", "executable_bytes": 4,
                "intervals": [{"start": 0x08000100, "end": 0x08000104}]
            },
            "overlays": [{
                "id": "resource_test", "audit": "complete", "decoded_bytes": 8,
                "executable_bytes": 4, "excluded_bytes": 4,
                "intervals": [{"start": 0x02000000, "end": 0x02000004}]
            }]
        })
    }

    #[test]
    fn executable_inventory_requires_a_complete_partition_before_scoring() {
        let inventory = inventory_fixture();
        let (main, overlays) = validated_inventory(&inventory, "tla-en").unwrap();
        assert_eq!(bytes(&main), 4);
        assert_eq!(mapped_bytes(&overlays), 4);

        let mut pending = inventory.clone();
        pending["state"] = json!("pending");
        pending["audit"] = json!("incomplete");
        assert!(validated_inventory(&pending, "tla-en")
            .unwrap_err()
            .contains("withheld"));

        let mut contradictory = inventory.clone();
        contradictory["state"] = json!("pending");
        assert!(validated_inventory(&contradictory, "tla-en")
            .unwrap_err()
            .contains("state and audit disagree"));

        let mut missing_overlay = inventory.clone();
        missing_overlay["overlay_count"] = json!(2);
        assert!(validated_inventory(&missing_overlay, "tla-en")
            .unwrap_err()
            .contains("overlay count"));

        let mut unpartitioned = inventory;
        unpartitioned["overlays"][0]["excluded_bytes"] = json!(3);
        assert!(validated_inventory(&unpartitioned, "tla-en")
            .unwrap_err()
            .contains("classification is incomplete"));
    }

    #[test]
    fn malformed_audit_ranges_are_not_silently_dropped() {
        let mut inventory = inventory_fixture();
        inventory["main"]["intervals"]
            .as_array_mut()
            .unwrap()
            .push(json!({"start": 12, "end": 8}));
        assert!(validated_inventory(&inventory, "tla-en").is_err());
    }

    #[test]
    fn component_files_use_physical_build_extents_and_preserve_clipping() {
        let mut region = json!({"address":100,"size":10,"sources":["INDEX.json"],"details":{"components":[
            {"address":100,"size":4,"kind":"gba-4bpp-tiles","sources":["IMAGE.png"]},
            {"address":104,"size":6,"kind":"byte-fill","sources":[]}
        ]}});
        let children = component_children(&region, &[Span::new(102, 109)]);
        assert_eq!(children.iter().map(|child| child.bytes).sum::<i64>(), 7);
        assert_eq!(children[0].source.as_deref(), Some("IMAGE.png"));
        assert_eq!(children[1].source.as_deref(), Some("INDEX.json"));
        assert_eq!(children[0].bytes, 2);
        for address in [103, 105] {
            region["details"]["components"][1]["address"] = json!(address);
            assert!(component_children(&region, &[Span::new(100, 110)]).is_empty());
        }
        region["details"]["components"][1]["address"] = json!(104);
        for size in [0, 5, 7] {
            region["details"]["components"][1]["size"] = json!(size);
            assert!(component_children(&region, &[Span::new(100, 110)]).is_empty());
        }
    }

    #[test]
    fn shared_map_links_follow_load_tables_without_adding_tiles() {
        let root = tempfile::tempdir().unwrap();
        let write = |path: &str, source: String| {
            let path = root.path().join(path);
            std::fs::create_dir_all(path.parent().unwrap()).unwrap();
            std::fs::write(path, source).unwrap();
        };
        write(
            "games/THE BROKEN SEAL/locations.tsv",
            "resource_3a0\tXian\t\t\t\t\tSRC/FIELD/XIAN\n".into(),
        );
        write(
            "games/THE BROKEN SEAL/SRC/FIELD/COMMON/SCENE_TABLE.JSON",
            json!({"segments":[{
                "address":"0x0809f1a8", "records":[{"resource_id":928,"map_index":7},
                    {"resource_id":928,"map_index":7}, {"resource_id":999,"map_index":99}]
            }]})
            .to_string(),
        );
        write(
            "games/THE BROKEN SEAL/SRC/FIELD/COMMON/LOAD_TABLE.JSON",
            json!({"fields":["palette","tiles"],
            "records":[{"map_index":7,"palette":"0","tiles":"1"}]})
            .to_string(),
        );
        write(
            "games/THE BROKEN SEAL/SRC/SYSTEM/RESOURCE/DIRECTORY.JSON",
            json!({"slots":["0x08001000","0x08002000"]}).to_string(),
        );
        let tiles = [
            (0x08001000, "games/THE BROKEN SEAL/GRAPHICS/TILE/SHARED.PNG"),
            (0x08002000, "games/THE BROKEN SEAL/SRC/FIELD/XIAN/MAP.PNG"),
        ]
        .map(|(address, source)| Tile {
            address: Some(address),
            source: Some(source.into()),
            bytes: 32,
            ..Tile::default()
        });
        let areas = vec![area("rom-data", "Data", tiles.into())];
        let tree = SourceTree::Work {
            id: "fixture".into(),
            root: root.path().into(),
        };
        assert_eq!(
            shared_map_assets(&tree, &areas).unwrap(),
            json!({
                "games/THE BROKEN SEAL/SRC/FIELD/XIAN/": ["games/THE BROKEN SEAL/GRAPHICS/TILE/SHARED.PNG"]
            })
        );
        assert_eq!(areas[0].bytes, 64);
        write(
            "games/THE BROKEN SEAL/SRC/SYSTEM/RESOURCE/DIRECTORY.JSON",
            json!({"slots":[]}).to_string(),
        );
        assert!(shared_map_assets(&tree, &areas).is_err());
    }

    fn classification(regions: Value) -> Value {
        json!({"format": 1, "regions": regions})
    }
    #[test]
    fn main_assembly_credit_requires_manifest_range_provenance() {
        let directory = tempfile::tempdir().unwrap();
        let write = |path: &str, value: Value| {
            let path = directory.path().join(path);
            std::fs::create_dir_all(path.parent().unwrap()).unwrap();
            std::fs::write(path, serde_json::to_vec(&value).unwrap()).unwrap();
        };
        let row = |address, provenance| {
            json!({
                "source": format!("games/THE BROKEN SEAL/raw/{address:08x}.s"),
                "address": address,
                "size": 8,
                "kind": "legacy_group_credit",
                "retention": "keep_asm",
                "confidence": "proven",
                "evidence": "group classification",
                "provenance": provenance
            })
        };
        write(
            "games/THE BROKEN SEAL/raw/classification.json",
            json!({"groups": [{
                "kind": "legacy_group_credit",
                "provenance": {"credit": "library", "proof": "legacy group proof"}
            }]}),
        );
        write(
            "out/tbs-en/full/asm/manifest.json",
            json!({"regions": [
                row(0x08000100, json!({
                    "credit": "library",
                    "evidence": ["per-range identity"],
                    "proof": "twelve-ROM comparison"
                })),
                row(0x08000120, Value::Null),
                row(0x08000140, json!({
                    "credit": "library",
                    "evidence": [""],
                    "proof": "twelve-ROM comparison"
                })),
                row(0x08000160, json!({
                    "credit": "library",
                    "evidence": ["per-range identity"]
                })),
                row(0x08000180, json!({
                    "credit": "handwritten",
                    "evidence": ["per-range handwritten idiom"],
                    "object": "named source object"
                }))
            ]}),
        );
        let tree = crate::coverage::tree::work_tree_at(directory.path().into());
        let (proven, draft, credited) = main_assembly_classification(&tree);
        assert!(draft.is_empty());
        assert_eq!(
            credited,
            [
                Span::new(0x0800_0100, 0x0800_0108),
                Span::new(0x0800_0180, 0x0800_0188)
            ]
        );
        assert_eq!(
            proven,
            [
                Span::new(0x0800_0120, 0x0800_0128),
                Span::new(0x0800_0140, 0x0800_0148),
                Span::new(0x0800_0160, 0x0800_0168)
            ]
        );
    }

    #[test]
    fn runtime_credit_stays_separate_from_range_credits() {
        // No tracked assembly carries the compiler runtime: the call_via bank
        // at 0x080072e4 is a container-built link, credited exactly where the
        // built manifest placed it.
        let tree = crate::coverage::tree::work_tree();
        let rom = [Span::new(0x0800_0000, 0x0880_0000)];
        let inventory = read_json(&tree, "games/THE BROKEN SEAL/metrics/executable.json").unwrap();
        let overlays = array(&inventory, "overlays")
            .iter()
            .map(|node| {
                (
                    text(node, "id"),
                    validated_executable(node).expect("audited overlay"),
                )
            })
            .collect();
        let (main, _) = runtime_credit(&tree, &rom, &overlays).unwrap();
        assert!(
            main.contains(&Span::new(0x0800_72e4, 0x0800_7320)),
            "runtime spans: {main:?}"
        );
    }
    #[test]
    fn runtime_credit_requires_complete_audited_runtime_windows() {
        let directory = tempfile::tempdir().unwrap();
        let root = directory.path();
        let write = |path: &str, text: String| {
            let path = root.join(path);
            std::fs::create_dir_all(path.parent().unwrap()).unwrap();
            std::fs::write(path, text).unwrap();
        };
        write(
            crate::compiler::runtime::REGISTRY,
            json!({"links": [
                {"image": "main", "text": "0x080072e4", "members": ["_m"]},
                {"image": "main", "text": "0x08001000", "members": ["_m"]},
                {"image": "resource_3bf", "text": "0x020057b0", "rodata": "0x02005f90", "members": ["_m"]},
                {"image": "resource_373", "text": "0x02006154", "members": ["_m"]}
            ]})
            .to_string(),
        );
        write(
            "out/tbs-en/full/asm/manifest.json",
            json!({"regions": [
                {"address": 0x0800_72e4, "size": 60, "source": crate::compiler::runtime::REGISTRY, "retention": "container_runtime"},
                {"address": 0x0800_1000, "size": 60, "source": "games/THE BROKEN SEAL/raw/08001000.s", "retention": "keep_asm"}
            ]})
            .to_string(),
        );
        write(
            "games/THE BROKEN SEAL/raw/overlays/resource_3bf_overlay.s",
            "AlchemyRuntime_020057b0:\n\t.space 0x728\n\t.4byte 1\nAlchemyRuntime_02005f90:\n\t.space 0x14\n".into(),
        );
        write(
            "games/THE BROKEN SEAL/raw/overlays/resource_373_overlay.s",
            "\t.4byte 0\n".into(),
        );
        let tree = crate::coverage::tree::work_tree_at(root.to_path_buf());
        let rom = [Span::new(0x0800_0000, 0x0880_0000)];
        let partial = SpanMap::from([
            (
                "resource_3bf".to_string(),
                vec![Span::new(0x0200_57bc, 0x0200_6000)],
            ),
            (
                "resource_373".to_string(),
                vec![Span::new(0x0200_6154, 0x0200_6190)],
            ),
        ]);
        assert!(runtime_credit(&tree, &rom, &partial)
            .unwrap_err()
            .contains("lies outside audited executable intervals"));
        let executable = SpanMap::from([
            (
                "resource_3bf".to_string(),
                vec![Span::new(0x0200_57b0, 0x0200_5ed8)],
            ),
            (
                "resource_373".to_string(),
                vec![Span::new(0x0200_6154, 0x0200_6190)],
            ),
        ]);
        let (main, overlays) = runtime_credit(&tree, &rom, &executable).unwrap();
        assert_eq!(main, [Span::new(0x0800_72e4, 0x0800_7320)]);
        assert_eq!(
            overlays["resource_3bf"],
            [Span::new(0x0200_57b0, 0x0200_5ed8)]
        );
        assert!(mapped(&overlays, "resource_373").is_empty());
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
    fn lost_age_coverage_reads_only_lost_age_owner_paths() {
        let root = tempfile::tempdir().unwrap();
        let write = |path: &str, text: &str| {
            let path = root.path().join(path);
            std::fs::create_dir_all(path.parent().unwrap()).unwrap();
            std::fs::write(path, text).unwrap();
        };
        write(
            "games/THE LOST AGE/source-paths.json",
            &json!({"format": 3, "owners": {
                "resource_64a:02000100": "FIELD/TEST.C"
            }})
            .to_string(),
        );
        write(
            "games/THE LOST AGE/SRC/FIELD/TEST.C",
            "void Test(void) {}\n",
        );
        write(
            "games/THE LOST AGE/raw/overlays/resource_64a_overlay.s",
            "AlchemyC_02000100:\n\t.space 0x20\n",
        );
        let tree = crate::coverage::tree::work_tree_at(root.path().to_path_buf());
        let target = crate::targets::decomp_target(Some("tla-en")).unwrap();
        let executable = SpanMap::from([(
            "resource_64a".into(),
            vec![Span::new(0x0200_0100, 0x0200_0120)],
        )]);
        let pairs = overlay_ids_for(&tree, &target.overlay_dir());
        let (owners, spans) = exact_overlay_for(&tree, &target, &pairs, &executable).unwrap();
        assert_eq!(bytes(&spans["resource_64a"]), 0x20);
        assert_eq!(
            owners["resource_64a"][0].source,
            "games/THE LOST AGE/SRC/FIELD/TEST.C"
        );
    }

    #[test]
    fn exact_overlay_ownership_requires_full_audit_containment() {
        let root = tempfile::tempdir().unwrap();
        let write = |path: &str, text: &str| {
            let path = root.path().join(path);
            std::fs::create_dir_all(path.parent().unwrap()).unwrap();
            std::fs::write(path, text).unwrap();
        };
        write(
            "games/THE LOST AGE/source-paths.json",
            &json!({"format": 3, "owners": {
                "resource_64a:02000100": "FIELD/TEST.C"
            }})
            .to_string(),
        );
        write(
            "games/THE LOST AGE/SRC/FIELD/TEST.C",
            "void Test(void) {}\n",
        );
        write(
            "games/THE LOST AGE/raw/overlays/resource_64a_overlay.s",
            "AlchemyC_02000100:\n\t.space 0x20\n",
        );
        let tree = crate::coverage::tree::work_tree_at(root.path().to_path_buf());
        let target = crate::targets::decomp_target(Some("tla-en")).unwrap();
        let executable = SpanMap::from([(
            "resource_64a".into(),
            vec![Span::new(0x0200_0100, 0x0200_0110)],
        )]);
        let pairs = overlay_ids_for(&tree, &target.overlay_dir());
        let error = exact_overlay_for(&tree, &target, &pairs, &executable).unwrap_err();
        assert!(
            error.contains(
                "resource_64a:02000100 C ownership is outside audited executable intervals"
            ),
            "{error}"
        );
    }

    #[test]
    fn lost_age_rom_tree_classifies_audited_code_and_remaining_data() {
        let root = tempfile::tempdir().unwrap();
        let write = |path: &str, value: Value| {
            let path = root.path().join(path);
            std::fs::create_dir_all(path.parent().unwrap()).unwrap();
            std::fs::write(path, serde_json::to_vec(&value).unwrap()).unwrap();
        };
        write("out/tla-en/assets/manifest.json", json!({"regions": []}));
        write(
            "out/tla-en/reports/executable-audit-candidate.json",
            json!({
                "main": {"intervals": [
                    {"start": 0x08000100, "end": 0x08000120, "kind": "thumb"}
                ]},
                "overlays": [{
                    "id": "resource_649", "rom_start": 0x08001000,
                    "rom_end": 0x08001100
                }],
                "resources": []
            }),
        );
        write(
            "games/THE LOST AGE/metrics/executable.json",
            json!({"main": {"intervals": [{
                "start": 0x08000100, "end": 0x08000120
            }]}, "overlays": [{
                "id": "resource_649", "rom_start": 0x08001000,
                "rom_end": 0x08001100
            }]}),
        );
        let tree = crate::coverage::tree::work_tree_at(root.path().to_path_buf());
        let tiles = lost_age_tiles(
            &tree,
            &[crate::coverage::proof::Credit {
                image: "main".into(),
                start: 0x08000100,
                end: 0x08000108,
                source: "games/COMMON/SRC/SOUND/TEST.C".into(),
                kind: "c".into(),
            }],
        );
        let totals = tiles.iter().fold([0; 6], |mut totals, tile| {
            for (total, bytes) in totals.iter_mut().zip(tile.categories) {
                *total += bytes;
            }
            totals
        });
        assert_eq!(totals[Category::Unknown as usize], 0);
        assert_eq!(totals[Category::ProvenC as usize], 8);
        assert_eq!(totals[Category::DraftAsm as usize], 24 + 0x100);
        assert_eq!(totals[Category::AssetData as usize], 0x1000000 - 32 - 0x100);
        assert!(tiles.iter().all(|tile| !tile
            .source
            .as_deref()
            .unwrap_or("")
            .ends_with("assets.json")));
        let pending = tiles
            .iter()
            .find(|tile| tile.label == "Unidentified")
            .unwrap();
        assert_eq!(pending.source.as_deref(), Some("games/THE LOST AGE/"));
        assert_eq!(pending.group.as_deref(), Some("unreconstructed-data"));
    }

    #[test]
    fn coverage_credits_instance_owners_once_in_their_own_overlay_with_compiler_gap() {
        let root = tempfile::tempdir().unwrap();
        let write = |path: &str, text: &str| {
            let path = root.path().join(path);
            std::fs::create_dir_all(path.parent().unwrap()).unwrap();
            std::fs::write(path, text).unwrap();
        };
        let source = "games/THE BROKEN SEAL/SRC/FIELD/COMMON/OBJECT/STAGED_ACTOR.C";
        write(source, "void FieldScene_FindActorRegion(void) {}\n");
        let mut register = serde_json::Map::new();
        for (image, find, redraw) in [
            ("resource_3bf", "0200034c", "020008c0"),
            ("resource_39b", "02000630", "02000ba4"),
        ] {
            for (address, name) in [
                (find, "FieldScene_FindActorRegion"),
                (redraw, "FieldScene_RedrawActorFootprint"),
            ] {
                let record = json!({"name": name, "source": "FIELD/COMMON/OBJECT/STAGED_ACTOR.C"});
                register.insert(format!("{image}:{address}"), record);
            }
        }
        write(
            SOURCE_PATHS_MANIFEST,
            &json!({"format": 3, "owners": register}).to_string(),
        );
        // A 1394-byte first member leaves one halfword of fill in each image.
        write(
            "games/THE BROKEN SEAL/raw/overlays/resource_3bf_overlay.s",
            "AlchemyC_0200034c:\n\t.space 0x572\n\t.short 0\nAlchemyC_020008c0:\n\t.space 0x11c\n",
        );
        write(
            "games/THE BROKEN SEAL/raw/overlays/resource_39b_overlay.s",
            "AlchemyC_02000630:\n\t.space 0x572\n\t.short 0\nAlchemyC_02000ba4:\n\t.space 0x11c\n",
        );
        let units = |gap: (&str, &str)| {
            json!({"units": [{
                "id": "staged-actor", "game": "tbs", "source": source, "overlay": "resource_3bf",
                "owners": [
                    {"address": "0x0200034c", "extent": 1394, "state": "exact-c"},
                    {"address": "0x020008c0", "extent": 284, "state": "exact-c"}
                ],
                "instances": {"resource_39b": {
                    "owners": {
                        "FieldScene_FindActorRegion": {"address": "0x02000630", "extent": 1394},
                        "FieldScene_RedrawActorFootprint": {"address": "0x02000ba4", "extent": 284}
                    },
                    "compiler_gaps": [{"start": gap.0, "end": gap.1}]
                }}
            }]})
            .to_string()
        };
        write(
            "games/THE BROKEN SEAL/recon/translation-units.json",
            &units(("0x02000ba2", "0x02000ba4")),
        );
        let tree = crate::coverage::tree::work_tree_at(root.path().to_path_buf());
        let executable = SpanMap::from([
            (
                "resource_3bf".into(),
                vec![Span::new(0x0200_0000, 0x0200_1000)],
            ),
            (
                "resource_39b".into(),
                vec![Span::new(0x0200_0000, 0x0200_1000)],
            ),
        ]);
        let (owners, spans) = exact_overlay(&tree, &overlay_ids(&tree), &executable).unwrap();
        let entries = |id: &str| {
            owners[id]
                .iter()
                .map(|owner| (owner.label.as_str(), owner.entry, bytes(&owner.spans)))
                .collect::<Vec<_>>()
        };
        // Each owner once, in its own overlay; the instance adds its own fill.
        assert_eq!(
            entries("resource_3bf"),
            [
                ("FieldScene_FindActorRegion", 0x0200_034c, 1394),
                ("FieldScene_RedrawActorFootprint", 0x0200_08c0, 284)
            ]
        );
        assert_eq!(
            entries("resource_39b"),
            [
                ("FieldScene_FindActorRegion", 0x0200_0630, 1394),
                ("FieldScene_RedrawActorFootprint", 0x0200_0ba4, 284),
                ("staged-actor compiler alignment", 0x0200_0ba2, 2)
            ]
        );
        assert_eq!(bytes(&spans["resource_3bf"]), 1394 + 284);
        assert_eq!(spans["resource_39b"], [Span::new(0x0200_0630, 0x0200_0cc0)]);
        // Fill that no two adjacent exact owners bound is refused.
        write(
            "games/THE BROKEN SEAL/recon/translation-units.json",
            &units(("0x020008be", "0x020008c0")),
        );
        let error = exact_overlay(&tree, &overlay_ids(&tree), &executable).unwrap_err();
        assert_eq!(
            error,
            "resource_39b: compiler gap lacks exact adjacent owners"
        );
    }
    #[test]
    fn overlay_drafts_follow_units_with_legacy_filename_fallback() {
        let root = tempfile::tempdir().unwrap();
        let directory = "games/THE BROKEN SEAL/recon/en/overlays";
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
                "game": "tbs", "overlay": "resource_test",
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
        write("games/THE BROKEN SEAL/recon/translation-units.json", &json!({"units": [
            named.clone(), named,
            unit("resource_test_c_02000160", json!([owner("0x02000160", 0x10, "retained-assembly")])),
            unit("resource_test_c_02000180", json!([owner("0x02000180", 0x10, "exact-c")])),
            unit("missing", json!([owner("0x02000190", 0x10, "retained-assembly")])),
            unit("uncanonical", json!([owner("0x020001a0", 0x10, "retained-assembly")]))
        ]}).to_string());
        write("games/THE BROKEN SEAL/recon/en/dossiers.json", &json!({"records": {
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
            candidate_overlay(
                &tree,
                &crate::targets::target_for(crate::targets::DEFAULT_TARGET),
                &executable(),
            ),
            (
                BTreeMap::from([("resource_test".into(), expected.clone())]),
                2
            )
        );
        write(
            &format!("{directory}/resource_test_c_020001c0.c"),
            "void Legacy_Run(void) {}\n",
        );
        let (found, sources) = candidate_overlay(
            &tree,
            &crate::targets::target_for(crate::targets::DEFAULT_TARGET),
            &executable(),
        );
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
            "games/THE BROKEN SEAL/SRC/GRAPHICS/CHARACTER/COMMON.JSON",
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
            "games/THE BROKEN SEAL/SRC/GRAPHICS/CHARACTER/COMMON.JSON",
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
        assert!(exact_spans(vec![Span::new(16, 24)], &executable, "x").is_err());
        let missing = SourceTree::Work {
            id: "fixture".into(),
            root: std::env::temp_dir()
                .join(format!("alchemy-coverage-missing-{}", std::process::id())),
        };
        assert!(exact_main(&missing, "tbs-en", &executable).is_err());
    }
    #[test]
    fn physical_stream_validation_rejects_bad_ranges_and_overlap() {
        let main = [Span::new(0x0800_1000, 0x0800_1100)];
        let rom = Span::new(0x0800_0000, 0x0800_2000);
        assert!(validate_streams(
            &[Stream {
                id: "outside".into(),
                start: 0x0800_1f00,
                rom: 0x200,
                source: None
            }],
            rom,
            &main
        )
        .is_err());
        assert!(validate_streams(
            &[Stream {
                id: "main".into(),
                start: 0x0800_1080,
                rom: 0x20,
                source: None
            }],
            rom,
            &main
        )
        .is_err());
        assert!(validate_streams(
            &[
                Stream {
                    id: "a".into(),
                    start: 0x0800_0000,
                    rom: 0x100,
                    source: None
                },
                Stream {
                    id: "b".into(),
                    start: 0x0800_0080,
                    rom: 0x100,
                    source: None
                },
            ],
            rom,
            &main
        )
        .is_err());
    }

    #[test]
    fn stream_ids_resolve_locations_resource_keys() {
        let locations = "resource_36f\tTitle\ttitle\t0\t0x99b\tevidence\tSRC/MENU/TITLE\n";
        assert_eq!(
            atlas_source("games/THE BROKEN SEAL", locations, "36f").as_deref(),
            Some("games/THE BROKEN SEAL/SRC/MENU/TITLE/")
        );
    }

    #[test]
    fn stream_category_partition_preserves_physical_bytes() {
        let categories = stream_categories(
            10,
            100,
            &[Span::new(0, 31)],
            &[Span::new(31, 62)],
            &[Span::new(62, 93)],
            &[Span::new(93, 100)],
        );
        assert_eq!(categories.iter().sum::<i64>(), 10);
        assert!(categories[2] >= 0);
    }
    #[test]
    fn evidence_backed_proven_assembly_is_counted() {
        let (found, draft, credited) = overlay_assembly_classification_document(
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
        assert!(credited.is_empty());
        assert_eq!(
            found["resource_test"],
            vec![Span::new(0x0200_0120, 0x0200_0140)]
        );
    }
    #[test]
    fn strong_assembly_reasoning_remains_draft() {
        let (proven, draft, credited) = overlay_assembly_classification_document(
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
        assert!(credited.is_empty());
        assert_eq!(
            draft["resource_test"],
            vec![Span::new(0x0200_0120, 0x0200_0140)]
        );
    }
    #[test]
    fn reconstructed_veneer_credit_is_restricted_to_audited_linkage() {
        let mut row = region(
            "0x02000120",
            "0x02000140",
            "proven",
            json!(["verified fixed linkage"]),
        );
        row["kind"] = json!("veneer");
        row["provenance"] = json!({"credit":"reconstructed_veneer", "proof":OVERLAY_VENEER_MACRO,
            "source":"games/THE BROKEN SEAL/SRC/FIELD/RUNPA_JO/IMPORT.INC"});
        let inventory = BTreeMap::from([(
            "resource_test".into(),
            vec![Region {
                span: Span::new(0x0200_0120, 0x0200_0140),
                kind: "veneer".into(),
                evidence: "audited".into(),
            }],
        )]);
        let (_, _, credited) = overlay_assembly_classification_document(
            &classification(json!([row.clone()])),
            &inventory,
            &executable(),
        )
        .unwrap();
        assert_eq!(
            credited["resource_test"],
            vec![Span::new(0x0200_0120, 0x0200_0140)]
        );
        assert!(!assembly_credit(&row));
        for (key, value) in [
            ("kind", json!("structured_scene_module")),
            ("confidence", json!("strong")),
            ("end", json!("0x02000148")),
            ("start", json!("0x02000122")),
            ("end", json!("0x0200013e")),
        ] {
            let mut invalid = row.clone();
            invalid[key] = value;
            assert!(overlay_assembly_classification_document(
                &classification(json!([invalid])),
                &inventory,
                &executable()
            )
            .is_err());
        }
        assert!(overlay_assembly_classification_document(
            &classification(json!([row])),
            &no_inventory(),
            &executable()
        )
        .is_err());
    }

    #[test]
    fn overlay_credit_requires_proven_range_and_its_own_provenance() {
        let mut library = region(
            "0x02000120",
            "0x02000140",
            "proven",
            json!(["historical assembly match"]),
        );
        library["provenance"] = json!({"credit":"library", "proof":"assembled bytes agree"});
        let uncredited = region(
            "0x02000140",
            "0x02000160",
            "proven",
            json!(["retained reasoning"]),
        );
        let mut strong = library.clone();
        strong["start"] = json!("0x02000160");
        strong["end"] = json!("0x02000180");
        strong["confidence"] = json!("strong");
        let mut bare = library.clone();
        bare["start"] = json!("0x02000180");
        bare["end"] = json!("0x020001a0");
        bare["provenance"] = json!({"credit":"library"});
        let (withdrawn, draft, credited) = overlay_assembly_classification_document(
            &classification(json!([library, uncredited, strong, bare])),
            &no_inventory(),
            &executable(),
        )
        .unwrap();
        assert_eq!(
            credited["resource_test"],
            vec![Span::new(0x0200_0120, 0x0200_0140)]
        );
        assert_eq!(
            draft["resource_test"],
            vec![Span::new(0x0200_0160, 0x0200_0180)]
        );
        assert_eq!(
            withdrawn["resource_test"],
            vec![
                Span::new(0x0200_0140, 0x0200_0160),
                Span::new(0x0200_0180, 0x0200_01a0)
            ]
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
