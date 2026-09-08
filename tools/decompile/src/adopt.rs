//! Adoption of an exact candidate: the owner's source lands under
//! `games/gs1/src`, the registers learn its name and path, the retained
//! records inside the span retire, and the overlay placeholder is applied
//! through `overlay adopt`. Every step refuses before it mutates when the
//! candidate is not exact or the span overlaps another registered region.
//! A main-image owner lands the same records without a placeholder, and
//! writes nothing until `--apply`.

use crate::owners::{self, modules, parse_owner, score, tool_command};
use compiler_core::source_paths::{SourceOwner, SourcePaths};
use serde_json::Value;
use std::path::{Path, PathBuf};
use std::process::Command;

pub struct Request<'a> {
    pub owner: &'a str,
    pub span: Option<u32>,
    pub name: Option<&'a str>,
    pub path: Option<&'a str>,
    pub source: Option<&'a Path>,
    /// Main-image landings verify by default and only write when asked.
    pub apply: bool,
}

/// `FieldScene_RunFlagGatedActorDialogue` becomes `flag_gated_actor_dialogue`.
pub fn slug(name: &str) -> String {
    let stem = name.strip_prefix("FieldScene_").unwrap_or(name);
    let mut out = String::new();
    for (i, c) in stem.chars().enumerate() {
        if c.is_ascii_uppercase() {
            if i > 0 {
                out.push('_');
            }
            out.push(c.to_ascii_lowercase());
        } else {
            out.push(c);
        }
    }
    out.strip_prefix("run_").map(str::to_string).unwrap_or(out)
}

/// The registered name, or the first free `FieldScene_RunScene<res>Sequence<L>`.
fn derive_name(root: &Path, sources: &SourcePaths, overlay: &str, owner: SourceOwner) -> String {
    if let Some(name) = sources.registered_name(owner) {
        return name.to_string();
    }
    let resource = overlay.strip_prefix("resource_").unwrap_or(overlay);
    let taken: Vec<String> = sources
        .registered_owners()
        .filter(|o| o.overlay_id().as_deref() == Some(overlay))
        .filter_map(|o| sources.registered_name(o).map(str::to_string))
        .collect();
    // The letter must be free as a name and as a source path: an earlier
    // owner may hold the path under a different registered name.
    ('A'..='Z')
        .map(|letter| format!("FieldScene_RunScene{resource}Sequence{letter}"))
        .find(|candidate| {
            let letter = candidate.chars().last().unwrap_or('a').to_ascii_lowercase();
            let path = root.join(format!(
                "games/gs1/src/overlays/scene_primary_script/run_scene_{resource}_sequence_{letter}.c"
            ));
            !taken.contains(candidate) && !path.exists()
        })
        .unwrap_or_else(|| format!("FieldScene_RunScene{resource}Sequence"))
}

fn read_json(path: &Path) -> Result<(Value, bool), String> {
    let text = std::fs::read_to_string(path).map_err(|e| format!("{}: {e}", path.display()))?;
    let value = serde_json::from_str(&text).map_err(|e| format!("{}: {e}", path.display()))?;
    Ok((value, text.ends_with('\n')))
}

fn write_json(path: &Path, value: &Value, pretty: bool, newline: bool) -> Result<(), String> {
    let mut text = if pretty {
        serde_json::to_string_pretty(value)
    } else {
        serde_json::to_string(value)
    }
    .map_err(|e| e.to_string())?;
    if newline {
        text.push('\n');
    }
    std::fs::write(path, text).map_err(|e| format!("{}: {e}", path.display()))
}

fn set_source(record: &mut Value, name: &str, source: &str) {
    if !record.is_object() {
        *record = serde_json::json!({});
    }
    record["name"] = name.into();
    record["source"] = source.into();
}

fn git(root: &Path, args: &[&str]) -> Result<(), String> {
    let status = Command::new("git")
        .current_dir(root)
        .args(args)
        .status()
        .map_err(|e| format!("git: {e}"))?;
    if status.success() {
        Ok(())
    } else {
        Err(format!("git {} failed", args.join(" ")))
    }
}

fn run_tool(root: &Path, tool: &str, args: &[&str]) -> Result<String, String> {
    let output = tool_command(root, tool)
        .current_dir(root)
        .args(args)
        .output()
        .map_err(|e| format!("{tool}: {e}"))?;
    Ok(format!(
        "{}{}",
        String::from_utf8_lossy(&output.stdout),
        String::from_utf8_lossy(&output.stderr)
    ))
}

/// The retained evidence a main-image owner takes with it: its assembly and
/// its reconstruction draft both retire when the source lands in production.
fn main_retirements(root: &Path, stem: &str) -> Vec<PathBuf> {
    [
        format!("games/gs1/asm/{stem}.s"),
        format!("games/gs1/recon/en/main/{stem}.c"),
    ]
    .into_iter()
    .map(|relative| root.join(relative))
    .filter(|path| path.exists())
    .collect()
}

/// Writes one main-image landing: the source lands in production, the
/// register learns the owner's name and path, and its dossier, assembly and
/// draft retire. Only the records actually changed are staged.
fn write_landing(
    root: &Path,
    owner: SourceOwner,
    name: &str,
    relative: &str,
    candidate: &Path,
    retiring: &[PathBuf],
    destination: &Path,
) -> Result<Vec<String>, String> {
    let mut report = Vec::new();
    if let Some(parent) = destination.parent() {
        std::fs::create_dir_all(parent).map_err(|e| format!("{}: {e}", parent.display()))?;
    }
    std::fs::copy(candidate, destination).map_err(|e| format!("{}: {e}", destination.display()))?;
    let manifest = root.join("games/gs1/source-paths.json");
    let (mut register, _) = read_json(&manifest)?;
    set_source(&mut register["owners"][owner.id()], name, relative);
    write_json(&manifest, &register, true, true)?;
    let mut staged = vec![destination.to_path_buf(), manifest];
    let dossiers = root.join("games/gs1/recon/en/dossiers.json");
    let (mut records, newline) = read_json(&dossiers)?;
    if records["records"]
        .as_object_mut()
        .and_then(|map| map.shift_remove(&owner.id()))
        .is_some()
    {
        write_json(&dossiers, &records, false, newline)?;
        report.push("dossiers removed: 1".to_string());
        staged.push(dossiers);
    }
    for path in retiring {
        git(root, &["rm", "-q", &path.to_string_lossy()])?;
        report.push(format!("retired {}", path.display()));
    }
    let staged: Vec<String> = staged
        .iter()
        .map(|path| path.to_string_lossy().into_owned())
        .collect();
    let mut args = vec!["add"];
    args.extend(staged.iter().map(String::as_str));
    git(root, &args)?;
    report.push(format!("adopted {} as {name} at {relative}", owner.id()));
    Ok(report)
}

/// The records a landing overwrites, kept so a failure can put them back.
/// A landing touches five things at once, and `check owners` only speaks
/// afterwards, so a half-applied landing has to be undoable.
struct Landing {
    register: (PathBuf, Vec<u8>),
    dossiers: (PathBuf, Vec<u8>),
    /// The production file's own bytes when the landing found one there, so
    /// an overwrite is put back rather than merely left in place.
    destination: (PathBuf, Option<Vec<u8>>),
    /// Directories the landing had to create, deepest first, so an undone
    /// landing does not leave an empty subsystem behind.
    created: Vec<PathBuf>,
    /// The retired records' own bytes. Restoring them from the repository
    /// would bring back the committed version and lose an uncommitted edit,
    /// so what was on disk is what goes back.
    retiring: Vec<(PathBuf, Option<Vec<u8>>)>,
}

impl Landing {
    fn read(root: &Path, destination: &Path, retiring: &[PathBuf]) -> Result<Self, String> {
        let read = |relative: &str| -> Result<(PathBuf, Vec<u8>), String> {
            let path = root.join(relative);
            let bytes = std::fs::read(&path).map_err(|e| format!("{}: {e}", path.display()))?;
            Ok((path, bytes))
        };
        Ok(Self {
            register: read("games/gs1/source-paths.json")?,
            dossiers: read("games/gs1/recon/en/dossiers.json")?,
            destination: (destination.to_path_buf(), std::fs::read(destination).ok()),
            created: destination
                .ancestors()
                .skip(1)
                .take_while(|directory| {
                    directory.starts_with(root.join("games/gs1/src")) && !directory.exists()
                })
                .map(Path::to_path_buf)
                .collect(),
            retiring: retiring
                .iter()
                .map(|path| (path.clone(), std::fs::read(path).ok()))
                .collect(),
        })
    }

    /// Every step is the undo of one the landing made. What could not be put
    /// back is named, so a rollback never fails silently.
    fn undo(&self, root: &Path) -> Vec<String> {
        let mut unrestored = Vec::new();
        let mut restore = |path: &Path, bytes: Option<&Vec<u8>>| {
            let outcome = match bytes {
                Some(bytes) => {
                    // Retiring a record empties, and so removes, its directory.
                    path.parent()
                        .map_or(Ok(()), std::fs::create_dir_all)
                        .and_then(|()| std::fs::write(path, bytes))
                }
                None => std::fs::remove_file(path),
            };
            if let Err(error) = outcome {
                unrestored.push(format!("{}: {error}", path.display()));
            }
        };
        restore(&self.register.0, Some(&self.register.1));
        restore(&self.dossiers.0, Some(&self.dossiers.1));
        restore(&self.destination.0, self.destination.1.as_ref());
        for (path, bytes) in &self.retiring {
            if let Some(bytes) = bytes {
                restore(path, Some(bytes));
            }
        }
        for path in self.retiring.iter().map(|(path, _)| path).chain([
            &self.destination.0,
            &self.register.0,
            &self.dossiers.0,
        ]) {
            let _ = git(root, &["reset", "-q", "--", &path.to_string_lossy()]);
        }
        for directory in &self.created {
            let _ = std::fs::remove_dir(directory);
        }
        unrestored
    }
}

/// Reports a failed landing, naming anything the rollback could not undo.
fn rolled_back(root: &Path, backup: &Landing, error: &str) -> String {
    match backup.undo(root).as_slice() {
        [] => format!("{error}; the landing was rolled back"),
        left => format!(
            "{error}; the landing was rolled back except: {}",
            left.join("; ")
        ),
    }
}

/// Writes one main-image landing and hands back what it overwrote, so the
/// caller can undo it if the owner check that follows disagrees. A failure
/// inside the landing undoes itself before it returns.
fn land_main_records(
    root: &Path,
    owner: SourceOwner,
    name: &str,
    relative: &str,
    candidate: &Path,
    retiring: &[PathBuf],
) -> Result<(Vec<String>, Landing), String> {
    let destination = root.join("games/gs1/src").join(relative);
    let backup = Landing::read(root, &destination, retiring)?;
    match write_landing(
        root,
        owner,
        name,
        relative,
        candidate,
        retiring,
        &destination,
    ) {
        Ok(report) => Ok((report, backup)),
        Err(error) => Err(rolled_back(root, &backup, &error)),
    }
}

/// A main-image landing, in the record shape the maintained history uses.
/// The candidate is verified first and nothing is written without `--apply`.
/// A member of a declared translation unit is refused: a shared owner is
/// installed with its unit, not through an isolated adoption.
fn adopt_main(root: &Path, request: &Request) -> Result<Vec<String>, String> {
    let owner = SourceOwner::parse_argument(request.owner)?;
    let stem = owner.legacy_stem();
    if let Some(unit) = compiler_core::translation_units::TranslationUnits::load(root)?
        .unit_for_game_owner("gs1", owner)
    {
        return Err(format!(
            "{} is declared by translation unit {}; install a shared owner with its unit, not through an isolated adoption",
            owner.id(),
            unit.id
        ));
    }
    let span = match request.span {
        Some(span) => span,
        None => diff::render::region_size(root, owner.address())
            .and_then(|size| u32::try_from(size).ok())
            .ok_or_else(|| {
                format!("no owner-size entry for {stem} in the claimed or asm build manifests; run make build-claimed or pass --span BYTES")
            })?,
    };
    let record =
        read_json(&root.join("games/gs1/source-paths.json"))?.0["owners"][owner.id()].clone();
    // An owner whose register already names a production source is verified,
    // never landed again, whatever --path was asked for: a second landing
    // would copy reviewed C to a new path and leave the old file behind.
    let registered = record["source"].as_str().map(str::to_string);
    if let Some(existing) = registered.as_deref() {
        // Refused whenever a landing is even proposed: with --apply, and in a
        // dry run that plans a second path for C already in production.
        if request.apply || request.path.is_some_and(|path| path != existing) {
            return Err(format!(
                "{} is already adopted as C at games/gs1/src/{existing}; nothing written",
                request.owner
            ));
        }
    }
    let name = request
        .name
        .map(str::to_string)
        .or_else(|| record["name"].as_str().map(str::to_string))
        .ok_or_else(|| format!("{} has no registered name; pass --name NAME", owner.id()))?;
    // The production directory expresses an evidenced subsystem, which the
    // owner's address and name cannot establish on their own.
    let relative = request
        .path
        .map(str::to_string)
        .or_else(|| registered.clone())
        .ok_or_else(|| {
            format!(
                "{} has no registered source path; pass --path SUBSYSTEM/FILE.c",
                owner.id()
            )
        })?;
    let production = Path::new(&relative);
    if production.is_absolute()
        || production
            .components()
            .any(|part| part == std::path::Component::ParentDir)
        || production.extension().and_then(|kind| kind.to_str()) != Some("c")
    {
        return Err(format!(
            "{relative}: a production path is a relative C file under games/gs1/src"
        ));
    }
    let destination = root.join("games/gs1/src").join(production);
    let landed = registered.is_some() && destination.is_file();
    let draft = root.join(format!("games/gs1/recon/en/main/{stem}.c"));
    let candidate = match request.source {
        Some(source) => source.to_path_buf(),
        None if draft.is_file() => draft,
        None if landed => destination.clone(),
        None => {
            return Err(format!(
                "no candidate for {}; pass --source FILE",
                owner.id()
            ))
        }
    };
    let result = score(root, &candidate, request.owner, span)?;
    let mut report = vec![format!(
        "candidate={} reference={} differing_halfwords={} span={span}",
        result.candidate, result.reference, result.differing
    )];
    if result.differing != 0 {
        return Err(format!(
            "{} is not exact ({} differing halfwords); nothing adopted",
            request.owner, result.differing
        ));
    }
    let retiring = main_retirements(root, &stem);
    if !request.apply {
        report.push(match landed {
            true => format!("already registered as {name} at {relative}; nothing to record"),
            false => format!("would register {} as {name} at {relative}", owner.id()),
        });
        for path in &retiring {
            report.push(format!("would retire {}", path.display()));
        }
        report.push("adopt=verified".to_string());
        return Ok(report);
    }
    let (landed_lines, backup) =
        land_main_records(root, owner, &name, &relative, &candidate, &retiring)?;
    report.extend(landed_lines);
    let verdict = match run_tool(root, "check", &["owners"]) {
        Ok(checked) => checked.trim().lines().last().unwrap_or("").to_string(),
        Err(error) => return Err(rolled_back(root, &backup, &error)),
    };
    if !verdict.contains("owner registers ok") {
        return Err(rolled_back(
            root,
            &backup,
            &format!("owner check failed after adoption: {verdict}"),
        ));
    }
    report.push(verdict);
    report.push("adopt=applied".to_string());
    Ok(report)
}

/// Adopts one owner. Returns the lines worth reporting.
pub fn adopt(root: &Path, request: &Request) -> Result<Vec<String>, String> {
    if SourceOwner::parse_argument(request.owner)?.is_main() {
        return adopt_main(root, request);
    }
    if request.apply {
        return Err(
            "--apply is the main-image landing switch; an overlay adoption applies as it lands"
                .into(),
        );
    }
    let (overlay, entry) = parse_owner(request.owner)?;
    let owner = SourceOwner::parse(&format!("{overlay}:{entry:08x}"))?;
    let span = owners::span_for(root, &overlay, entry, request.span)?;
    let end = entry + span;
    // A retained region that lies wholly inside the span and has no source
    // is this function's own bytes, a literal pool or a tail the register
    // split off; an exact candidate absorbs it. Anything else refuses.
    let overlapping: Vec<String> = modules(root)?
        .into_iter()
        .filter(|m| m.overlay == overlay && m.entry != entry)
        .filter(|m| m.entry < end && m.entry + m.span > entry)
        .filter(|m| m.registered || m.entry < entry || m.entry + m.span > end)
        .map(|m| format!("{} ({} bytes, {})", m.key(), m.span, m.kind))
        .collect();
    if !overlapping.is_empty() {
        return Err(format!(
            "{} overlaps registered regions; retire or shorten them first: {}",
            request.owner,
            overlapping.join("; ")
        ));
    }
    let sources = SourcePaths::load(root)?;
    // An owner that already has its exact source is not adopted again:
    // writing the unit would replace reviewed C and re-register its path.
    if let Some(existing) = sources.mapped_source_path(owner) {
        if existing.exists() {
            return Err(format!(
                "{} is already adopted as C at {}; nothing written",
                request.owner,
                existing.display()
            ));
        }
    }
    let name = match request.name {
        Some(name) => name.to_string(),
        None => derive_name(root, &sources, &overlay, owner),
    };
    let resource = overlay.strip_prefix("resource_").unwrap_or(&overlay);
    let relative = match request.path {
        Some(path) => path.to_string(),
        None => {
            let stem = slug(&name);
            // A derived `FieldScene_RunScene<res>Sequence<L>` name repeats the
            // resource the path already carries.
            let stem = stem
                .strip_prefix(&format!("scene{resource}_"))
                .map(str::to_string)
                .unwrap_or(stem);
            format!("overlays/scene_primary_script/run_scene_{resource}_{stem}.c")
        }
    };
    let unit = match request.source {
        Some(source) => std::fs::read_to_string(source)
            .map_err(|e| format!("{}: {e}", source.display()))?
            .replace(&format!("Lifted_{entry:08x}"), &name),
        None => crate::lift_owner(root, request.owner, Some(span), Some(&name))?.0,
    };
    let destination = root.join("games/gs1/src").join(&relative);
    let existed = destination.exists();
    if let Some(parent) = destination.parent() {
        std::fs::create_dir_all(parent).map_err(|e| format!("{}: {e}", parent.display()))?;
    }
    std::fs::write(&destination, &unit).map_err(|e| format!("{}: {e}", destination.display()))?;
    let result = score(root, &destination, request.owner, span)?;
    let mut report = vec![format!(
        "candidate={} reference={} differing_halfwords={} span={span}",
        result.candidate, result.reference, result.differing
    )];
    if result.differing != 0 {
        if !existed {
            let _ = std::fs::remove_file(&destination);
        }
        return Err(format!(
            "{} is not exact ({} differing halfwords); nothing adopted",
            request.owner, result.differing
        ));
    }

    // The source register: keep an existing name, record the path.
    let manifest = root.join("games/gs1/source-paths.json");
    let (mut register, _) = read_json(&manifest)?;
    let owners_map = register
        .get_mut("owners")
        .and_then(Value::as_object_mut)
        .ok_or("source-paths.json: owners must be an object")?;
    let registered_name = owners_map
        .get(&owner.id())
        .and_then(|v| v.get("name"))
        .and_then(Value::as_str)
        .map(str::to_string)
        .unwrap_or_else(|| name.clone());
    set_source(
        owners_map.entry(owner.id()).or_insert(Value::Null),
        &registered_name,
        &relative,
    );
    write_json(&manifest, &register, true, true)?;

    // The owner and every absorbed region lose their records. This reads the
    // retained regions before the ones inside the span are removed below;
    // afterwards they are gone and nothing would be reported as absorbed,
    // leaving their names in the register with no evidence behind them.
    let mut retired: Vec<SourceOwner> = vec![owner];
    for m in modules(root)? {
        if m.overlay == overlay && m.entry != entry && m.entry >= entry && m.entry + m.span <= end {
            retired.push(SourceOwner::parse(&m.key())?);
            report.push(format!("absorbed {}", m.key()));
        }
    }

    // Retained regions inside the span retire.
    let assembly = root.join("games/gs1/semantic/overlay-assembly.json");
    let (mut regions, _) = read_json(&assembly)?;
    if let Some(list) = regions.get_mut("regions").and_then(Value::as_array_mut) {
        let before = list.len();
        list.retain(|region| {
            let same = region["overlay"].as_str() == Some(overlay.as_str());
            let start = region["start"]
                .as_str()
                .and_then(|s| u32::from_str_radix(s.trim_start_matches("0x"), 16).ok());
            !(same && start.is_some_and(|s| s >= entry && s < end))
        });
        report.push(format!("assembly regions {before} -> {}", list.len()));
    }
    write_json(&assembly, &regions, true, true)?;

    // An absorbed region's name leaves the source register: its bytes are
    // this function's, and an owner with a name but no source would be
    // asked for assembly evidence it no longer has.
    if retired.len() > 1 {
        let (mut register, _) = read_json(&manifest)?;
        if let Some(map) = register.get_mut("owners").and_then(Value::as_object_mut) {
            for gone in retired.iter().skip(1) {
                map.shift_remove(&gone.id());
            }
        }
        write_json(&manifest, &register, true, true)?;
    }
    let dossiers = root.join("games/gs1/recon/en/dossiers.json");
    let (mut records, newline) = read_json(&dossiers)?;
    if let Some(map) = records.get_mut("records").and_then(Value::as_object_mut) {
        let mut removed = 0;
        for gone in &retired {
            if map.shift_remove(&gone.id()).is_some() {
                removed += 1;
            }
        }
        if removed > 0 {
            report.push(format!("dossiers removed: {removed}"));
            write_json(&dossiers, &records, false, newline)?;
        }
    }

    let stems: Vec<String> = retired.iter().map(|o| o.legacy_stem()).collect();
    let unmatchable = root.join("games/gs1/semantic/unmatchable.json");
    let (mut withdrawn, _) = read_json(&unmatchable)?;
    if let Some(list) = withdrawn
        .get_mut("unmatchable")
        .and_then(Value::as_array_mut)
    {
        let before = list.len();
        list.retain(|entry| {
            !entry["owner"]
                .as_str()
                .is_some_and(|o| stems.iter().any(|s| s == o))
        });
        if list.len() != before {
            report.push("unmatchable entry removed".to_string());
            write_json(&unmatchable, &withdrawn, true, true)?;
        }
    }

    for stem in &stems {
        let draft = root.join(format!("games/gs1/recon/en/overlays/{stem}.c"));
        if draft.exists() {
            git(root, &["rm", "-q", &draft.to_string_lossy()])?;
            report.push(format!("draft removed: {stem}"));
        }
    }

    let applied = run_tool(
        root,
        "overlay",
        &[
            "adopt",
            request.owner,
            "--source",
            &destination.to_string_lossy(),
            "--span",
            &span.to_string(),
            "--apply",
        ],
    )?;
    let last = applied.trim().lines().last().unwrap_or("").to_string();
    if !last.contains("adopt=applied") {
        return Err(format!("overlay adopt did not apply: {last}"));
    }
    report.push(last);

    let overlay_source: PathBuf = root.join(format!("games/gs1/assets/code/{overlay}_overlay.s"));
    let mut staged: Vec<String> = vec![
        destination.to_string_lossy().into_owned(),
        manifest.to_string_lossy().into_owned(),
        assembly.to_string_lossy().into_owned(),
        dossiers.to_string_lossy().into_owned(),
        unmatchable.to_string_lossy().into_owned(),
        overlay_source.to_string_lossy().into_owned(),
    ];
    let mut args = vec!["add".to_string()];
    args.append(&mut staged);
    let args: Vec<&str> = args.iter().map(String::as_str).collect();
    git(root, &args)?;

    let checked = run_tool(root, "check", &["owners"])?;
    let verdict = checked.trim().lines().last().unwrap_or("").to_string();
    if !verdict.contains("owner registers ok") {
        return Err(format!("owner check failed after adoption: {verdict}"));
    }
    report.push(verdict);
    report.push(format!("adopted {} as {name} at {relative}", request.owner));
    Ok(report)
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn source_registration_preserves_call_via() {
        let mut record = serde_json::json!({
            "name": "SceneActor_MoveAndRedraw",
            "call_via": "020068d6"
        });
        set_source(&mut record, "SceneActor_MoveAndRedraw", "scene/move.c");
        assert_eq!(record["call_via"], "020068d6");
        assert_eq!(record["source"], "scene/move.c");

        let mut legacy = Value::String("scene/old.c".into());
        set_source(&mut legacy, "SceneActor_MoveAndRedraw", "scene/move.c");
        assert_eq!(legacy["name"], "SceneActor_MoveAndRedraw");
        assert_eq!(legacy["source"], "scene/move.c");
    }

    /// The retained evidence that retires with a landing, read from the
    /// fixture tree so that adopting or renaming a real owner cannot fail it.
    /// The refusals these two pinned owners used to cover are asserted by
    /// `a_main_landing_refuses_before_it_compiles` over derived owners.
    #[test]
    fn a_main_owner_retires_its_assembly_and_its_draft() {
        let (tree, assembly, draft) = landing_fixture(true);
        let root = tree.path();
        assert_eq!(main_retirements(root, "0808e4b4"), vec![assembly, draft]);
        assert!(main_retirements(root, "08000000").is_empty());
    }

    fn request<'a>(
        owner: &'a str,
        name: Option<&'a str>,
        path: Option<&'a str>,
        apply: bool,
    ) -> Request<'a> {
        Request {
            owner,
            span: None,
            name,
            path,
            source: None,
            apply,
        }
    }

    /// Owners are chosen from the maintained records rather than pinned, so
    /// adopting, renaming or regrouping any one of them cannot fail these.
    fn register(root: &Path) -> Value {
        read_json(&root.join("games/gs1/source-paths.json"))
            .unwrap()
            .0
    }

    /// A standalone main owner the register already places in production.
    fn a_landed_main_owner(root: &Path) -> (String, String) {
        let units = compiler_core::translation_units::TranslationUnits::load(root).unwrap();
        register(root)["owners"]
            .as_object()
            .unwrap()
            .iter()
            .find_map(|(id, record)| {
                let source = record["source"].as_str()?;
                let owner = SourceOwner::parse_argument(id).ok()?;
                (owner.is_main()
                    && units.unit_for_game_owner("gs1", owner).is_none()
                    && root.join("games/gs1/src").join(source).is_file())
                .then(|| (id.clone(), source.to_string()))
            })
            .expect("a landed standalone main owner")
    }

    /// A main owner a declared translation unit already composes.
    fn a_main_unit_member(root: &Path) -> (String, String) {
        compiler_core::translation_units::TranslationUnits::load(root)
            .unwrap()
            .units
            .iter()
            .filter(|unit| unit.game == "gs1" && unit.overlay.is_none())
            .find_map(|unit| {
                let member = unit.owners.first()?;
                Some((format!("main:{:08x}", member.address), unit.id.clone()))
            })
            .expect("a declared main translation unit")
    }

    /// A main draft the register does not name at all.
    fn an_unnamed_main_owner(root: &Path) -> String {
        let register = register(root);
        let owners = register["owners"].as_object().unwrap();
        let units = compiler_core::translation_units::TranslationUnits::load(root).unwrap();
        let mut drafts: Vec<String> = std::fs::read_dir(root.join("games/gs1/recon/en/main"))
            .unwrap()
            .filter_map(Result::ok)
            .filter_map(|entry| {
                let path = entry.path();
                let stem = path.file_stem()?.to_str()?.to_string();
                let id = format!("main:{stem}");
                // A unit member is refused earlier, on a different message.
                let owner = SourceOwner::parse_argument(&id).ok()?;
                (path.extension()? == "c"
                    && !owners.contains_key(&id)
                    && units.unit_for_game_owner("gs1", owner).is_none())
                .then_some(stem)
            })
            .collect();
        drafts.sort();
        format!("main:{}", drafts.first().expect("an unnamed main draft"))
    }

    /// The refusals a main landing makes before it compiles anything.
    #[test]
    fn a_main_landing_refuses_before_it_compiles() {
        let root = owners::root();
        let (member, unit) = a_main_unit_member(&root);
        let shared = adopt(&root, &request(&member, None, None, false)).unwrap_err();
        assert!(
            shared.contains(&format!("declared by translation unit {unit}")),
            "{shared}"
        );

        let unnamed = an_unnamed_main_owner(&root);
        let no_name = adopt(&root, &request(&unnamed, None, None, false)).unwrap_err();
        assert!(no_name.contains("has no registered name"), "{no_name}");
        let no_path = adopt(
            &root,
            &request(&unnamed, Some("Runtime_Probe"), None, false),
        )
        .unwrap_err();
        assert!(
            no_path.contains("has no registered source path"),
            "{no_path}"
        );
        let escaping = adopt(
            &root,
            &request(
                &unnamed,
                Some("Runtime_Probe"),
                Some("../../elsewhere.c"),
                false,
            ),
        )
        .unwrap_err();
        assert!(
            escaping.contains("relative C file under games/gs1/src"),
            "{escaping}"
        );

        // An owner already in production is refused however --path is spelled,
        // and a dry run may not advertise a second path for it either: the
        // register, not the requested destination, decides.
        let (landed, source) = a_landed_main_owner(&root);
        let refusal =
            format!("{landed} is already adopted as C at games/gs1/src/{source}; nothing written");
        for (path, apply) in [
            (None, true),
            (Some("menu/elsewhere.c"), true),
            (Some("menu/elsewhere.c"), false),
        ] {
            assert_eq!(
                adopt(&root, &request(&landed, None, path, apply)).unwrap_err(),
                refusal
            );
        }
        assert!(!root.join("games/gs1/src/menu/elsewhere.c").exists());

        let overlay =
            adopt(&root, &request("resource_378:0200088c", None, None, true)).unwrap_err();
        assert!(
            overlay.contains("--apply is the main-image landing switch"),
            "{overlay}"
        );
    }

    /// A landing scores the candidate before it records anything, and a dry
    /// run records nothing at all. The inexact candidate is a stub for the
    /// owner, so no maintained draft becoming exact can fail this.
    #[test]
    fn a_main_landing_verifies_before_it_writes() {
        let root = owners::root();
        let (landed, source) = a_landed_main_owner(&root);
        let address = SourceOwner::parse_argument(&landed).unwrap().address_stem();
        let stub = tempfile::Builder::new().suffix(".c").tempfile().unwrap();
        std::fs::write(
            stub.path(),
            format!("#include \"types.h\"\n\nvoid Func_{address}(void)\n{{\n}}\n"),
        )
        .unwrap();
        let manifest = root.join("games/gs1/source-paths.json");
        let before = std::fs::read(&manifest).unwrap();

        let mut stubbed = request(&landed, None, None, false);
        stubbed.source = Some(stub.path());
        let inexact = adopt(&root, &stubbed).unwrap_err();
        assert!(inexact.contains("is not exact ("), "{inexact}");

        let verified = adopt(&root, &request(&landed, None, None, false)).unwrap();
        assert!(
            verified[0].contains("differing_halfwords=0"),
            "{:?}",
            verified[0]
        );
        assert!(
            verified
                .iter()
                .any(|line| line.contains(&format!("already registered as"))
                    && line.contains(&source))
        );
        assert_eq!(verified.last().map(String::as_str), Some("adopt=verified"));
        assert_eq!(std::fs::read(&manifest).unwrap(), before);
    }

    /// A fixture landing tree for `main:0808e4b4`.    /// A fixture landing tree for `main:0808e4b4`; `tracked` commits the
    /// retained records a landing has to retire.
    fn landing_fixture(tracked: bool) -> (tempfile::TempDir, PathBuf, PathBuf) {
        let tree = tempfile::tempdir().unwrap();
        let root = tree.path().to_path_buf();
        let write = |relative: &str, text: &str| {
            let path = root.join(relative);
            std::fs::create_dir_all(path.parent().expect("fixture parent")).unwrap();
            std::fs::write(&path, text).unwrap();
            path
        };
        write(
            "games/gs1/source-paths.json",
            r#"{"owners":{"main:0808e4b4":{"name":"BattleEffect_FindMatchingEvent"}}}"#,
        );
        write(
            "games/gs1/recon/en/dossiers.json",
            r#"{"records":{"main:0808e4b4":{"owner_bytes":292},"main:0808e23c":{}}}"#,
        );
        let assembly = write("games/gs1/asm/0808e4b4.s", "@ retained\n");
        let draft = write(
            "games/gs1/recon/en/main/0808e4b4.c",
            "void BattleEffect_FindMatchingEvent(void) {}\n",
        );
        git(&root, &["init", "-q", "-b", "fixture"]).unwrap();
        if tracked {
            git(&root, &["add", "-A"]).unwrap();
            git(
                &root,
                &[
                    "-c",
                    "user.name=fixture",
                    "-c",
                    "user.email=fixture@example.com",
                    "commit",
                    "-q",
                    "-m",
                    "fixture",
                ],
            )
            .unwrap();
        }
        (tree, assembly, draft)
    }

    const LANDED_PATH: &str = "battle/effects/find_matching_event.c";

    fn land(root: &Path, draft: &Path, retiring: &[PathBuf]) -> Result<Vec<String>, String> {
        land_main_records(
            root,
            SourceOwner::parse_argument("main:0808e4b4").unwrap(),
            "BattleEffect_FindMatchingEvent",
            LANDED_PATH,
            draft,
            retiring,
        )
        .map(|(report, _)| report)
    }

    /// A whole main-image landing written into a fixture tree and read back,
    /// in the record shape `6eb123ae3` and `ab2ac2533` use: production source,
    /// register entry, dossier removal, and the assembly and draft retired.
    #[test]
    fn a_main_landing_writes_the_recorded_shape() {
        let (tree, assembly, draft) = landing_fixture(true);
        let root = tree.path();
        let retiring = main_retirements(root, "0808e4b4");
        assert_eq!(retiring, vec![assembly.clone(), draft.clone()]);
        let report = land(root, &draft, &retiring).unwrap_or_else(|error| panic!("{error}"));

        let read = |path: &str| std::fs::read_to_string(root.join(path)).unwrap();
        assert_eq!(
            read(&format!("games/gs1/src/{LANDED_PATH}")),
            "void BattleEffect_FindMatchingEvent(void) {}\n"
        );
        let register: Value = serde_json::from_str(&read("games/gs1/source-paths.json")).unwrap();
        assert_eq!(
            register["owners"]["main:0808e4b4"],
            serde_json::json!({
                "name": "BattleEffect_FindMatchingEvent",
                "source": LANDED_PATH
            })
        );
        let dossiers: Value =
            serde_json::from_str(&read("games/gs1/recon/en/dossiers.json")).unwrap();
        assert!(dossiers["records"]["main:0808e4b4"].is_null());
        assert!(dossiers["records"]["main:0808e23c"].is_object());
        assert!(!assembly.exists() && !draft.exists());
        assert_eq!(
            report,
            [
                "dossiers removed: 1".to_string(),
                format!("retired {}", assembly.display()),
                format!("retired {}", draft.display()),
                format!("adopted main:0808e4b4 as BattleEffect_FindMatchingEvent at {LANDED_PATH}"),
            ]
        );
    }

    /// The rollback the owner check reaches for once a landing has already
    /// succeeded. By then the retirements are staged deletions, so they have
    /// to come back from HEAD rather than from the index that recorded them.
    #[test]
    fn a_landing_undone_after_it_succeeded_puts_every_record_back() {
        let (tree, assembly, draft) = landing_fixture(true);
        let root = tree.path();
        let read = |path: &str| std::fs::read(root.join(path)).unwrap();
        let (register, dossiers) = (
            read("games/gs1/source-paths.json"),
            read("games/gs1/recon/en/dossiers.json"),
        );
        let destination = root.join("games/gs1/src").join(LANDED_PATH);
        let retiring = main_retirements(root, "0808e4b4");
        let (_, backup) = land_main_records(
            root,
            SourceOwner::parse_argument("main:0808e4b4").unwrap(),
            "BattleEffect_FindMatchingEvent",
            LANDED_PATH,
            &draft,
            &retiring,
        )
        .unwrap_or_else(|error| panic!("{error}"));
        assert!(destination.is_file() && !assembly.exists() && !draft.exists());

        assert_eq!(backup.undo(root), Vec::<String>::new());
        assert_eq!(read("games/gs1/source-paths.json"), register);
        assert_eq!(read("games/gs1/recon/en/dossiers.json"), dossiers);
        assert!(assembly.exists() && draft.exists());
        assert!(!destination.exists());
        // The subsystem directories the landing created go with it.
        assert!(!root.join("games/gs1/src").exists());
    }

    /// A landing touches five records before `check owners` can speak, so a
    /// failure part-way has to leave the tree as it found it. Here the
    /// retained records are untracked, so retiring them fails after the
    /// register, the dossier and the production source were already written.
    #[test]
    fn a_failed_landing_puts_every_record_back() {
        let (tree, assembly, draft) = landing_fixture(false);
        let root = tree.path();
        let before = |path: &str| std::fs::read(root.join(path)).unwrap();
        let (register, dossiers) = (
            before("games/gs1/source-paths.json"),
            before("games/gs1/recon/en/dossiers.json"),
        );
        // A landing overwrites whatever production held, so that file is part
        // of what has to come back.
        let destination = root.join("games/gs1/src").join(LANDED_PATH);
        std::fs::create_dir_all(destination.parent().unwrap()).unwrap();
        std::fs::write(&destination, "void Reviewed(void) {}\n").unwrap();

        let retiring = main_retirements(root, "0808e4b4");
        let error = land(root, &draft, &retiring).unwrap_err();
        assert!(error.contains("the landing was rolled back"), "{error}");
        assert!(!error.contains("except:"), "{error}");
        assert_eq!(before("games/gs1/source-paths.json"), register);
        assert_eq!(before("games/gs1/recon/en/dossiers.json"), dossiers);
        assert_eq!(
            std::fs::read_to_string(&destination).unwrap(),
            "void Reviewed(void) {}\n"
        );
        assert!(assembly.exists() && draft.exists());

        // With nothing there before, the rollback removes what it copied.
        std::fs::remove_file(&destination).unwrap();
        assert!(land(root, &draft, &retiring).is_err());
        assert!(!destination.exists());
    }

    #[test]
    fn slugs_follow_the_name() {
        assert_eq!(
            slug("FieldScene_RunFlagGatedActorDialogue"),
            "flag_gated_actor_dialogue"
        );
        assert_eq!(
            slug("FieldScene_ConfigurePairedActors"),
            "configure_paired_actors"
        );
        assert_eq!(slug("RunCallScript"), "call_script");
    }
}
