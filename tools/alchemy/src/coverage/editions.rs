//! Diagnostic edition coverage. Missing C is reported, never made a credit rule.
use super::calcrom::{self, Language};
use crate::targets::{target_for, DecompTarget, DecompTargetId, TARGET_IDS};
use std::collections::{BTreeMap, BTreeSet};
use std::path::{Path, PathBuf};
use std::process::ExitCode;

const USAGE: &str = "usage: alchemy check editions [--write-report]\n\
Report English placed C missing from an edition, separately for main and each\n\
live code overlay. Missing C is diagnostic and does not alter DONE. Reports\n\
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
    english_bytes: i64,
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
    english_bytes: i64,
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
/// matches that object; timestamps bound the object/key to the verified image.
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
        for path in [&object, &stamp] {
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
    for script in std::iter::once(target.script()).chain(target.edition_script()) {
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
        let object = output
            .join("overlays")
            .join(format!("resource_{id}_overlay.o"));
        let written = std::fs::metadata(&object).and_then(|metadata| metadata.modified());
        match written {
            Ok(written) if written <= time => {}
            Ok(_) => {
                return Ok(pending(format!(
                    "{} is newer than its verified image; rebuild",
                    object.display()
                )))
            }
            Err(error) => return Ok(pending(format!("{}: {error}; rebuild", object.display()))),
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
        let english = cached_definitions(
            root,
            target_for(english(target)),
            &placement.source,
            definitions,
        )?;
        let edition = cached_definitions(root, target, &placement.source, definitions)?;
        rows.push(MissingFunction {
            target: target.id,
            placement: placement.clone(),
            name: name.clone(),
            english_bytes: *extent,
            absent_source: english.contains(name) && !edition.contains(name),
        });
    }
    Ok(rows)
}

fn render_functions(rows: &[MissingFunction]) -> String {
    let mut text =
        String::from("game\tedition\timage\tsource\tfunction\tenglish_text_bytes\tstatus\n");
    for row in rows {
        let target = target_for(row.target);
        text.push_str(&format!(
            "{}\t{}\t{}\t{}\t{}\t{}\t{}\n",
            target.compiler.as_str(),
            row.target,
            row.placement.image,
            row.placement.source,
            row.name,
            row.english_bytes,
            if row.absent_source {
                "absent-in-edition-source"
            } else {
                "missing-placement"
            }
        ));
    }
    text
}

fn missing(target: DecompTargetId, english: &Snapshot, edition: &Snapshot) -> Vec<Missing> {
    if english.pending.is_some() || edition.pending.is_some() {
        return Vec::new();
    }
    english
        .placed
        .iter()
        .filter(|(key, _)| edition.placed.get(key).copied().unwrap_or(0) <= 0)
        .map(|(key, bytes)| Missing {
            target,
            placement: key.clone(),
            english_bytes: *bytes,
        })
        .collect()
}

fn english(target: DecompTarget) -> DecompTargetId {
    match target.compiler {
        crate::compiler::routing::CompilerTarget::Tbs => DecompTargetId::TbsEn,
        crate::compiler::routing::CompilerTarget::Tla => DecompTargetId::TlaEn,
    }
}

fn render(
    snapshots: &BTreeMap<String, Snapshot>,
    absent: &BTreeSet<(String, Placement)>,
) -> (String, String) {
    let mut summary = String::from("target\tstatus\tlive_overlays\tmain_c_objects\toverlay_c_placements\tmissing_main\tmissing_overlay\tabsent_source_main\tabsent_source_overlay\treason\n");
    let mut details = String::from(
        "game\tedition\timage\tsource\tenglish_text_bytes\tedition_text_bytes\tstatus\n",
    );
    for id in TARGET_IDS {
        let target = target_for(id);
        let own = &snapshots[id.as_str()];
        let baseline = &snapshots[english(target).as_str()];
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
                row.english_bytes,
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
    let mut functions = Vec::new();
    let mut definitions = Definitions::new();
    let mut absent = BTreeSet::new();
    for id in TARGET_IDS {
        let target = target_for(id);
        functions.extend(missing_functions(
            root,
            target,
            &snapshots[english(target).as_str()],
            &snapshots[id.as_str()],
            &mut definitions,
        )?);
        for row in missing(
            id,
            &snapshots[english(target).as_str()],
            &snapshots[id.as_str()],
        ) {
            let en = cached_definitions(
                root,
                target_for(english(target)),
                &row.placement.source,
                &mut definitions,
            )?;
            let own = cached_definitions(root, target, &row.placement.source, &mut definitions)?;
            if !en.is_empty() && own.is_empty() {
                absent.insert((id.as_str().into(), row.placement));
            }
        }
    }
    let (summary, details) = render(&snapshots, &absent);
    let function_details = render_functions(&functions);
    print!("{summary}");
    if write {
        let path = root.join("out/reports");
        std::fs::create_dir_all(&path).map_err(|error| error.to_string())?;
        for (name, text) in [
            ("edition-c-summary.tsv", summary),
            ("edition-c-missing.tsv", details),
            ("edition-c-functions.tsv", function_details),
        ] {
            let path = path.join(name);
            std::fs::write(&path, text).map_err(|error| format!("{}: {error}", path.display()))?;
            println!(
                "report={}",
                path.strip_prefix(root).unwrap_or(&path).display()
            );
        }
    } else {
        print!("{details}{function_details}");
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
        assert_eq!(rows[0].english_bytes, 24);
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
        let script = root.path().join(target.script());
        std::fs::create_dir_all(script.parent().unwrap()).unwrap();
        std::fs::write(&script, "SECTIONS {}\n").unwrap();
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
        assert!(snapshot(root.path(), target).unwrap().pending.is_none());
        std::fs::create_dir_all(output.join("overlays")).unwrap();
        std::fs::write(output.join("overlays/resource_36f.lz"), "leftover stream").unwrap();
        std::fs::write(output.join("overlays/resource_36f.map"), "leftover map").unwrap();
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
            "#if defined(TBS_EDITION_EN)\nvoid EnglishOnly(void) {}\n#endif\nvoid Both(void) {}\n",
        );
        let placement = Placement {
            image: "main".into(),
            source: "games/COMMON/SRC/A.C".into(),
        };
        let baseline = Snapshot {
            functions: [
                ((placement.clone(), "EnglishOnly".into()), 16),
                ((placement.clone(), "Both".into()), 20),
            ]
            .into(),
            ..Snapshot::default()
        };
        let own = Snapshot::default();
        let rows = missing_functions(
            root.path(),
            target_for(DecompTargetId::TbsJa),
            &baseline,
            &own,
            &mut Definitions::new(),
        )
        .unwrap();
        assert_eq!(rows.len(), 2);
        assert!(
            rows.iter()
                .find(|row| row.name == "EnglishOnly")
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
        assert!(text.contains("EnglishOnly\t16\tabsent-in-edition-source"));
        assert!(text.contains("Both\t20\tmissing-placement"));
        assert_eq!(baseline.functions.len(), 2);
    }
}
