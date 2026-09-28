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
fn regions(value: &Value, evidence: Option<&Map<String, Value>>) -> Vec<Region> {
    array(value, "intervals")
        .iter()
        .filter_map(|item| {
            let evidence = item
                .get("evidence")
                .and_then(Value::as_str)
                .or_else(|| {
                    item.get("evidence_ref")
                        .and_then(Value::as_str)
                        .and_then(|key| evidence?.get(key))
                        .and_then(Value::as_str)
                })
                .unwrap_or("");
            Some(Region {
                span: Span::new(integer(item, "start")?, integer(item, "end")?),
                kind: text(item, "kind"),
                evidence: evidence.into(),
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
    overlay_ids_for(tree, "recon/tbs/raw/overlays")
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
/// The main-image C a game's full build compiled, linked and compared with
/// its ROM: the claimed stage's regions whose source is ordinary C. Each game
/// reads its own build, never an earlier receipt.
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
    }
    Ok(spans)
}

/// The audited main and overlay intervals of a target's executable
/// inventory, the only way any reader scores one. An inventory that claims to
/// be complete counts only while it is the independently verified automatic
/// count ([`super::audit::authenticate`]); until then, like a pending one, it
/// is withheld and DONE is `?`. A verified inventory that is inconsistent in
/// itself is an error.
pub(super) fn validated_inventory(
    root: &Path,
    inventory: &Value,
    target: DecompTarget,
) -> Result<(Vec<Span>, SpanMap), String> {
    let id = target.id;
    if text(inventory, "state") != "pending" {
        super::audit::authenticate(root, target, inventory).map_err(|reason| {
            format!("Full-C Byte Share withheld: the {id} executable inventory is not the independently verified automatic count: {reason}")
        })?;
    }
    inventory_intervals(inventory, id.as_str())
}

/// An inventory's intervals when it is complete and consistent in itself.
fn inventory_intervals(inventory: &Value, target: &str) -> Result<(Vec<Span>, SpanMap), String> {
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

/// A target's generated executable inventory when it is authoritative, or
/// `None` while it is absent or withheld: pending its audit, or not the
/// independently verified automatic count. A verified inventory that is
/// inconsistent in itself is an error, never a reason to skip.
pub(crate) fn authoritative_inventory(
    root: &Path,
    target: DecompTarget,
) -> Result<Option<Value>, String> {
    let path = format!("{}/reports/executable.json", target.output_dir);
    let text = match std::fs::read(root.join(&path)) {
        Ok(text) => text,
        Err(error) if error.kind() == std::io::ErrorKind::NotFound => return Ok(None),
        Err(error) => return Err(format!("{path}: {error}")),
    };
    let inventory: Value =
        serde_json::from_slice(&text).map_err(|error| format!("{path}: {error}"))?;
    match validated_inventory(root, &inventory, target) {
        Ok(_) => Ok(Some(inventory)),
        Err(error) if error.contains("withheld") => Ok(None),
        Err(error) => Err(format!("{path}: {error}")),
    }
}

/// A target's generated executable inventory in a checkout, with the
/// intervals [`validated_inventory`] admits. A revision has none.
#[cfg(test)]
fn read_inventory(
    tree: &SourceTree,
    target: DecompTarget,
) -> Result<(Value, Vec<Span>, SpanMap), String> {
    let SourceTree::Work { root, .. } = tree else {
        return Err(format!(
            "{}: a revision has no generated executable inventory",
            tree.id()
        ));
    };
    let inventory = read_json(
        tree,
        &format!("{}/reports/executable.json", target.output_dir),
    )?;
    let (main, overlays) = validated_inventory(root, &inventory, target)?;
    Ok((inventory, main, overlays))
}

fn candidate_main(
    tree: &SourceTree,
    target: &DecompTarget,
    _executable: &[Span],
) -> (Vec<Span>, usize) {
    let directory = format!("{}/en/main", target.recon_dir());
    let mut sources = 0;
    for name in tree.list(&directory) {
        let Some(_) = name.strip_suffix(".c").or_else(|| name.strip_suffix(".C")) else {
            continue;
        };
        if !tree
            .read(&format!("{directory}/{name}"))
            .is_some_and(|s| canonical(&s))
        {
            continue;
        }
        sources += 1;
    }
    (Vec::new(), sources)
}
fn candidate_overlay(
    tree: &SourceTree,
    target: &DecompTarget,
    executable: &SpanMap,
) -> (SpanMap, usize) {
    let directory = format!("{}/en/overlays", target.recon_dir());
    let reviewed = json(
        tree,
        &format!("{}/semantic/regions.json", target.recon_dir()),
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
        &format!("{}/translation-units.json", target.recon_dir()),
    )
    .unwrap_or(Value::Null);
    let mut registered = BTreeSet::new();
    for unit in array(&units, "units") {
        let source = text(unit, "source");
        registered.insert(source.clone());
        let id = text(unit, "overlay");
        if !executable.contains_key(&id)
            || !source.starts_with(&format!("{directory}/"))
            || !(source.ends_with(".c") || source.ends_with(".C"))
            || !tree.read(&source).is_some_and(|code| canonical(&code))
        {
            continue;
        }
        for owner in array(unit, "owners") {
            let Some(entry) = address(owner, "address") else {
                continue;
            };
            let Some(size) = extents.get(&(id.clone(), entry)).copied() else {
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
        let Some(size) = extents.get(&(id.into(), entry)).copied() else {
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
    let manifest_path = format!("{}/source-paths.json", target.recon_dir());
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
        let units_path = format!("{}/translation-units.json", target.recon_dir());
        if let Some(units) = json(tree, &units_path) {
            // Each placeholder already credits its owner once, in its own
            // overlay; an instance adds only the fill it declares there.
            let linked = array(&units, "units")
                .iter()
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
    // Each game's registry places the container library its own link used.
    let registry_path = crate::compiler::runtime::registry_path(target.compiler);
    let Some(registry) = json(tree, &registry_path) else {
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
                text(region, "source") == registry_path
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
        let start = address(link, "text")
            .ok_or_else(|| format!("{registry_path}: invalid runtime link"))?;
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

/// A maintained SRC module whose header declares library, handwritten or
/// reconstructed-veneer provenance, with the byte-comparison evidence the
/// assembly build adds only when the module reproduces the ROM. The build
/// admits a reconstructed veneer only as a table of whole far-call veneers
/// built from the shared veneer macro (Pascal, 2026-09-24).
fn maintained_assembly_credit(region: &Value, target: &DecompTarget) -> bool {
    let source = text(region, "source");
    let provenance = &region["provenance"];
    let source_root = format!("{}/SRC/", target.game_dir());
    text(region, "kind") == "maintained_assembly"
        && source.starts_with(&source_root)
        && matches!(
            text(provenance, "credit").as_str(),
            "handwritten" | "library" | "reconstructed_veneer"
        )
        && array(provenance, "evidence")
            .iter()
            .any(|item| item.as_str().is_some_and(|item| !item.trim().is_empty()))
        && (!text(provenance, "proof").trim().is_empty()
            || !text(provenance, "object").trim().is_empty())
}

/// Each credited maintained module of an assembly manifest built against the
/// ROM, with its source: the complete assembled extent less the spans the
/// module marks uncredited (padding, never-run filler, placeholder slots and
/// words rewritten at run time), in ascending ranges. Both games read their
/// assembly credit through this one rule.
pub(crate) fn maintained_assembly_credits(
    manifest: &Value,
    target: &DecompTarget,
) -> Vec<(Span, String)> {
    if text(manifest, "verification") != "rom" {
        return Vec::new();
    }
    array(manifest, "regions")
        .iter()
        .filter(|region| maintained_assembly_credit(region, target))
        .flat_map(|region| {
            let extent = integer(region, "address")
                .zip(integer(region, "size"))
                .map(|(address, size)| Span::new(address, address + size));
            let cuts = array(region, "uncredited")
                .iter()
                .filter_map(|span| {
                    let (address, size) = (integer(span, "address")?, integer(span, "size")?);
                    Some(Span::new(address, address + size))
                })
                .collect::<Vec<_>>();
            let source = text(region, "source");
            extent
                .map(|extent| subtract(&[extent], &cuts))
                .unwrap_or_default()
                .into_iter()
                .map(move |span| (span, source.clone()))
        })
        .collect()
}

/// Refuses a maintained module whose credited extent misses the audited main
/// executable intervals entirely: its declared credit would count nothing, so
/// either the module or the inventory is wrong.
fn refuse_unaudited_module_credit(
    tree: &SourceTree,
    target: &DecompTarget,
    main_exec: &[Span],
) -> Result<(), String> {
    let Some(manifest) = json(
        tree,
        &format!("{}/full/asm/manifest.json", target.output_dir),
    ) else {
        return Ok(());
    };
    match maintained_assembly_credits(&manifest, target)
        .into_iter()
        .find(|(span, _)| intersect(&[*span], main_exec).is_empty())
    {
        Some((_, source)) => Err(format!(
            "{source} lies outside audited executable intervals"
        )),
        None => Ok(()),
    }
}

/// Proven retained and draft main assembly. Only byte-verified maintained
/// source with its own authorship or library provenance earns completion credit.
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
        credited.extend(
            maintained_assembly_credits(&value, target)
                .into_iter()
                .map(|(span, _)| span),
        );
        for region in array(&value, "regions") {
            if text(region, "kind") == "maintained_assembly" {
                continue;
            }
            let retention = text(region, "retention");
            let kind = text(region, "kind");
            let evidence = text(region, "evidence");
            let classified = retention == "keep_asm"
                || retention == "keep_structured_asm"
                || retention == "not_yet_c"
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
                    } else {
                        proven.push(span);
                    }
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
    let classification_path = format!("{}/semantic/overlay-assembly.json", target.recon_dir());
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
                                && matches!(region.kind.as_str(), "veneer" | "executable_alignment")
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
        if !matches!(
            text(row, "retention").as_str(),
            "keep_structured_asm" | "not_yet_c"
        ) || evidence.is_empty()
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
        let credit = text(&row["provenance"], "credit");
        if credit == "reconstructed_veneer" {
            let veneer_spans = mapped(inventory, &overlay)
                .iter()
                .filter(|region| region.kind == "veneer" && !region.evidence.trim().is_empty())
                .map(|region| region.span)
                .collect::<Vec<_>>();
            if text(row, "confidence") != "proven"
                || text(row, "kind") != "overlay_trampoline"
                || text(&row["provenance"], "proof") != veneer_macro
                || !text(&row["provenance"], "source").starts_with(&format!("{source_dir}/"))
                || span.start % 4 != 0
                || span.bytes() % 8 != 0
                || bytes(&intersect(&[span], &veneer_spans)) != span.bytes()
            {
                return Err(format!(
                    "assembly classification {index} has invalid reconstructed overlay-trampoline credit"
                ));
            }
            credited.entry(overlay).or_default().push(span);
        } else if !credit.is_empty() {
            return Err(format!(
                "assembly classification {index} requests unsupported generic assembly credit"
            ));
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
/// Each field overlay's source directory, as `{game_dir}/SRC/.../`.
fn atlas_sources(tree: &SourceTree, target: &DecompTarget) -> BTreeMap<i64, String> {
    super::places::places(tree, target)
        .unwrap_or_default()
        .into_iter()
        .filter_map(|place| {
            let home = place.home?;
            Some((
                place.overlay as i64,
                format!("{}/{home}/", target.game_dir()),
            ))
        })
        .collect()
}
fn streams(tree: &SourceTree, target: &DecompTarget) -> Vec<Stream> {
    let Some(manifest) = json(tree, target.asset_manifest) else {
        return Vec::new();
    };
    let mut out = Vec::new();
    let sources = atlas_sources(tree, target);
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
                let source = hex(&id).and_then(|id| sources.get(&id)).cloned();
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
    let sources = atlas_sources(
        tree,
        &crate::targets::target_for(crate::targets::DecompTargetId::TbsEn),
    );
    let scenes = array(&scenes, "segments")
        .first()
        .ok_or("missing Atlas scene table")?;
    let mut users: BTreeMap<i64, BTreeSet<String>> = BTreeMap::new();
    // The loader indexes 201 scene records into the map loading table.
    for scene in array(scenes, "records").iter().take(201) {
        let resource = integer(scene, "resource_id").ok_or("invalid scene resource")?;
        let Some(area) = sources.get(&resource).cloned() else {
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
    let inventory = json(tree, "out/tla-en/reports/executable.json").unwrap_or(Value::Null);
    let evidence = inventory.get("evidence").and_then(Value::as_object);
    let executable: Vec<Span> = regions(&inventory["main"], evidence)
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
                source: Some("recon/tla/raw/main.s".into()),
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
            let source = format!("recon/tla/raw/overlays/{id}_overlay.s");
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
        "percent_of_executable": crate::coverage::jsnum::floor_percent(bytes, total)
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
/// The overlay images that carry credited assembly, found without the
/// executable inventory: the classification's reconstructed veneers and the
/// registry's overlay runtime links placed in their listings. Comparing these
/// images with the ROM needs no denominator; crediting their spans does, and
/// only a full build's receipt credits them ([`verified_credits`]).
pub(crate) fn overlay_assembly_images(
    tree: &SourceTree,
    target: &DecompTarget,
) -> Result<BTreeSet<String>, String> {
    let classification_path = format!("{}/semantic/overlay-assembly.json", target.recon_dir());
    let source = tree
        .read(&classification_path)
        .ok_or_else(|| "overlay assembly classification is missing".to_string())?;
    let document: Value =
        serde_json::from_str(&source).map_err(|error| format!("{classification_path}: {error}"))?;
    let mut images = array(&document, "regions")
        .iter()
        .filter(|row| text(&row["provenance"], "credit") == "reconstructed_veneer")
        .map(|row| text(row, "overlay"))
        .collect::<BTreeSet<_>>();
    let registry_path = crate::compiler::runtime::registry_path(target.compiler);
    let Some(registry) = json(tree, &registry_path) else {
        return Ok(images);
    };
    for link in array(&registry, "links") {
        let image = text(link, "image");
        if image == "main" {
            continue;
        }
        let start = address(link, "text")
            .ok_or_else(|| format!("{registry_path}: invalid runtime link"))?;
        let listing = tree
            .read(&target.overlay_assembly(&image))
            .unwrap_or_default();
        if crate::compiler::runtime::listing_windows(&listing)?
            .into_iter()
            .any(|(window, _)| i64::from(window) == start)
        {
            images.insert(image);
        }
    }
    Ok(images)
}

pub fn classify(options: &BuildOptions) -> Result<Classification, String> {
    let target = crate::targets::decomp_target(Some(&options.target))?;
    // Classifying owners needs only where the code lies, not a fresh proof of
    // DONE: an adoption changes inputs before its twin check runs, so the
    // inventory must be complete and consistent, not re-authenticated.
    let SourceTree::Work { .. } = options.exact else {
        return Err(format!(
            "{}: a revision has no generated executable inventory",
            options.exact.id()
        ));
    };
    let inventory = read_json(
        options.exact,
        &format!("{}/reports/executable.json", target.output_dir),
    )?;
    inventory_intervals(&inventory, target.id.as_str())?;
    let evidence = inventory.get("evidence").and_then(Value::as_object);
    let main = regions(&inventory["main"], evidence);
    let main_exec = normalize(&main.iter().map(|r| r.span).collect::<Vec<_>>());
    let mut overlay_exec = SpanMap::new();
    let mut overlay_regions = BTreeMap::new();
    for node in array(&inventory, "overlays") {
        let id = text(node, "id");
        let rows = regions(node, evidence);
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
    refuse_unaudited_module_credit(options.exact, &target, &main_exec)?;
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
    // The Lost Age scores only once its own full build proves an inventory;
    // until then it is absent and readers show it as pending.
    if options.target == "tbs-en" {
        if let Some(tla) = super::progress::measured(root, "tla-en")? {
            game_scores.insert("tla-en".into(), serde_json::to_value(tla).unwrap());
        }
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
        // A withheld receipt, while The Lost Age awaits its inventory, shows
        // no verified code; a receipt that exists must still be current.
        let withheld = !root.join("out/tla-en/reports/verified-code.json").exists();
        let credits = if withheld {
            Vec::new()
        } else {
            super::proof::read(root, "tla-en")?.credits
        };
        rom_areas.push(area(
            "rom-lost-age",
            "The Lost Age ROM",
            lost_age_tiles(options.exact, &credits),
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
            "proven_assembly_standard": "handwritten-or-library-proven; audited-overlay-veneer-reconstruction; main-far-call-veneer-tables; container-built-compiler-runtime",
            "credited_assembly_bytes": bytes(&retained_main) + mapped_bytes(&retained_overlay),
            "withdrawn_assembly_bytes": withdrawn_assembly,
            "main_assembly_classification": format!("{}/full/asm/manifest.json", target.output_dir),
            "overlay_assembly_classification": format!("{}/semantic/overlay-assembly.json", target.recon_dir()),
            "draft_superseded_bytes": 0,
            "draft_outside_extent_bytes": 0,
            "draft_unresolved": []
        },
        "rom_areas": rom_areas.iter().map(area_json).collect::<Vec<_>>(),
        "executable_areas": executable_areas.iter().map(area_json).collect::<Vec<_>>()
    });
    Ok(CoverageMap {
        document,
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
                "executable_bytes": 4,
                "intervals": [{"start": 0x02000000, "end": 0x02000004}]
            }]
        })
    }

    #[test]
    fn executable_inventory_requires_a_complete_partition_before_scoring() {
        let inventory = inventory_fixture();
        let (main, overlays) = inventory_intervals(&inventory, "tla-en").unwrap();
        assert_eq!(bytes(&main), 4);
        assert_eq!(mapped_bytes(&overlays), 4);

        let mut pending = inventory.clone();
        pending["state"] = json!("pending");
        pending["audit"] = json!("incomplete");
        assert!(inventory_intervals(&pending, "tla-en")
            .unwrap_err()
            .contains("withheld"));

        let mut contradictory = inventory.clone();
        contradictory["state"] = json!("pending");
        assert!(inventory_intervals(&contradictory, "tla-en")
            .unwrap_err()
            .contains("state and audit disagree"));

        let mut missing_overlay = inventory.clone();
        missing_overlay["overlay_count"] = json!(2);
        assert!(inventory_intervals(&missing_overlay, "tla-en")
            .unwrap_err()
            .contains("overlay count"));

        let mut outside_decoded = inventory;
        outside_decoded["overlays"][0]["executable_bytes"] = json!(12);
        outside_decoded["overlays"][0]["intervals"] = json!([
            {"start": 0x02000000, "end": 0x0200000c}
        ]);
        assert!(inventory_intervals(&outside_decoded, "tla-en")
            .unwrap_err()
            .contains("outside its decoded image"));
    }

    #[test]
    fn malformed_audit_ranges_are_not_silently_dropped() {
        let mut inventory = inventory_fixture();
        inventory["main"]["intervals"]
            .as_array_mut()
            .unwrap()
            .push(json!({"start": 12, "end": 8}));
        assert!(inventory_intervals(&inventory, "tla-en").is_err());
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
            "recon/tbs/overlay-homes.tsv",
            "resource_3a0\tSRC/FIELD/XIAN\n".into(),
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
    /// The Lost Age's receipt credits its reconstructed overlay veneers from
    /// its own full build and inventory, exactly as The Broken Seal's does,
    /// and an earlier receipt has no say in what it credits.
    #[test]
    fn lost_age_full_build_credits_its_veneers_without_an_earlier_receipt() {
        let directory = tempfile::tempdir().unwrap();
        let write = |path: &str, value: Value| {
            let path = directory.path().join(path);
            std::fs::create_dir_all(path.parent().unwrap()).unwrap();
            std::fs::write(path, serde_json::to_vec(&value).unwrap()).unwrap();
        };
        let target = crate::targets::decomp_target(Some("tla-en")).unwrap();
        let mut row = region(
            "0x02000120",
            "0x02000140",
            "proven",
            json!(["fixture proof"]),
        );
        row["kind"] = json!("overlay_trampoline");
        row["provenance"] = json!({
            "credit":"reconstructed_veneer",
            "proof":"games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC",
            "source":"games/THE LOST AGE/SRC/FIELD/TEST/ENTRY.INC"
        });
        write(
            "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC",
            json!("fixture"),
        );
        write(
            "games/THE LOST AGE/SRC/FIELD/TEST/ENTRY.INC",
            json!("fixture"),
        );
        write(
            "recon/tla/semantic/overlay-assembly.json",
            classification(json!([row])),
        );
        write(
            "recon/tla/source-paths.json",
            json!({"format": 3, "owners": {}}),
        );
        crate::coverage::audit::authoritative_fixture(
            directory.path(),
            target,
            &[],
            json!([{"id": "resource_test", "decoded_bytes": 0x140, "intervals": [
                {"start": 0x02000120, "end": 0x02000140, "kind": "veneer"}
            ]}]),
        );
        // The full build's claimed and assembly stages, which place no main
        // code in this fixture.
        write(
            "out/tla-en/full/claimed/manifest.json",
            json!({"regions": []}),
        );
        write(
            "out/tla-en/full/asm/manifest.json",
            json!({"verification": "rom", "regions": []}),
        );
        let tree = crate::coverage::tree::work_tree_at(directory.path().to_path_buf());
        let credits = || {
            verified_credits(&BuildOptions {
                target: "tla-en".into(),
                exact: &tree,
                recon: None,
            })
            .unwrap()
            .into_iter()
            .map(|credit| {
                (
                    credit.image,
                    credit.start,
                    credit.end,
                    credit.source,
                    credit.kind,
                )
            })
            .collect::<Vec<_>>()
        };
        let expected = [(
            "resource_test".to_string(),
            0x0200_0120,
            0x0200_0140,
            "recon/tla/raw/overlays/resource_test_overlay.s".to_string(),
            "assembly".to_string(),
        )];
        assert_eq!(credits(), expected);
        write(
            "out/tla-en/reports/verified-code.json",
            json!({"credits": []}),
        );
        assert_eq!(credits(), expected);
    }
    #[test]
    fn generic_main_assembly_provenance_earns_no_credit() {
        let directory = tempfile::tempdir().unwrap();
        let write = |path: &str, value: Value| {
            let path = directory.path().join(path);
            std::fs::create_dir_all(path.parent().unwrap()).unwrap();
            std::fs::write(path, serde_json::to_vec(&value).unwrap()).unwrap();
        };
        let row = |address, provenance| {
            json!({
                "source": format!("recon/tbs/raw/{address:08x}.s"),
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
        assert!(credited.is_empty());
        assert_eq!(
            proven,
            [
                Span::new(0x0800_0100, 0x0800_0108),
                Span::new(0x0800_0120, 0x0800_0128),
                Span::new(0x0800_0140, 0x0800_0148),
                Span::new(0x0800_0160, 0x0800_0168),
                Span::new(0x0800_0180, 0x0800_0188)
            ]
        );
    }

    #[test]
    fn verified_maintained_modules_earn_their_declared_credit() {
        let directory = tempfile::tempdir().unwrap();
        let module = |address: i64, source: &str, credit: Value| {
            json!({
                "source": source,
                "address": address,
                "size": 16,
                "kind": "maintained_assembly",
                "retention": "keep_asm",
                "confidence": "verified",
                "evidence": "",
                "provenance": credit
            })
        };
        let declared = |evidence: &[&str]| {
            json!({"credit": "handwritten", "proof": "software interrupts have no C form",
                   "object": "bios wrapper", "evidence": evidence})
        };
        let manifest = |verification: &str| {
            json!({"verification": verification, "regions": [
                module(0x0800_0100, "games/THE BROKEN SEAL/SRC/SYSTEM/BIOS/A.S",
                       declared(&["maintained source reproduces its ROM bytes"])),
                // Declared, but assembled without a ROM comparison.
                module(0x0800_0200, "games/THE BROKEN SEAL/SRC/SYSTEM/BIOS/B.S", declared(&[])),
                // Maintained but undeclared: kept, never credited.
                module(0x0800_0300, "games/THE BROKEN SEAL/SRC/SYSTEM/IWRAM/C.S", Value::Null),
                // Raw listings cannot carry maintained credit.
                module(0x0800_0400, "recon/tbs/raw/08000400.s",
                       declared(&["maintained source reproduces its ROM bytes"])),
                // Another game's source does not credit this one.
                module(0x0800_0500, "games/THE LOST AGE/SRC/SYSTEM/D.S",
                       declared(&["maintained source reproduces its ROM bytes"])),
            ]})
        };
        let target = crate::targets::target_for(crate::targets::DEFAULT_TARGET);
        assert_eq!(
            maintained_assembly_credits(&manifest("rom"), &target),
            [(
                Span::new(0x0800_0100, 0x0800_0110),
                "games/THE BROKEN SEAL/SRC/SYSTEM/BIOS/A.S".to_string()
            )]
        );
        assert!(maintained_assembly_credits(&manifest("source_only"), &target).is_empty());
        let path = directory.path().join("out/tbs-en/full/asm/manifest.json");
        std::fs::create_dir_all(path.parent().unwrap()).unwrap();
        std::fs::write(&path, serde_json::to_vec(&manifest("rom")).unwrap()).unwrap();
        let tree = crate::coverage::tree::work_tree_at(directory.path().into());
        let (proven, draft, credited) = main_assembly_classification(&tree);
        assert!(proven.is_empty() && draft.is_empty());
        assert_eq!(credited, [Span::new(0x0800_0100, 0x0800_0110)]);
    }

    #[test]
    fn marked_uncredited_spans_leave_a_module_credit() {
        let source = "games/THE BROKEN SEAL/SRC/SYSTEM/A.S";
        let manifest = json!({"verification": "rom", "regions": [{
            "source": source, "address": 0x0800_0100, "size": 0x20,
            "kind": "maintained_assembly",
            "provenance": {"credit": "library", "proof": "shared object", "object": "a",
                           "evidence": ["maintained source reproduces its ROM bytes"]},
            "uncredited": [{"address": 0x0800_0100, "size": 4},
                           {"address": 0x0800_0110, "size": 2},
                           {"address": 0x0800_011c, "size": 4}]
        }]});
        let target = crate::targets::target_for(crate::targets::DEFAULT_TARGET);
        assert_eq!(
            maintained_assembly_credits(&manifest, &target),
            [
                (Span::new(0x0800_0104, 0x0800_0110), source.to_string()),
                (Span::new(0x0800_0112, 0x0800_011c), source.to_string())
            ]
        );
    }

    #[test]
    fn runtime_credit_stays_separate_from_range_credits() {
        // No tracked assembly carries the compiler runtime: the call_via bank
        // at 0x080072e4 is a container-built link, credited exactly where the
        // built manifest placed it.
        // Both inputs are generated: the executable inventory by `alchemy
        // coverage audit --inventory` and the manifest by the full build.
        let root = crate::compiler::routing::root();
        let manifest = "out/tbs-en/full/asm/manifest.json";
        let tbs = crate::targets::target_for(crate::targets::DEFAULT_TARGET);
        let Some(inventory) = authoritative_inventory(root, tbs).unwrap() else {
            eprintln!("skipped: out/tbs-en/reports/executable.json is absent or pending, so no audited overlay intervals exist");
            return;
        };
        if !root.join(manifest).is_file() {
            eprintln!("skipped: {manifest} is absent; the full TBS build generates it");
            return;
        }
        let tree = crate::coverage::tree::work_tree();
        let rom = [Span::new(0x0800_0000, 0x0880_0000)];
        let (_, overlays) = validated_inventory(root, &inventory, tbs).unwrap();
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
        let registry =
            crate::compiler::runtime::registry_path(crate::compiler::routing::CompilerTarget::Tbs);
        write(
            &registry,
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
                {"address": 0x0800_72e4, "size": 60, "source": registry, "retention": "container_runtime"},
                {"address": 0x0800_1000, "size": 60, "source": "recon/tbs/raw/08001000.s", "retention": "keep_asm"}
            ]})
            .to_string(),
        );
        write(
            "recon/tbs/raw/overlays/resource_3bf_overlay.s",
            "AlchemyRuntime_020057b0:\n\t.space 0x728\n\t.4byte 1\nAlchemyRuntime_02005f90:\n\t.space 0x14\n".into(),
        );
        write(
            "recon/tbs/raw/overlays/resource_373_overlay.s",
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
    #[test]
    fn overlay_assembly_images_are_named_without_the_inventory() {
        let directory = tempfile::tempdir().unwrap();
        let root = directory.path();
        let write = |path: &str, text: String| {
            let path = root.join(path);
            std::fs::create_dir_all(path.parent().unwrap()).unwrap();
            std::fs::write(path, text).unwrap();
        };
        let target = crate::targets::target_for(crate::targets::DEFAULT_TARGET);
        write(
            &format!("{}/semantic/overlay-assembly.json", target.recon_dir()),
            json!({"format": 1, "regions": [
                {"overlay": "resource_370", "provenance": {"credit": "reconstructed_veneer"}},
                {"overlay": "resource_371", "provenance": {}}
            ]})
            .to_string(),
        );
        write(
            &crate::compiler::runtime::registry_path(target.compiler),
            json!({"links": [
                {"image": "main", "text": "0x080072e4", "members": ["_m"]},
                {"image": "resource_3bf", "text": "0x020057b0", "members": ["_m"]},
                {"image": "resource_373", "text": "0x02006154", "members": ["_m"]}
            ]})
            .to_string(),
        );
        write(
            &target.overlay_assembly("resource_3bf"),
            "AlchemyRuntime_020057b0:\n\t.space 0x728\n".into(),
        );
        write(
            &target.overlay_assembly("resource_373"),
            "\t.4byte 0\n".into(),
        );
        let tree = crate::coverage::tree::work_tree_at(root.to_path_buf());
        assert_eq!(
            overlay_assembly_images(&tree, &target).unwrap(),
            BTreeSet::from(["resource_370".to_string(), "resource_3bf".to_string()])
        );
    }
    /// The Broken Seal, whose full ROM build is byte-identical.
    fn tbs_en() -> DecompTarget {
        crate::targets::target_for(crate::targets::DecompTargetId::TbsEn)
    }

    /// An authoritative inventory written as `--inventory` writes it, for
    /// tests to break: the main image 0x08000100..0x08000104 and one overlay.
    fn authoritative_fixture(root: &Path) -> Value {
        crate::coverage::audit::authoritative_fixture(
            root,
            tbs_en(),
            &[(0x0800_0100, 0x0800_0104)],
            json!([{"id": "resource_test", "decoded_bytes": 8, "intervals": [
                {"start": 0x0200_0000, "end": 0x0200_0004, "kind": "thumb"}
            ]}]),
        )
        .1
    }

    /// Writes `inventory` as the TBS inventory. When every reader refuses to
    /// score it, treating it as pending, returns why; `None` when scored.
    fn withheld_by_every_reader(root: &Path, inventory: &Value) -> Option<String> {
        let path = root.join("out/tbs-en/reports/executable.json");
        std::fs::write(&path, inventory.to_string()).unwrap();
        let tree = crate::coverage::tree::work_tree_at(root.to_path_buf());
        let authoritative = authoritative_inventory(root, tbs_en()).unwrap().is_some();
        let intervals = read_inventory(&tree, tbs_en());
        assert_eq!(authoritative, intervals.is_ok());
        if authoritative {
            return None;
        }
        assert!(super::super::progress::measured(root, "tbs-en")
            .unwrap()
            .is_none());
        let reason = intervals.unwrap_err();
        assert!(reason.contains("withheld"), "{reason}");
        Some(reason)
    }

    /// Asserts that every reader withholds `inventory` for `reason`.
    fn withheld_for(root: &Path, inventory: &Value, reason: &str) {
        let found = withheld_by_every_reader(root, inventory).expect("scored");
        assert!(found.contains(reason), "{found}");
    }

    #[test]
    fn only_a_validated_inventory_is_authoritative() {
        let directory = tempfile::tempdir().unwrap();
        let root = directory.path();
        assert!(authoritative_inventory(root, tbs_en()).unwrap().is_none());
        // An absent inventory is pending, never an error.
        assert!(super::super::progress::measured(root, "tbs-en")
            .unwrap()
            .is_none());
        let genuine = authoritative_fixture(root);
        assert!(authoritative_inventory(root, tbs_en()).unwrap().is_some());
        let pending = json!({
            "format": 1, "metric": "full-c-byte-share", "target": "tbs-en",
            "state": "pending", "audit": "incomplete", "pending": ["main unproven"],
            "overlay_count": 0, "main": {"id": "main", "audit": "incomplete"}, "overlays": []
        });
        withheld_for(root, &pending, "executable audit is incomplete");
        // A verified inventory inconsistent in itself is an error.
        let mut stale = genuine;
        stale["total_union_bytes"] = json!(9);
        let path = root.join("out/tbs-en/reports/executable.json");
        std::fs::write(path, stale.to_string()).unwrap();
        assert!(authoritative_inventory(root, tbs_en())
            .unwrap_err()
            .contains("stale"));
    }

    /// Every reader recomputes an inventory's provenance: a well-formed
    /// inventory is scored only while its overlay intervals hash to the
    /// verified digest and its main intervals are the complement of the
    /// byte-identical full build it records.
    #[test]
    fn copied_tampered_or_unproven_inventories_are_never_scored() {
        let directory = tempfile::tempdir().unwrap();
        let root = directory.path();
        let genuine = authoritative_fixture(root);
        assert_eq!(withheld_by_every_reader(root, &genuine), None);

        // Hand-made: consistent in itself, with no provenance.
        let mut hand_made = inventory_fixture();
        hand_made["target"] = json!("tbs-en");
        assert!(inventory_intervals(&hand_made, "tbs-en").is_ok());
        withheld_for(
            root,
            &hand_made,
            "not the independently verified automatic count",
        );
        // Carrying the genuine provenance and overlay intervals, its main
        // image is still not the proven asset complement.
        hand_made["verification"] = genuine["verification"].clone();
        hand_made["overlays"][0]["intervals"] = genuine["overlays"][0]["intervals"].clone();
        hand_made["main"]["intervals"][0]["kind"] = json!("thumb");
        withheld_for(
            root,
            &hand_made,
            "audits a ROM other than the one the full build reproduced",
        );
        hand_made["rom_sha256"] = genuine["rom_sha256"].clone();
        withheld_for(
            root,
            &hand_made,
            "main intervals are not the asset complement",
        );

        // A tampered overlay interval, its totals kept consistent.
        let mut tampered = genuine.clone();
        tampered["overlays"][0]["intervals"][0]["end"] = json!(0x0200_0008);
        tampered["overlays"][0]["executable_bytes"] = json!(8);
        tampered["total_union_bytes"] = json!(12);
        assert!(inventory_intervals(&tampered, "tbs-en").is_ok());
        withheld_for(root, &tampered, "its overlay intervals hash to");
        // A tampered interval kind.
        let mut tampered = genuine.clone();
        tampered["overlays"][0]["intervals"][0]["kind"] = json!("veneer");
        withheld_for(root, &tampered, "its overlay intervals hash to");
        // A tampered main interval.
        let mut tampered = genuine.clone();
        tampered["main"]["intervals"][0]["end"] = json!(0x0800_0108);
        tampered["main"]["executable_bytes"] = json!(8);
        tampered["total_union_bytes"] = json!(12);
        assert!(inventory_intervals(&tampered, "tbs-en").is_ok());
        withheld_for(
            root,
            &tampered,
            "main intervals are not the asset complement",
        );
        // A decoded size the digest does not cover.
        let mut tampered = genuine.clone();
        tampered["overlays"][0]["decoded_bytes"] = json!(16);
        withheld_for(root, &tampered, "recorded totals");
        // A recorded digest the intervals do not have.
        let mut tampered = genuine.clone();
        tampered["verification"]["overlay_sha256"] = json!("0".repeat(64));
        withheld_for(root, &tampered, "does not record the provenance");

        // A missing main-image proof.
        let mut unproven = genuine.clone();
        unproven["verification"]
            .as_object_mut()
            .unwrap()
            .remove("main");
        withheld_for(root, &unproven, "records no main-image proof");
        let mut unproven = genuine.clone();
        unproven["verification"]["main"] = json!({"state": "unproven"});
        withheld_for(root, &unproven, "records no main-image proof");
        // A recorded proof the last full build no longer supports.
        let report = root.join("out/tbs-en/full/rebuilt.json");
        let built = std::fs::read(&report).unwrap();
        std::fs::write(&report, r#"{"byte_identical":false}"#).unwrap();
        withheld_for(root, &genuine, "main-image proof no longer holds");
        std::fs::remove_file(&report).unwrap();
        withheld_for(root, &genuine, "main-image proof no longer holds");
        // A hand-written report with every flag a byte-identical build sets,
        // but no proof the readers can recompute.
        std::fs::write(
            &report,
            r#"{"format":1,"target":"tbs-en","verification":"rom","byte_identical":true,"unowned_bytes":0,"rom_fallback_bytes":0}"#,
        )
        .unwrap();
        withheld_for(root, &genuine, "records no main-image proof");
        std::fs::write(&report, &built).unwrap();
        assert_eq!(withheld_by_every_reader(root, &genuine), None);
        // A rebuilt ROM that is not the reference ROM.
        let rebuilt = root.join("out/tbs-en/full/rebuilt.gba");
        let rom = std::fs::read(&rebuilt).unwrap();
        std::fs::write(&rebuilt, b"another ROM").unwrap();
        withheld_for(root, &genuine, "is not the ROM the build recorded");
        std::fs::write(&rebuilt, &rom).unwrap();
        // An asset manifest changed after the build, even to one that would
        // leave the same complement.
        let manifest = root.join("out/tbs-en/full/assets/manifest.json");
        let assets = std::fs::read(&manifest).unwrap();
        std::fs::write(&manifest, r#"{"regions": []}"#).unwrap();
        withheld_for(
            root,
            &genuine,
            "is not the asset manifest the build recorded",
        );
        let mut reordered: Value = serde_json::from_slice(&assets).unwrap();
        reordered["regions"].as_array_mut().unwrap().reverse();
        std::fs::write(&manifest, reordered.to_string()).unwrap();
        withheld_for(
            root,
            &genuine,
            "is not the asset manifest the build recorded",
        );
        std::fs::remove_file(&manifest).unwrap();
        withheld_for(root, &genuine, "full/assets/manifest.json");
        std::fs::write(&manifest, &assets).unwrap();
        assert_eq!(withheld_by_every_reader(root, &genuine), None);
        // An encoder changed after the build: the build no longer proves the
        // tree, so no receipt can credit it, but the count of the reference
        // ROM it laid out stays authoritative.
        let encoder = root.join("tools/alchemy/src/build_assets/packer.rs");
        std::fs::create_dir_all(encoder.parent().unwrap()).unwrap();
        std::fs::write(&encoder, "fn pack() {}").unwrap();
        assert_eq!(withheld_by_every_reader(root, &genuine), None);
        let stale = crate::coverage::proof::full_build(root, tbs_en()).map(|_| ());
        assert!(stale
            .unwrap_err()
            .contains("build inputs changed after the build"));
        assert!(super::super::progress::measured(root, "tbs-en")
            .unwrap()
            .is_none());
        std::fs::remove_file(&encoder).unwrap();
        assert_eq!(withheld_by_every_reader(root, &genuine), None);
        // The verification record withdrawn.
        let record = root.join("recon/tbs/metrics/audit-verification.json");
        std::fs::remove_file(&record).unwrap();
        withheld_for(root, &genuine, "no independent verification");
    }

    /// The exploit an adversarial verification found: a hand-kept ledger
    /// copied into a game's inventory path, beside that game's real
    /// verification record and a byte-identical full build, earns no score.
    /// The ledger here is the genuine count as a person would have edited it:
    /// promoted to `verified`, stripped of its provenance and reclassified.
    #[test]
    fn a_copied_ledger_is_never_scored() {
        use crate::targets::DecompTargetId::{TbsEn, TlaEn};
        for id in [TbsEn, TlaEn] {
            let game_target = crate::targets::target_for(id);
            let target = id.as_str();
            let directory = tempfile::tempdir().unwrap();
            let root = directory.path();
            let (_, genuine) = crate::coverage::audit::authoritative_fixture(
                root,
                game_target,
                &[(0x0800_0100, 0x0800_0104)],
                json!([{"id": "resource_test", "decoded_bytes": 8, "intervals": [
                    {"start": 0x0200_0000, "end": 0x0200_0004, "kind": "thumb"}
                ]}]),
            );
            assert!(
                authoritative_inventory(root, game_target)
                    .unwrap()
                    .is_some(),
                "{target}"
            );
            let mut ledger = genuine;
            ledger["state"] = json!("verified");
            ledger.as_object_mut().unwrap().remove("verification");
            ledger["overlays"][0]["intervals"][0]["kind"] = json!("hand_written_thumb");
            let path = root.join(format!("out/{target}/reports/executable.json"));
            std::fs::write(&path, ledger.to_string()).unwrap();
            assert!(
                authoritative_inventory(root, game_target)
                    .unwrap()
                    .is_none(),
                "{target}"
            );
            assert!(
                super::super::progress::measured(root, target)
                    .unwrap()
                    .is_none(),
                "{target}"
            );
            let tree = crate::coverage::tree::work_tree_at(root.to_path_buf());
            let reason = read_inventory(&tree, game_target).unwrap_err();
            assert!(
                reason.contains("not the independently verified automatic count"),
                "{target}: {reason}"
            );
        }
    }
    /// Only an edition with a supported full ROM build has a main image. The
    /// same complete files that make The Lost Age's English inventory
    /// authoritative give its Japanese edition, whose build is compile-only,
    /// nothing: every reader withholds that inventory.
    #[test]
    fn an_edition_without_a_full_build_is_never_scored() {
        use crate::targets::DecompTargetId::{TlaEn, TlaJa};
        for (id, supported) in [(TlaJa, false), (TlaEn, true)] {
            let directory = tempfile::tempdir().unwrap();
            let root = directory.path();
            let target = crate::targets::target_for(id);
            crate::coverage::audit::authoritative_fixture(
                root,
                crate::coverage::proof::fully_buildable(target),
                &[(0x0800_0100, 0x0800_0104)],
                json!([{"id": "resource_test", "decoded_bytes": 8, "intervals": [
                    {"start": 0x0200_0000, "end": 0x0200_0004, "kind": "thumb"}
                ]}]),
            );
            assert_eq!(
                authoritative_inventory(root, target).unwrap().is_some(),
                supported,
                "{id}"
            );
            let tree = crate::coverage::tree::work_tree_at(root.to_path_buf());
            match read_inventory(&tree, target) {
                Ok(_) => assert!(supported, "{id}"),
                Err(reason) => {
                    assert!(!supported, "{id}: {reason}");
                    assert!(reason.contains("no supported full ROM build"), "{reason}");
                }
            }
        }
    }
    /// The Lost Age credits its maintained main assembly and container
    /// runtime from its full build's verified assembly manifest, as The
    /// Broken Seal does, and refuses a credited module that misses its
    /// audited executable intervals.
    #[test]
    fn lost_age_main_assembly_credit_reads_its_verified_manifest() {
        let directory = tempfile::tempdir().unwrap();
        let root = directory.path();
        let write = |path: &str, value: Value| {
            let path = root.join(path);
            std::fs::create_dir_all(path.parent().unwrap()).unwrap();
            std::fs::write(path, value.to_string()).unwrap();
        };
        let registry =
            crate::compiler::runtime::registry_path(crate::compiler::routing::CompilerTarget::Tla);
        let target = crate::targets::target_for(crate::targets::DecompTargetId::TlaEn);
        write(
            &registry,
            json!({"links": [{"image": "main", "text": "0x08017878", "members": ["_m"]}]}),
        );
        write(
            "recon/tla/semantic/overlay-assembly.json",
            classification(json!([])),
        );
        write(
            "recon/tla/source-paths.json",
            json!({"format": 3, "owners": {}}),
        );
        crate::coverage::audit::authoritative_fixture(
            root,
            target,
            &[(0x0801_7800, 0x0801_7900)],
            json!([]),
        );
        let credit = json!({"credit": "handwritten", "proof": "software interrupts have no C form",
                            "object": "bios wrapper", "evidence": ["maintained source reproduces its ROM bytes"]});
        let manifest = |module_address: i64| {
            json!({"verification": "rom", "regions": [
                {"address": module_address, "size": 8, "kind": "maintained_assembly",
                 "source": "games/THE LOST AGE/SRC/SYSTEM/BIOS/CPU_SET.S", "retention": "keep_asm",
                 "confidence": "verified", "evidence": "", "provenance": credit},
                {"address": 0x0801_7800, "size": 8, "kind": "raw_assembly",
                 "source": "recon/tla/raw/08017800.s", "retention": "c_candidate",
                 "confidence": "unknown", "evidence": ""},
                {"address": 0x0801_7878, "size": 60, "kind": "compiler_runtime", "source": registry,
                 "retention": "container_runtime", "confidence": "proven",
                 "evidence": "built_from_licensed_compiler_container"}
            ]})
        };
        write(
            "out/tla-en/full/claimed/manifest.json",
            json!({"regions": []}),
        );
        write("out/tla-en/full/asm/manifest.json", manifest(0x0801_7808));
        let credits = || {
            let tree = crate::coverage::tree::work_tree_at(root.to_path_buf());
            verified_credits(&BuildOptions {
                target: target.id.to_string(),
                exact: &tree,
                recon: None,
            })
            .map(|credits| {
                credits
                    .into_iter()
                    .map(|credit| (credit.image, credit.start, credit.end, credit.source))
                    .collect::<Vec<_>>()
            })
        };
        assert_eq!(
            credits().unwrap(),
            [
                (
                    "main".to_string(),
                    0x0801_7808,
                    0x0801_7810,
                    "games/THE LOST AGE/SRC/SYSTEM/BIOS/CPU_SET.S".to_string()
                ),
                (
                    "main".to_string(),
                    0x0801_7878,
                    0x0801_78b4,
                    registry.clone()
                ),
            ]
        );
        // A credited module entirely outside the audited intervals is refused.
        write("out/tla-en/full/asm/manifest.json", manifest(0x0802_0000));
        let error = credits().unwrap_err();
        assert!(
            error.contains("CPU_SET.S lies outside audited executable intervals"),
            "{error}"
        );
    }
    #[test]
    fn each_game_credits_runtime_from_its_own_registry() {
        let directory = tempfile::tempdir().unwrap();
        let root = directory.path();
        let write = |path: &str, value: Value| {
            let path = root.join(path);
            std::fs::create_dir_all(path.parent().unwrap()).unwrap();
            std::fs::write(path, value.to_string()).unwrap();
        };
        let tla =
            crate::compiler::runtime::registry_path(crate::compiler::routing::CompilerTarget::Tla);
        let tbs =
            crate::compiler::runtime::registry_path(crate::compiler::routing::CompilerTarget::Tbs);
        write(
            &tla,
            json!({"links": [{"image": "main", "text": "0x08017878", "members": ["_m"]}]}),
        );
        write(
            "out/tla-en/full/asm/manifest.json",
            json!({"regions": [
                {"address": 0x0801_7878, "size": 60, "source": tla, "retention": "container_runtime"},
                // A region attributed to the other game's registry is not this game's runtime.
                {"address": 0x0801_78b4, "size": 60, "source": tbs, "retention": "container_runtime"}
            ]}),
        );
        let tree = crate::coverage::tree::work_tree_at(root.to_path_buf());
        let target = crate::targets::target_for(crate::targets::DecompTargetId::TlaEn);
        let exec = [Span::new(0x0801_7000, 0x0801_8000)];
        let (main, overlays) = runtime_credit_for(&tree, &target, &exec, &SpanMap::new()).unwrap();
        assert_eq!(main, [Span::new(0x0801_7878, 0x0801_78b4)]);
        assert!(overlays.is_empty());
        let outside = [Span::new(0x0801_7880, 0x0801_8000)];
        assert!(
            runtime_credit_for(&tree, &target, &outside, &SpanMap::new())
                .unwrap_err()
                .contains("lies outside audited executable intervals")
        );
        let tbs_target = crate::targets::target_for(crate::targets::DEFAULT_TARGET);
        let (main, _) = runtime_credit_for(&tree, &tbs_target, &exec, &SpanMap::new()).unwrap();
        assert!(main.is_empty());
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
            "recon/tla/source-paths.json",
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
            "recon/tla/raw/overlays/resource_64a_overlay.s",
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
            "recon/tla/source-paths.json",
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
            "recon/tla/raw/overlays/resource_64a_overlay.s",
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
            "out/tla-en/reports/executable.json",
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
            "recon/tbs/raw/overlays/resource_3bf_overlay.s",
            "AlchemyC_0200034c:\n\t.space 0x572\n\t.short 0\nAlchemyC_020008c0:\n\t.space 0x11c\n",
        );
        write(
            "recon/tbs/raw/overlays/resource_39b_overlay.s",
            "AlchemyC_02000630:\n\t.space 0x572\n\t.short 0\nAlchemyC_02000ba4:\n\t.space 0x11c\n",
        );
        let units = |gap: (&str, &str)| {
            json!({"units": [{
                "id": "staged-actor", "source": source, "overlay": "resource_3bf",
                "owners": [
                    {"address": "0x0200034c", "extent": 1394},
                    {"address": "0x020008c0", "extent": 284}
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
            "recon/tbs/translation-units.json",
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
            "recon/tbs/translation-units.json",
            &units(("0x020008be", "0x020008c0")),
        );
        let error = exact_overlay(&tree, &overlay_ids(&tree), &executable).unwrap_err();
        assert_eq!(
            error,
            "resource_39b: compiler gap lacks exact adjacent owners"
        );
    }
    #[test]
    fn overlay_drafts_follow_declared_units() {
        let root = tempfile::tempdir().unwrap();
        let directory = "recon/tbs/en/overlays";
        let write = |path: &str, source: &str| {
            let path = root.path().join(path);
            std::fs::create_dir_all(path.parent().unwrap()).unwrap();
            std::fs::write(path, source).unwrap();
        };
        for name in ["actor_sequence", "unregistered", "resource_test_c_02000160"] {
            write(
                &format!("{directory}/{name}.c"),
                "void Actor_Run(void) {}\n",
            );
        }
        write(&format!("{directory}/uncanonical.c"), "M2C_ERROR\n");
        // The reviewed register bounds each not-yet-C owner.
        write(
            "recon/tbs/raw/overlays/resource_test_overlay.s",
            "\t.space 0x1b0\n",
        );
        let region = |entry: &str, span: usize| json!({"overlay": "resource_test", "entry": entry, "span_bytes": span});
        write(
            "recon/tbs/semantic/regions.json",
            &json!({"format": 1, "manual_regions": [
                region("0x02000100", 0x20),
                region("0x02000120", 0x20),
                region("0x02000160", 0x10),
                region("0x02000190", 0x10),
                region("0x020001a0", 0x10)
            ]})
            .to_string(),
        );
        let owner = |entry| json!({"address": entry});
        let unit = |name, owners| {
            json!({
                "overlay": "resource_test",
                "source": format!("{directory}/{name}.c"), "owners": owners
            })
        };
        let named = unit(
            "actor_sequence",
            json!([owner("0x02000100"), owner("0x02000120")]),
        );
        write(
            "recon/tbs/translation-units.json",
            &json!({"units": [
                named.clone(), named,
                unit("resource_test_c_02000160", json!([owner("0x02000160")])),
                unit("missing", json!([owner("0x02000190")])),
                unit("uncanonical", json!([owner("0x020001a0")]))
            ]})
            .to_string(),
        );
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
        let found: BTreeMap<String, Vec<Span>> =
            BTreeMap::from([("resource_test".into(), expected.clone())]);
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
        assert_eq!(tile.categories[Category::DraftC as usize], 0x40);
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
    fn stream_ids_resolve_overlay_homes() {
        let root = tempfile::tempdir().unwrap();
        let write = |path: &str, text: String| {
            let path = root.path().join(path);
            std::fs::create_dir_all(path.parent().unwrap()).unwrap();
            std::fs::write(path, text).unwrap();
        };
        write(
            "games/THE BROKEN SEAL/SRC/FIELD/COMMON/SCENE_TABLE.JSON",
            json!({"segments":[{"records":[{"resource_id":879,"map_index":0}]}]}).to_string(),
        );
        write(
            "recon/tbs/overlay-homes.tsv",
            "resource_36f\tSRC/MENU/TITLE\n".into(),
        );
        let tree = SourceTree::Work {
            id: "fixture".into(),
            root: root.path().into(),
        };
        let target = crate::targets::target_for(crate::targets::DecompTargetId::TbsEn);
        assert_eq!(
            atlas_sources(&tree, &target)
                .get(&0x36f)
                .map(String::as_str),
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
        row["kind"] = json!("overlay_trampoline");
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
    fn generic_overlay_credit_is_rejected() {
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
        let error = overlay_assembly_classification_document(
            &classification(json!([library])),
            &no_inventory(),
            &executable(),
        )
        .unwrap_err();
        assert!(error.contains("unsupported generic assembly credit"));

        for mut row in [uncredited, strong, bare] {
            row["provenance"] = Value::Null;
            assert!(overlay_assembly_classification_document(
                &classification(json!([row])),
                &no_inventory(),
                &executable(),
            )
            .is_ok());
        }
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
