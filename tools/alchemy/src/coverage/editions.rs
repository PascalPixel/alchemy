//! Diagnostic edition coverage. Missing C is reported, never made a credit rule.
use super::calcrom::{self, Language};
use crate::targets::{target_for, DecompTarget, DecompTargetId, TARGET_IDS};
use std::collections::{BTreeMap, BTreeSet};
use std::path::{Path, PathBuf};
use std::process::ExitCode;

const USAGE: &str = "usage: alchemy check editions [--write-report]\n\
Report Japanese placed C and the verified six-edition union missing from each\n\
edition, separately for main and each live code overlay. Japanese evidence is\n\
preferred; other reference editions are named. A union stays pending until all\n\
six snapshots are current. Missing C is diagnostic and does not alter DONE. Reports\n\
under out/ are for people and never feed builds or counting policy.";

#[derive(Clone, Debug, PartialEq, Eq, PartialOrd, Ord)]
struct Placement {
    image: String,
    source: String,
}

#[derive(Clone, Debug, Default)]
struct Snapshot {
    pending: Option<String>,
    placed: BTreeMap<Placement, i64>,
    overlays: BTreeSet<String>,
    functions: BTreeMap<(Placement, String), i64>,
}

#[derive(Clone, Debug)]
struct MissingFunction {
    target: DecompTargetId,
    placement: Placement,
    name: String,
    japanese_bytes: i64,
    absent_source: bool,
}

type Definitions = BTreeMap<(String, String), BTreeSet<String>>;

fn cached_definitions(
    root: &Path,
    target: DecompTarget,
    source: &str,
    cache: &mut Definitions,
) -> Result<BTreeSet<String>, String> {
    let key = (target.id.as_str().to_string(), source.to_string());
    if !cache.contains_key(&key) {
        cache.insert(key.clone(), source_definitions(root, target, source)?);
    }
    Ok(cache[&key].clone())
}

#[derive(Clone, Debug, PartialEq, Eq)]
struct Missing {
    target: DecompTargetId,
    placement: Placement,
    japanese_bytes: i64,
}

#[derive(Clone, Copy, Debug, PartialEq, Eq)]
struct Evidence {
    target: DecompTargetId,
    bytes: i64,
}

#[derive(Debug, Default)]
struct PlacedUnion {
    pending: Option<String>,
    placed: BTreeMap<Placement, Evidence>,
    functions: BTreeMap<(Placement, String), Evidence>,
}

#[derive(Debug)]
struct UnionMissing {
    target: DecompTargetId,
    placement: Placement,
    evidence: Evidence,
    absent_source: bool,
}

#[derive(Debug)]
struct UnionMissingFunction {
    target: DecompTargetId,
    placement: Placement,
    name: String,
    evidence: Evidence,
    absent_source: bool,
}

/// A union is complete only after every edition of this game passes the same
/// snapshot gates. Prefer Japanese extents, never the largest localized size.
fn verified_union(target: DecompTarget, snapshots: &BTreeMap<String, Snapshot>) -> PlacedUnion {
    let native = japanese(target);
    let editions: Vec<_> = std::iter::once(native)
        .chain(
            TARGET_IDS
                .into_iter()
                .filter(|id| *id != native && target_for(*id).compiler == target.compiler),
        )
        .collect();
    let pending: Vec<_> = editions
        .iter()
        .filter_map(|id| {
            snapshots[id.as_str()]
                .pending
                .as_ref()
                .map(|reason| format!("{id}: {reason}"))
        })
        .collect();
    if !pending.is_empty() {
        return PlacedUnion {
            pending: Some(pending.join("; ")),
            ..PlacedUnion::default()
        };
    }
    let mut union = PlacedUnion::default();
    for id in editions {
        let own = &snapshots[id.as_str()];
        for (key, bytes) in &own.placed {
            if *bytes > 0 {
                union.placed.entry(key.clone()).or_insert(Evidence {
                    target: id,
                    bytes: *bytes,
                });
            }
        }
        for (key, bytes) in &own.functions {
            if *bytes > 0 {
                union.functions.entry(key.clone()).or_insert(Evidence {
                    target: id,
                    bytes: *bytes,
                });
            }
        }
    }
    union
}

fn union_missing(
    root: &Path,
    target: DecompTarget,
    union: &PlacedUnion,
    own: &Snapshot,
    definitions: &mut Definitions,
) -> Result<(Vec<UnionMissing>, Vec<UnionMissingFunction>), String> {
    if union.pending.is_some() || own.pending.is_some() {
        return Ok((Vec::new(), Vec::new()));
    }
    let mut objects = Vec::new();
    for (placement, evidence) in &union.placed {
        if own.placed.get(placement).copied().unwrap_or(0) > 0 {
            continue;
        }
        let reference = cached_definitions(
            root,
            target_for(evidence.target),
            &placement.source,
            definitions,
        )?;
        let edition = cached_definitions(root, target, &placement.source, definitions)?;
        objects.push(UnionMissing {
            target: target.id,
            placement: placement.clone(),
            evidence: *evidence,
            absent_source: !reference.is_empty() && edition.is_empty(),
        });
    }
    let mut functions = Vec::new();
    for ((placement, name), evidence) in &union.functions {
        if own
            .functions
            .get(&(placement.clone(), name.clone()))
            .copied()
            .unwrap_or(0)
            > 0
        {
            continue;
        }
        let reference = cached_definitions(
            root,
            target_for(evidence.target),
            &placement.source,
            definitions,
        )?;
        let edition = cached_definitions(root, target, &placement.source, definitions)?;
        functions.push(UnionMissingFunction {
            target: target.id,
            placement: placement.clone(),
            name: name.clone(),
            evidence: *evidence,
            absent_source: reference.contains(name) && !edition.contains(name),
        });
    }
    Ok((objects, functions))
}

fn render_union(
    unions: &BTreeMap<String, PlacedUnion>,
    snapshots: &BTreeMap<String, Snapshot>,
    objects: &[UnionMissing],
    functions: &[UnionMissingFunction],
) -> (String, String, String) {
    let mut summary = String::from("target\tstatus\tunion_main_c_objects\tunion_overlay_c_placements\tmissing_main\tmissing_overlay\tabsent_source_main\tabsent_source_overlay\tmissing_main_functions\tmissing_overlay_functions\tabsent_source_main_functions\tabsent_source_overlay_functions\treason\n");
    let mut details = String::from("game\tedition\timage\tsource\treference_edition\treference_text_bytes\tedition_text_bytes\tstatus\n");
    let mut function_details = String::from(
        "game\tedition\timage\tsource\tfunction\treference_edition\treference_text_bytes\tstatus\n",
    );
    for id in TARGET_IDS {
        let target = target_for(id);
        let union = &unions[japanese(target).as_str()];
        if let Some(reason) = union
            .pending
            .as_ref()
            .or(snapshots[id.as_str()].pending.as_ref())
        {
            summary.push_str(&format!(
                "{id}\tpending\t\t\t\t\t\t\t\t\t\t\t{}\n",
                reason.replace(['\t', '\n', '\r'], " ")
            ));
            continue;
        }
        let main = union
            .placed
            .keys()
            .filter(|key| key.image == "main")
            .count();
        let rows: Vec<_> = objects.iter().filter(|row| row.target == id).collect();
        let functions: Vec<_> = functions.iter().filter(|row| row.target == id).collect();
        let counts = |main: bool, absent: bool| {
            rows.iter()
                .filter(|row| {
                    (row.placement.image == "main") == main && (!absent || row.absent_source)
                })
                .count()
        };
        let function_counts = |main: bool, absent: bool| {
            functions
                .iter()
                .filter(|row| {
                    (row.placement.image == "main") == main && (!absent || row.absent_source)
                })
                .count()
        };
        summary.push_str(&format!(
            "{id}\tverified\t{main}\t{}\t{}\t{}\t{}\t{}\t{}\t{}\t{}\t{}\t\n",
            union.placed.len() - main,
            counts(true, false),
            counts(false, false),
            counts(true, true),
            counts(false, true),
            function_counts(true, false),
            function_counts(false, false),
            function_counts(true, true),
            function_counts(false, true)
        ));
        for row in rows {
            details.push_str(&format!(
                "{}\t{id}\t{}\t{}\t{}\t{}\t0\t{}\n",
                target.compiler.as_str(),
                row.placement.image,
                row.placement.source,
                row.evidence.target,
                row.evidence.bytes,
                if row.absent_source {
                    "absent-in-edition-source"
                } else {
                    "missing-placement"
                }
            ));
        }
        for row in functions {
            function_details.push_str(&format!(
                "{}\t{id}\t{}\t{}\t{}\t{}\t{}\t{}\n",
                target.compiler.as_str(),
                row.placement.image,
                row.placement.source,
                row.name,
                row.evidence.target,
                row.evidence.bytes,
                if row.absent_source {
                    "absent-in-edition-source"
                } else {
                    "missing-placement"
                }
            ));
        }
    }
    (summary, details, function_details)
}

fn placed_c(
    root: &Path,
    target: DecompTarget,
    context: &str,
    map: &str,
) -> BTreeMap<Placement, i64> {
    let mut placed = BTreeMap::new();
    for (section, size, object) in calcrom::sections(map) {
        if size <= 0 || !section.contains("text") {
            continue;
        }
        let Some((path, _)) =
            calcrom::relative_object(object, target.output_dir, context != "main")
        else {
            continue;
        };
        let Some(stem) = path
            .strip_suffix(".o")
            .filter(|path| path.starts_with("games/"))
        else {
            continue;
        };
        if calcrom::maintained_source(root, stem) != Some(Language::C) {
            continue;
        }
        let source = ["C", "c"]
            .iter()
            .map(|ext| format!("{stem}.{ext}"))
            .find(|path| root.join(path).is_file())
            .expect("C source classified above");
        *placed
            .entry(Placement {
                image: context.into(),
                source,
            })
            .or_default() += size;
    }
    placed
}

fn placed_functions(
    root: &Path,
    target: DecompTarget,
    context: &str,
    map: &str,
) -> Result<BTreeMap<(Placement, String), i64>, String> {
    let sources = placed_c(root, target, context, map);
    let mut functions = BTreeMap::new();
    for (section, size, object) in calcrom::sections(map) {
        if size <= 0 || !section.contains("text") {
            continue;
        }
        let Some((path, _)) =
            calcrom::relative_object(object, target.output_dir, context != "main")
        else {
            continue;
        };
        let Some(stem) = path.strip_suffix(".o") else {
            continue;
        };
        let source = sources
            .keys()
            .find(|key| key.source == format!("{stem}.C") || key.source == format!("{stem}.c"));
        let Some(placement) = source else {
            continue;
        };
        for (name, extent) in calcrom::placed_functions(object, section, size)? {
            *functions.entry((placement.clone(), name)).or_default() += extent;
        }
    }
    Ok(functions)
}

/// Only objects actually placed in main can own the overlay streams it reads.
/// Leftover maps in out/ never create a live stream.
fn live_overlays(
    root: &Path,
    target: DecompTarget,
    main: &str,
) -> Result<BTreeSet<String>, String> {
    let mut owners = BTreeSet::new();
    for (_, size, object) in calcrom::sections(main) {
        if size <= 0 {
            continue;
        }
        if let Some((path, _)) = calcrom::relative_object(object, target.output_dir, false) {
            if let Some(stem) = path.strip_suffix(".o") {
                owners.insert(stem.to_string());
            }
        }
    }
    let mut live = BTreeSet::new();
    for stem in owners {
        let source = ["S", "s"]
            .iter()
            .map(|ext| PathBuf::from(format!("{stem}.{ext}")))
            .find(|path| root.join(path).is_file());
        if let Some(source) = source {
            live.extend(crate::build_rom::stream_ids(root, target, &source)?);
        }
    }
    Ok(live)
}

fn read_map(
    root: &Path,
    path: &Path,
    image_time: std::time::SystemTime,
) -> Result<Result<String, String>, String> {
    let metadata = match std::fs::metadata(path) {
        Ok(metadata) => metadata,
        Err(error) if error.kind() == std::io::ErrorKind::NotFound => {
            return Ok(Err(format!(
                "missing {}",
                path.strip_prefix(root).unwrap_or(path).display()
            )))
        }
        Err(error) => return Err(format!("{}: {error}", path.display())),
    };
    if metadata.modified().map_err(|error| error.to_string())? > image_time {
        return Ok(Err(format!(
            "{} is newer than its verified image; rebuild",
            path.strip_prefix(root).unwrap_or(path).display()
        )));
    }
    Ok(Ok(
        std::fs::read_to_string(path).map_err(|error| format!("{}: {error}", path.display()))?
    ))
}

/// Maps describe the verified link, so a later experimental object cannot
/// supply its function intervals. Input keys prove the current source still
/// matches that object and its digest; timestamps bind them to the image.
fn current_objects(
    root: &Path,
    target: DecompTarget,
    map: &str,
    overlay: bool,
    image_time: std::time::SystemTime,
) -> Result<(), String> {
    let mut seen = BTreeSet::new();
    for (_, size, name) in calcrom::sections(map) {
        if size < 0 {
            continue;
        }
        let Some((relative, _)) = calcrom::relative_object(name, target.output_dir, overlay) else {
            continue;
        };
        let Some(stem) = relative.strip_suffix(".o") else {
            continue;
        };
        let Some(source) = ["C", "c", "S", "s"]
            .iter()
            .map(|ext| PathBuf::from(format!("{stem}.{ext}")))
            .find(|path| root.join(path).is_file())
        else {
            continue;
        };
        if !seen.insert(source.clone()) {
            continue;
        }
        let object = root
            .join(target.output_dir)
            .join("obj")
            .join(&source)
            .with_extension("o");
        let stamp = object.with_extension("o.key");
        let digest = object.with_extension("o.digest");
        for path in [&object, &stamp, &digest] {
            let time = std::fs::metadata(root.join(path))
                .and_then(|metadata| metadata.modified())
                .map_err(|error| format!("{}: {error}; rebuild", path.display()))?;
            if time > image_time {
                return Err(format!(
                    "{} is newer than its verified image; rebuild",
                    path.display()
                ));
            }
        }
        let key = crate::build_rom::current_object_key(root, target, &source, &object).map_err(
            |error| {
                format!(
                    "{}: current inputs unavailable: {error}; rebuild",
                    source.display()
                )
            },
        )?;
        let saved =
            std::fs::read_to_string(root.join(&stamp)).map_err(|error| error.to_string())?;
        if saved != key {
            return Err(format!(
                "{} inputs differ from its placed object; rebuild",
                source.display()
            ));
        }
        if !crate::build_rom::compiled_cache(&object, &stamp, &key) {
            return Err(format!(
                "{} content differs from its compiled digest; rebuild",
                object.display()
            ));
        }
    }
    Ok(())
}

/// Source composition has no expected-byte role. Conservatively mark a link
/// pending after a script/listing/include edit, even if its old outputs remain.
fn composition_fresh(
    root: &Path,
    path: &Path,
    output: &Path,
    image_time: std::time::SystemTime,
) -> Result<(), String> {
    fn visit(
        root: &Path,
        path: &Path,
        output: &Path,
        limit: std::time::SystemTime,
        seen: &mut BTreeSet<PathBuf>,
    ) -> Result<(), String> {
        let path = root.join(path);
        if !seen.insert(path.clone()) {
            return Ok(());
        }
        let time = std::fs::metadata(&path)
            .and_then(|metadata| metadata.modified())
            .map_err(|error| format!("{}: {error}; rebuild", path.display()))?;
        if time > limit {
            return Err(format!(
                "{} composition is newer than its verified image; rebuild",
                path.display()
            ));
        }
        let text = std::fs::read_to_string(&path).map_err(|error| error.to_string())?;
        let pattern = regex::Regex::new(r#"(?m)^\s*(?:INCLUDE|\.include)\s+"([^"]+)""#)
            .expect("static pattern");
        for capture in pattern.captures_iter(&text) {
            let name = Path::new(&capture[1]);
            let include = [root.join(name), output.join(name)]
                .into_iter()
                .find(|path| path.is_file())
                .unwrap_or_else(|| root.join(name));
            visit(root, &include, output, limit, seen)?;
        }
        Ok(())
    }
    visit(root, path, output, image_time, &mut BTreeSet::new())
}

fn snapshot(root: &Path, target: DecompTarget) -> Result<Snapshot, String> {
    let pending = |reason| Snapshot {
        pending: Some(reason),
        ..Snapshot::default()
    };
    if let Err(reason) = calcrom::verified(root, target)? {
        return Ok(pending(reason));
    }
    let time = std::fs::metadata(root.join(calcrom::image(target)))
        .and_then(|metadata| metadata.modified())
        .map_err(|error| error.to_string())?;
    let output = root.join(target.output_dir);
    let mut scripts: BTreeSet<_> = std::iter::once(target.script())
        .chain(target.edition_script())
        .collect();
    // International fragments derive from their source-owned native English
    // order. A change there invalidates their diagnostic placement as well.
    if target.edition_script().is_some() {
        scripts.insert(
            Path::new("recon")
                .join(target.compiler.as_str())
                .join("en/MAIN.LD"),
        );
    }
    for script in scripts {
        if let Err(reason) = composition_fresh(root, &script, &output, time) {
            return Ok(pending(reason));
        }
    }
    let main_path = root
        .join(target.output_dir)
        .join(format!("{}.map", target.id));
    let main = match read_map(root, &main_path, time)? {
        Ok(map) => map,
        Err(reason) => return Ok(pending(reason)),
    };
    if let Err(reason) = current_objects(root, target, &main, false, time) {
        return Ok(pending(reason));
    }
    let overlays = live_overlays(root, target, &main)?;
    let mut placed = placed_c(root, target, "main", &main);
    let mut functions = placed_functions(root, target, "main", &main)?;
    for id in &overlays {
        let listings = Path::new(target.asm_dir).join("overlays");
        for source in [
            listings.join(format!("resource_{id}.ld")),
            listings.join(format!("resource_{id}_overlay.s")),
        ] {
            if let Err(reason) = composition_fresh(root, &source, &output, time) {
                return Ok(pending(reason));
            }
        }
        let directory = output.join("overlays");
        if !crate::build_rom::valid_overlay_products(&directory, id) {
            return Ok(pending(format!(
                "resource_{id} products differ from their source-built receipt; rebuild"
            )));
        }
        let products = crate::build_rom::overlay_products(&directory, id);
        for path in products.into_iter().chain([
            directory.join(format!("resource_{id}.lz.key")),
            directory.join(format!("resource_{id}.lz.digest")),
        ]) {
            let written = std::fs::metadata(&path).and_then(|metadata| metadata.modified());
            match written {
                Ok(written) if written <= time => {}
                Ok(_) => {
                    return Ok(pending(format!(
                        "{} is newer than its verified image; rebuild",
                        path.display()
                    )))
                }
                Err(error) => return Ok(pending(format!("{}: {error}; rebuild", path.display()))),
            }
        }
        let path = root
            .join(target.output_dir)
            .join("overlays")
            .join(format!("resource_{id}.map"));
        let map = match read_map(root, &path, time)? {
            Ok(map) => map,
            Err(reason) => return Ok(pending(reason)),
        };
        if let Err(reason) = current_objects(root, target, &map, true, time) {
            return Ok(pending(reason));
        }
        placed.extend(placed_c(root, target, &format!("overlay:{id}"), &map));
        functions.extend(placed_functions(
            root,
            target,
            &format!("overlay:{id}"),
            &map,
        )?);
    }
    Ok(Snapshot {
        pending: None,
        placed,
        overlays,
        functions,
    })
}

fn source_definitions(
    root: &Path,
    target: DecompTarget,
    source: &str,
) -> Result<BTreeSet<String>, String> {
    let path = root.join(source);
    let expanded = crate::compiler::preprocess::fresh(root, target, &path.to_string_lossy())?;
    crate::compiler::steering::owned_definitions(&expanded, &path.to_string_lossy())
}

/// A source exclusion is distinct from a source-defined function the edition
/// never places. Neither classification changes DONE or credit requirements.
fn missing_functions(
    root: &Path,
    target: DecompTarget,
    baseline: &Snapshot,
    own: &Snapshot,
    definitions: &mut Definitions,
) -> Result<Vec<MissingFunction>, String> {
    if baseline.pending.is_some() || own.pending.is_some() {
        return Ok(Vec::new());
    }
    let mut rows = Vec::new();
    for ((placement, name), extent) in &baseline.functions {
        if own
            .functions
            .get(&(placement.clone(), name.clone()))
            .copied()
            .unwrap_or(0)
            > 0
        {
            continue;
        }
        let japanese = cached_definitions(
            root,
            target_for(japanese(target)),
            &placement.source,
            definitions,
        )?;
        let edition = cached_definitions(root, target, &placement.source, definitions)?;
        rows.push(MissingFunction {
            target: target.id,
            placement: placement.clone(),
            name: name.clone(),
            japanese_bytes: *extent,
            absent_source: japanese.contains(name) && !edition.contains(name),
        });
    }
    Ok(rows)
}

fn render_functions(rows: &[MissingFunction]) -> String {
    let mut text =
        String::from("game\tedition\timage\tsource\tfunction\tjapanese_text_bytes\tstatus\n");
    for row in rows {
        let target = target_for(row.target);
        text.push_str(&format!(
            "{}\t{}\t{}\t{}\t{}\t{}\t{}\n",
            target.compiler.as_str(),
            row.target,
            row.placement.image,
            row.placement.source,
            row.name,
            row.japanese_bytes,
            if row.absent_source {
                "absent-in-edition-source"
            } else {
                "missing-placement"
            }
        ));
    }
    text
}

fn missing(target: DecompTargetId, baseline: &Snapshot, edition: &Snapshot) -> Vec<Missing> {
    if baseline.pending.is_some() || edition.pending.is_some() {
        return Vec::new();
    }
    baseline
        .placed
        .iter()
        .filter(|(key, _)| edition.placed.get(key).copied().unwrap_or(0) <= 0)
        .map(|(key, bytes)| Missing {
            target,
            placement: key.clone(),
            japanese_bytes: *bytes,
        })
        .collect()
}

fn japanese(target: DecompTarget) -> DecompTargetId {
    match target.compiler {
        crate::compiler::routing::CompilerTarget::Tbs => DecompTargetId::TbsJa,
        crate::compiler::routing::CompilerTarget::Tla => DecompTargetId::TlaJa,
    }
}

fn render(
    snapshots: &BTreeMap<String, Snapshot>,
    absent: &BTreeSet<(String, Placement)>,
) -> (String, String) {
    let mut summary = String::from("target\tstatus\tlive_overlays\tmain_c_objects\toverlay_c_placements\tmissing_main\tmissing_overlay\tabsent_source_main\tabsent_source_overlay\treason\n");
    let mut details = String::from(
        "game\tedition\timage\tsource\tjapanese_text_bytes\tedition_text_bytes\tstatus\n",
    );
    for id in TARGET_IDS {
        let target = target_for(id);
        let own = &snapshots[id.as_str()];
        let baseline = &snapshots[japanese(target).as_str()];
        let pending = own.pending.as_ref().or(baseline.pending.as_ref());
        let rows = missing(id, baseline, own);
        let main = own.placed.keys().filter(|key| key.image == "main").count();
        let missing_main = rows
            .iter()
            .filter(|row| row.placement.image == "main")
            .count();
        let absent_main = rows
            .iter()
            .filter(|row| {
                row.placement.image == "main"
                    && absent.contains(&(id.as_str().into(), row.placement.clone()))
            })
            .count();
        let absent_overlay = rows
            .iter()
            .filter(|row| {
                row.placement.image != "main"
                    && absent.contains(&(id.as_str().into(), row.placement.clone()))
            })
            .count();
        let status = if pending.is_some() {
            "pending"
        } else {
            "verified"
        };
        let reason = pending
            .map_or("", String::as_str)
            .replace(['\t', '\n', '\r'], " ");
        summary.push_str(&format!(
            "{id}\t{status}\t{}\t{main}\t{}\t{missing_main}\t{}\t{absent_main}\t{absent_overlay}\t{reason}\n",
            own.overlays.len(),
            own.placed.len() - main,
            rows.len() - missing_main
        ));
        for row in rows {
            details.push_str(&format!(
                "{}\t{id}\t{}\t{}\t{}\t0\t{}\n",
                target.compiler.as_str(),
                row.placement.image,
                row.placement.source,
                row.japanese_bytes,
                if absent.contains(&(id.as_str().into(), row.placement.clone())) {
                    "absent-in-edition-source"
                } else {
                    "missing-placement"
                }
            ));
        }
    }
    (summary, details)
}

fn run(root: &Path, write: bool) -> Result<(), String> {
    let snapshots = TARGET_IDS
        .into_iter()
        .map(|id| snapshot(root, target_for(id)).map(|value| (id.as_str().into(), value)))
        .collect::<Result<BTreeMap<_, _>, _>>()?;
    let unions: BTreeMap<String, PlacedUnion> = [DecompTargetId::TbsJa, DecompTargetId::TlaJa]
        .into_iter()
        .map(|id| {
            (
                id.as_str().into(),
                verified_union(target_for(id), &snapshots),
            )
        })
        .collect();
    let mut functions = Vec::new();
    let mut union_objects = Vec::new();
    let mut union_functions = Vec::new();
    let mut definitions = Definitions::new();
    let mut absent = BTreeSet::new();
    for id in TARGET_IDS {
        let target = target_for(id);
        let (objects, function_rows) = union_missing(
            root,
            target,
            &unions[japanese(target).as_str()],
            &snapshots[id.as_str()],
            &mut definitions,
        )?;
        union_objects.extend(objects);
        union_functions.extend(function_rows);
        functions.extend(missing_functions(
            root,
            target,
            &snapshots[japanese(target).as_str()],
            &snapshots[id.as_str()],
            &mut definitions,
        )?);
        for row in missing(
            id,
            &snapshots[japanese(target).as_str()],
            &snapshots[id.as_str()],
        ) {
            let source = cached_definitions(
                root,
                target_for(japanese(target)),
                &row.placement.source,
                &mut definitions,
            )?;
            let own = cached_definitions(root, target, &row.placement.source, &mut definitions)?;
            if !source.is_empty() && own.is_empty() {
                absent.insert((id.as_str().into(), row.placement));
            }
        }
    }
    let (summary, details) = render(&snapshots, &absent);
    let function_details = render_functions(&functions);
    let (union_summary, union_details, union_function_details) =
        render_union(&unions, &snapshots, &union_objects, &union_functions);
    print!("{summary}{union_summary}");
    if write {
        let path = root.join("out/reports");
        std::fs::create_dir_all(&path).map_err(|error| error.to_string())?;
        for (name, text) in [
            ("edition-c-summary.tsv", summary),
            ("edition-c-missing.tsv", details),
            ("edition-c-functions.tsv", function_details),
            ("edition-c-union-summary.tsv", union_summary),
            ("edition-c-union-missing.tsv", union_details),
            ("edition-c-union-functions.tsv", union_function_details),
        ] {
            let path = path.join(name);
            std::fs::write(&path, text).map_err(|error| format!("{}: {error}", path.display()))?;
            println!(
                "report={}",
                path.strip_prefix(root).unwrap_or(&path).display()
            );
        }
    } else {
        print!("{details}{function_details}{union_details}{union_function_details}");
    }
    Ok(())
}

pub(crate) fn entry(args: &[String]) -> ExitCode {
    let write = match args {
        [] => false,
        [arg] if arg == "--write-report" => true,
        [arg] if arg == "--help" || arg == "-h" => {
            println!("{USAGE}");
            return ExitCode::SUCCESS;
        }
        _ => {
            eprintln!("{USAGE}");
            return ExitCode::from(2);
        }
    };
    match run(crate::compiler::routing::root(), write) {
        Ok(()) => ExitCode::SUCCESS,
        Err(error) => {
            eprintln!("error: {error}");
            ExitCode::FAILURE
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    fn record_digest(object: &Path) {
        use sha2::Digest;
        std::fs::write(
            object.with_extension("o.digest"),
            format!("{:x}", sha2::Sha256::digest(std::fs::read(object).unwrap())),
        )
        .unwrap();
    }
    fn source(root: &Path, stem: &str, extension: &str, text: &str) {
        let path = root.join(format!("{stem}.{extension}"));
        std::fs::create_dir_all(path.parent().unwrap()).unwrap();
        std::fs::write(path, text).unwrap();
    }
    fn fixture() -> (tempfile::TempDir, DecompTarget) {
        let root = tempfile::tempdir().unwrap();
        source(root.path(), "games/COMMON/SRC/A", "C", "void A(void) {}\n");
        source(root.path(), "games/COMMON/SRC/B", "C", "void B(void) {}\n");
        source(root.path(), "games/COMMON/SRC/ASM", "S", ".thumb\nbx lr\n");
        (root, target_for(DecompTargetId::TbsEn))
    }

    fn empty_snapshots() -> BTreeMap<String, Snapshot> {
        TARGET_IDS
            .into_iter()
            .map(|id| (id.as_str().into(), Snapshot::default()))
            .collect()
    }

    #[test]
    fn union_reports_japanese_omissions_with_named_evidence_and_separate_images() {
        let (root, target) = fixture();
        let main = Placement {
            image: "main".into(),
            source: "games/COMMON/SRC/A.C".into(),
        };
        let overlay = Placement {
            image: "overlay:36f".into(),
            source: "games/COMMON/SRC/B.C".into(),
        };
        let mut snapshots = empty_snapshots();
        snapshots.get_mut("tbs-ja").unwrap().placed = [(main.clone(), 20)].into();
        snapshots.get_mut("tbs-ja").unwrap().functions = [((main.clone(), "A".into()), 20)].into();
        snapshots.get_mut("tbs-en").unwrap().placed =
            [(main.clone(), 24), (overlay.clone(), 32)].into();
        snapshots.get_mut("tbs-en").unwrap().functions = [
            ((main.clone(), "A".into()), 24),
            ((overlay.clone(), "B".into()), 32),
        ]
        .into();
        // A different game and a non-positive placement supply no TBS evidence.
        snapshots.get_mut("tla-ja").unwrap().placed = [(
            Placement {
                image: "main".into(),
                source: "games/COMMON/SRC/OTHER.C".into(),
            },
            80,
        )]
        .into();
        snapshots.get_mut("tbs-de").unwrap().placed = [(
            Placement {
                image: "main".into(),
                source: "games/COMMON/SRC/EMPTY.C".into(),
            },
            0,
        )]
        .into();
        let before = snapshots["tbs-ja"].placed.clone();
        let union = verified_union(target, &snapshots);
        assert!(union.pending.is_none());
        assert_eq!(union.placed.len(), 2);
        assert_eq!(
            union.placed[&main],
            Evidence {
                target: DecompTargetId::TbsJa,
                bytes: 20
            }
        );
        assert_eq!(union.functions[&(main.clone(), "A".into())].bytes, 20);
        let (objects, functions) = union_missing(
            root.path(),
            target_for(DecompTargetId::TbsJa),
            &union,
            &snapshots["tbs-ja"],
            &mut Definitions::new(),
        )
        .unwrap();
        assert_eq!(objects.len(), 1);
        assert_eq!(functions.len(), 1);
        assert_eq!(objects[0].placement, overlay);
        assert_eq!(objects[0].evidence.target, DecompTargetId::TbsEn);
        assert!(!objects[0].absent_source);
        let (present_objects, present_functions) = union_missing(
            root.path(),
            target,
            &union,
            &snapshots["tbs-en"],
            &mut Definitions::new(),
        )
        .unwrap();
        assert!(present_objects.is_empty() && present_functions.is_empty());
        let unions = [
            ("tbs-ja".into(), union),
            (
                "tla-ja".into(),
                verified_union(target_for(DecompTargetId::TlaJa), &snapshots),
            ),
        ]
        .into();
        let (_, details, function_details) =
            render_union(&unions, &snapshots, &objects, &functions);
        assert!(details.contains(
            "tbs\ttbs-ja\toverlay:36f\tgames/COMMON/SRC/B.C\ttbs-en\t32\t0\tmissing-placement"
        ));
        assert!(function_details.contains("B\ttbs-en\t32\tmissing-placement"));
        assert_eq!(snapshots["tbs-ja"].placed, before);
    }

    #[test]
    fn union_source_exclusions_use_fresh_target_definitions_and_do_not_hide_unlinked_functions() {
        let (root, target) = fixture();
        source(root.path(), "games/COMMON/SRC/A", "C",
            "#ifdef TBS_EDITION_EN\nvoid InternationalOnly(void) {}\n#endif\nvoid Shared(void) {}\n");
        source(
            root.path(),
            "games/COMMON/SRC/B",
            "C",
            "#ifdef TBS_EDITION_EN\nvoid InternationalObject(void) {}\n#endif\n",
        );
        let main = Placement {
            image: "main".into(),
            source: "games/COMMON/SRC/A.C".into(),
        };
        let only = Placement {
            image: "main".into(),
            source: "games/COMMON/SRC/B.C".into(),
        };
        let mut snapshots = empty_snapshots();
        snapshots.get_mut("tbs-en").unwrap().placed =
            [(main.clone(), 36), (only.clone(), 16)].into();
        snapshots.get_mut("tbs-en").unwrap().functions = [
            ((main.clone(), "InternationalOnly".into()), 16),
            ((main.clone(), "Shared".into()), 20),
            ((only.clone(), "InternationalObject".into()), 16),
        ]
        .into();
        let union = verified_union(target, &snapshots);
        let (objects, functions) = union_missing(
            root.path(),
            target_for(DecompTargetId::TbsJa),
            &union,
            &snapshots["tbs-ja"],
            &mut Definitions::new(),
        )
        .unwrap();
        assert!(
            !objects
                .iter()
                .find(|row| row.placement == main)
                .unwrap()
                .absent_source
        );
        assert!(
            objects
                .iter()
                .find(|row| row.placement == only)
                .unwrap()
                .absent_source
        );
        assert!(
            !functions
                .iter()
                .find(|row| row.name == "Shared")
                .unwrap()
                .absent_source
        );
        assert!(
            functions
                .iter()
                .find(|row| row.name == "InternationalOnly")
                .unwrap()
                .absent_source
        );
        assert!(
            functions
                .iter()
                .find(|row| row.name == "InternationalObject")
                .unwrap()
                .absent_source
        );
        // A new report must observe source edits rather than borrowing a cached
        // expansion from an earlier classification.
        source(
            root.path(),
            "games/COMMON/SRC/B",
            "C",
            "void InternationalObject(void) {}\n",
        );
        let (objects, functions) = union_missing(
            root.path(),
            target_for(DecompTargetId::TbsJa),
            &union,
            &snapshots["tbs-ja"],
            &mut Definitions::new(),
        )
        .unwrap();
        assert!(
            !objects
                .iter()
                .find(|row| row.placement == only)
                .unwrap()
                .absent_source
        );
        assert!(
            !functions
                .iter()
                .find(|row| row.name == "InternationalObject")
                .unwrap()
                .absent_source
        );
    }

    #[test]
    fn union_pending_snapshots_cannot_contribute_stale_evidence_or_claim_complete_totals() {
        let (_, target) = fixture();
        let mut snapshots = empty_snapshots();
        let stale = snapshots.get_mut("tbs-es").unwrap();
        let placement = Placement {
            image: "main".into(),
            source: "games/COMMON/SRC/A.C".into(),
        };
        stale.pending = Some("changed object digest\nrebuild".into());
        stale.placed = [(placement.clone(), 32)].into();
        stale.functions = [((placement, "A".into()), 32)].into();
        let union = verified_union(target, &snapshots);
        assert!(union
            .pending
            .as_ref()
            .unwrap()
            .contains("tbs-es: changed object digest"));
        assert!(union.placed.is_empty() && union.functions.is_empty());
        let unions = [
            ("tbs-ja".into(), union),
            (
                "tla-ja".into(),
                verified_union(target_for(DecompTargetId::TlaJa), &snapshots),
            ),
        ]
        .into();
        let (summary, details, functions) = render_union(&unions, &snapshots, &[], &[]);
        let row: Vec<_> = summary
            .lines()
            .find(|line| line.starts_with("tbs-ja\t"))
            .unwrap()
            .split('\t')
            .collect();
        assert_eq!(row.len(), 13);
        assert_eq!(row[1], "pending");
        assert!(row[2..12].iter().all(|field| field.is_empty()));
        assert!(summary.contains("tla-ja\tverified"));
        assert_eq!(details.lines().count(), 1);
        assert_eq!(functions.lines().count(), 1);
    }
    #[test]
    fn japanese_placed_main_and_overlays_are_the_diagnostic_base_for_all_editions() {
        let main = Placement {
            image: "main".into(),
            source: "games/COMMON/SRC/A.C".into(),
        };
        let overlay = Placement {
            image: "overlay:36f".into(),
            source: "games/COMMON/SRC/B.C".into(),
        };
        let mut snapshots = BTreeMap::new();
        for id in TARGET_IDS {
            let target = target_for(id);
            assert!(japanese(target).as_str().ends_with("-ja"));
            assert_eq!(target_for(japanese(target)).compiler, target.compiler);
            snapshots.insert(id.as_str().into(), Snapshot::default());
        }
        snapshots.get_mut("tbs-ja").unwrap().placed =
            [(main.clone(), 20), (overlay.clone(), 32)].into();
        snapshots.get_mut("tbs-en").unwrap().placed = [(main.clone(), 24)].into();
        let before = snapshots["tbs-ja"].placed.clone();
        let (summary, details) = render(&snapshots, &BTreeSet::new());
        assert!(summary.contains("tbs-en\tverified\t0\t1\t0\t0\t1\t"));
        assert!(summary.contains("tbs-ja\tverified\t0\t1\t1\t0\t0\t"));
        assert!(details.starts_with("game\tedition\timage\tsource\tjapanese_text_bytes\t"));
        assert!(details
            .contains("tbs\ttbs-en\toverlay:36f\tgames/COMMON/SRC/B.C\t32\t0\tmissing-placement"));
        assert!(!details.contains("tbs\ttbs-ja\t"));
        assert_eq!(snapshots["tbs-ja"].placed, before);
    }
    #[test]
    fn only_positive_placed_c_text_counts_and_images_remain_distinct() {
        let (root, target) = fixture();
        let map = "Discarded input sections\n .text 0x0 0x40 /r/out/tbs-en/obj/games/COMMON/SRC/B.o\nLinker script and memory map\n .text 0x0 0x10 /r/out/tbs-en/obj/games/COMMON/SRC/A.o\n .text.more\n                0x10 0x8 /r/out/tbs-en/obj/games/COMMON/SRC/A.o\n .rodata 0x18 0x20 /r/out/tbs-en/obj/games/COMMON/SRC/B.o\n .text 0x38 0x0 /r/out/tbs-en/obj/games/COMMON/SRC/B.o\n .text 0x38 0x8 /r/out/tbs-en/obj/games/COMMON/SRC/ASM.o\n";
        let main = placed_c(root.path(), target, "main", map);
        assert_eq!(main.len(), 1);
        assert_eq!(main.values().next(), Some(&24));
        let overlays = placed_c(root.path(), target, "overlay:36f", map);
        let baseline = Snapshot {
            placed: main.clone().into_iter().chain(overlays).collect(),
            ..Snapshot::default()
        };
        let own = Snapshot {
            placed: main,
            ..Snapshot::default()
        };
        let rows = missing(DecompTargetId::TbsJa, &baseline, &own);
        assert_eq!(rows.len(), 1);
        assert_eq!(rows[0].placement.image, "overlay:36f");
        assert_eq!(rows[0].japanese_bytes, 24);
        assert_eq!(baseline.placed.len(), 2); // reporting never changes the baseline
    }
    #[test]
    fn pending_inputs_never_claim_zero_missing_and_different_extents_are_present() {
        let key = Placement {
            image: "main".into(),
            source: "games/COMMON/SRC/A.C".into(),
        };
        let baseline = Snapshot {
            placed: [(key.clone(), 32)].into(),
            ..Snapshot::default()
        };
        let own = Snapshot {
            placed: [(key, 28)].into(),
            ..Snapshot::default()
        };
        assert!(missing(DecompTargetId::TbsJa, &baseline, &own).is_empty());
        let pending = Snapshot {
            pending: Some("image differs".into()),
            ..Snapshot::default()
        };
        assert!(missing(DecompTargetId::TbsJa, &baseline, &pending).is_empty());
        let mut snapshots = BTreeMap::new();
        for id in TARGET_IDS {
            snapshots.insert(id.as_str().into(), pending.clone());
        }
        let (summary, details) = render(&snapshots, &BTreeSet::new());
        assert!(summary.contains("tbs-ja\tpending"));
        assert_eq!(details.lines().count(), 1);
    }
    #[test]
    fn orphan_maps_are_not_live_and_active_source_streams_are_owned_by_placed_main() {
        let (root, target) = fixture();
        source(root.path(), "recon/tbs/ja/rom", "s", ".section .overlays\n.ifdef TBS_EDITION_EN\n.incbin \"overlays/resource_36f.lz\"\n.else\n.incbin \"overlays/resource_370.lz\"\n.endif\n");
        let output = root.path().join("out/tbs-en/overlays");
        std::fs::create_dir_all(&output).unwrap();
        std::fs::write(output.join("resource_dead.map"), "stale orphan").unwrap();
        let main = "Linker script and memory map\n .overlays 0x0 0x10 /r/out/tbs-en/obj/recon/tbs/ja/rom.o\n";
        assert_eq!(
            live_overlays(root.path(), target, main).unwrap(),
            ["36f".into()].into()
        );
        assert!(live_overlays(root.path(), target, "").unwrap().is_empty());
    }
    #[test]
    fn newer_maps_and_missing_live_maps_remain_pending() {
        let root = tempfile::tempdir().unwrap();
        let path = root.path().join("map");
        assert!(read_map(root.path(), &path, std::time::SystemTime::now())
            .unwrap()
            .is_err());
        std::fs::write(&path, "map").unwrap();
        assert!(read_map(root.path(), &path, std::time::UNIX_EPOCH)
            .unwrap()
            .is_err());
    }

    #[test]
    fn changed_source_dependencies_and_later_objects_cannot_borrow_verified_maps() {
        let (root, target) = fixture();
        let source = Path::new("games/COMMON/SRC/A.C");
        let object = root
            .path()
            .join(target.output_dir)
            .join("obj")
            .join(source)
            .with_extension("o");
        let stamp = object.with_extension("o.key");
        std::fs::create_dir_all(root.path().join(&object).parent().unwrap()).unwrap();
        std::fs::write(
            root.path().join(source),
            "#include \"OWN.H\"\nint A(void) { return VALUE; }\n",
        )
        .unwrap();
        let header = root.path().join("games/COMMON/SRC/OWN.H");
        std::fs::write(&header, "#define VALUE 1\n").unwrap();
        let key =
            crate::build_rom::current_object_key(root.path(), target, source, &object).unwrap();
        std::fs::write(root.path().join(&object), "placed object").unwrap();
        record_digest(&object);
        std::fs::write(root.path().join(&stamp), key).unwrap();
        let map = format!(
            "Linker script and memory map\n .text 0x0 0x8 {}\n",
            object.display()
        );
        let image_time = std::time::SystemTime::now();
        current_objects(root.path(), target, &map, false, image_time).unwrap();
        std::fs::write(&header, "#define VALUE 2\n").unwrap();
        assert!(
            current_objects(root.path(), target, &map, false, image_time)
                .unwrap_err()
                .contains("inputs differ")
        );
        std::fs::write(&header, "#define VALUE 1\n").unwrap();
        let object_time = std::fs::metadata(&object).unwrap().modified().unwrap();
        std::fs::write(&object, "later experiment with preserved timestamp").unwrap();
        std::fs::File::open(&object)
            .unwrap()
            .set_times(std::fs::FileTimes::new().set_modified(object_time))
            .unwrap();
        assert!(
            current_objects(root.path(), target, &map, false, image_time)
                .unwrap_err()
                .contains("content differs")
        );
        std::fs::write(&object, "placed object").unwrap();
        let future = image_time + std::time::Duration::from_secs(2);
        std::fs::File::open(root.path().join(&object))
            .unwrap()
            .set_times(std::fs::FileTimes::new().set_modified(future))
            .unwrap();
        assert!(
            current_objects(root.path(), target, &map, false, image_time)
                .unwrap_err()
                .contains("newer than its verified image")
        );
        assert!(!root.path().join(object.with_extension("i")).exists());
    }

    #[test]
    fn an_old_verified_scaffold_does_not_make_new_current_streams_live() {
        use sha1::Digest;
        let root = tempfile::tempdir().unwrap();
        let target = target_for(DecompTargetId::TbsEn);
        for script in std::iter::once(target.script()).chain(target.edition_script()) {
            let script = root.path().join(script);
            std::fs::create_dir_all(script.parent().unwrap()).unwrap();
            std::fs::write(&script, "SECTIONS {}\n").unwrap();
        }
        let owner = Path::new("recon/tbs/overlays.S");
        source(
            root.path(),
            "recon/tbs/overlays",
            "s",
            ".section .overlays\n.incbin \"baserom.gba\", 0, 16\n",
        );
        let output = root.path().join(target.output_dir);
        let object = output.join("obj").join(owner).with_extension("o");
        std::fs::create_dir_all(object.parent().unwrap()).unwrap();
        let key =
            crate::build_rom::current_object_key(root.path(), target, owner, &object).unwrap();
        std::fs::write(&object, "old placed assembly object").unwrap();
        record_digest(&object);
        std::fs::write(object.with_extension("o.key"), key).unwrap();
        std::fs::write(
            output.join("tbs-en.map"),
            format!(
                "Linker script and memory map\n .overlays 0x0 0x10 {}\n",
                object.display()
            ),
        )
        .unwrap();
        let image = b"synthetic verified image";
        std::fs::write(
            root.path().join("rom.sha1"),
            format!(
                "{:x}  {}\n",
                sha1::Sha1::digest(image),
                calcrom::image(target)
            ),
        )
        .unwrap();
        std::fs::write(root.path().join(calcrom::image(target)), image).unwrap();
        let current = snapshot(root.path(), target).unwrap();
        assert!(current.pending.is_none(), "{:?}", current.pending);
        std::fs::create_dir_all(output.join("overlays")).unwrap();
        std::fs::write(output.join("overlays/resource_36f.lz"), "leftover stream").unwrap();
        std::fs::write(
            output.join("overlays/resource_36f.map"),
            format!(
                "Linker script and memory map\n .text 0x0 0x40 {}/obj/games/COMMON/SRC/B.o\n",
                output.display()
            ),
        )
        .unwrap();
        let orphan = snapshot(root.path(), target).unwrap();
        assert!(orphan.pending.is_none());
        assert!(orphan.overlays.is_empty() && orphan.placed.is_empty());
        let mut snapshots = empty_snapshots();
        snapshots.insert(target.id.as_str().into(), orphan);
        let union = verified_union(target, &snapshots);
        assert!(union.pending.is_none());
        assert!(union.placed.is_empty() && union.functions.is_empty());
        std::fs::write(
            root.path().join(owner),
            ".section .overlays\n.incbin \"overlays/resource_36f.lz\"\n",
        )
        .unwrap();
        let stale = snapshot(root.path(), target).unwrap();
        assert!(stale.pending.unwrap().contains("inputs differ"));
        assert!(stale.overlays.is_empty() && stale.placed.is_empty());
    }

    #[test]
    fn international_native_order_changes_leave_derived_edition_diagnostics_pending() {
        use sha1::Digest;
        let root = tempfile::tempdir().unwrap();
        let target = target_for(DecompTargetId::TbsDe);
        for path in [
            target.script(),
            target.edition_script().unwrap(),
            PathBuf::from("recon/tbs/en/MAIN.LD"),
        ] {
            let path = root.path().join(path);
            std::fs::create_dir_all(path.parent().unwrap()).unwrap();
            std::fs::write(path, "SECTIONS {}\n").unwrap();
        }
        let output = root.path().join(target.output_dir);
        std::fs::create_dir_all(&output).unwrap();
        std::fs::write(output.join("tbs-de.map"), "Linker script and memory map\n").unwrap();
        let image = b"synthetic current image";
        std::fs::write(
            root.path().join("rom.sha1"),
            format!(
                "{:x}  {}\n",
                sha1::Sha1::digest(image),
                calcrom::image(target)
            ),
        )
        .unwrap();
        std::fs::write(root.path().join(calcrom::image(target)), image).unwrap();
        assert!(snapshot(root.path(), target).unwrap().pending.is_none());
        let future = std::time::SystemTime::now() + std::time::Duration::from_secs(2);
        std::fs::File::open(root.path().join("recon/tbs/en/MAIN.LD"))
            .unwrap()
            .set_times(std::fs::FileTimes::new().set_modified(future))
            .unwrap();
        let stale = snapshot(root.path(), target).unwrap();
        assert!(stale.pending.unwrap().contains("composition is newer"));
        assert!(stale.placed.is_empty() && stale.overlays.is_empty());
    }

    #[test]
    fn a_linked_empty_edition_object_still_needs_current_source_provenance() {
        let (root, _) = fixture();
        let target = target_for(DecompTargetId::TbsJa);
        let source = Path::new("games/COMMON/SRC/A.C");
        std::fs::write(
            root.path().join(source),
            "#ifdef TBS_EDITION_EN\nvoid EnglishOnly(void) {}\n#endif\n",
        )
        .unwrap();
        let object = root
            .path()
            .join(target.output_dir)
            .join("obj")
            .join(source)
            .with_extension("o");
        std::fs::create_dir_all(object.parent().unwrap()).unwrap();
        std::fs::write(&object, "empty edition object").unwrap();
        record_digest(&object);
        std::fs::write(
            object.with_extension("o.key"),
            crate::build_rom::current_object_key(root.path(), target, source, &object).unwrap(),
        )
        .unwrap();
        let map = format!(
            "Linker script and memory map\n .text 0x0 0x0 {}\n",
            object.display()
        );
        let image_time = std::time::SystemTime::now();
        current_objects(root.path(), target, &map, false, image_time).unwrap();
        std::fs::write(root.path().join(source), "void NowPresent(void) {}\n").unwrap();
        assert!(
            current_objects(root.path(), target, &map, false, image_time)
                .unwrap_err()
                .contains("inputs differ")
        );
    }

    #[test]
    fn scripts_raw_listings_and_generated_include_composition_are_bounded_to_the_link() {
        let root = tempfile::tempdir().unwrap();
        let output = root.path().join("out/target");
        std::fs::create_dir_all(&output).unwrap();
        let script = root.path().join("MAIN.LD");
        let included = root.path().join("OWN.LD");
        std::fs::write(&script, "INCLUDE \"OWN.LD\"\n").unwrap();
        std::fs::write(&included, "SECTIONS {}\n").unwrap();
        let listing = root.path().join("resource_36f_overlay.s");
        std::fs::write(&listing, ".include \"generated.inc\"\n").unwrap();
        let generated = output.join("generated.inc");
        std::fs::write(&generated, ".byte 1\n").unwrap();
        let time = std::time::SystemTime::now();
        for path in [&script, &listing] {
            composition_fresh(root.path(), path, &output, time).unwrap();
        }
        for path in [&script, &included, &listing, &generated] {
            std::fs::File::open(path)
                .unwrap()
                .set_times(
                    std::fs::FileTimes::new()
                        .set_modified(time + std::time::Duration::from_secs(2)),
                )
                .unwrap();
            let owner = if path == &script || path == &included {
                &script
            } else {
                &listing
            };
            assert!(composition_fresh(root.path(), owner, &output, time)
                .unwrap_err()
                .contains("composition is newer"));
            std::fs::File::open(path)
                .unwrap()
                .set_times(std::fs::FileTimes::new().set_modified(time))
                .unwrap();
        }
    }

    #[test]
    fn source_absence_is_separate_from_an_active_but_unlinked_function() {
        let (root, _) = fixture();
        source(
            root.path(),
            "games/COMMON/SRC/A",
            "C",
            "#if defined(TBS_EDITION_JA)\nvoid JapaneseOnly(void) {}\n#endif\nvoid Both(void) {}\n",
        );
        let placement = Placement {
            image: "main".into(),
            source: "games/COMMON/SRC/A.C".into(),
        };
        let baseline = Snapshot {
            functions: [
                ((placement.clone(), "JapaneseOnly".into()), 16),
                ((placement.clone(), "Both".into()), 20),
            ]
            .into(),
            ..Snapshot::default()
        };
        let own = Snapshot::default();
        let rows = missing_functions(
            root.path(),
            target_for(DecompTargetId::TbsEn),
            &baseline,
            &own,
            &mut Definitions::new(),
        )
        .unwrap();
        assert_eq!(rows.len(), 2);
        assert!(
            rows.iter()
                .find(|row| row.name == "JapaneseOnly")
                .unwrap()
                .absent_source
        );
        assert!(
            !rows
                .iter()
                .find(|row| row.name == "Both")
                .unwrap()
                .absent_source
        );
        let text = render_functions(&rows);
        assert!(text.contains("JapaneseOnly\t16\tabsent-in-edition-source"));
        assert!(text.contains("Both\t20\tmissing-placement"));
        assert_eq!(baseline.functions.len(), 2);
    }
}
