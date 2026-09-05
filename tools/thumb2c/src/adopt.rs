//! Adoption of an exact candidate: the owner's source lands under
//! `games/gs1/src`, the registers learn its name and path, the retained
//! records inside the span retire, and the overlay placeholder is applied
//! through `overlay adopt`. Every step refuses before it mutates when the
//! candidate is not exact or the span overlaps another registered region.

use crate::owners::{self, modules, parse_owner, score_extending, tool_command};
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

/// Adopts one owner. Returns the lines worth reporting.
pub fn adopt(root: &Path, request: &Request) -> Result<Vec<String>, String> {
    let (overlay, entry) = parse_owner(request.owner)?;
    let owner = SourceOwner::parse(&format!("{overlay}:{entry:08x}"))?;
    let span = match request.span {
        Some(span) => span,
        None => owners::span_for(root, &overlay, entry)?,
    };
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
    let result = score_extending(root, &destination, request.owner, span)?;
    // A pool past the registered end extends the span; the gap it fills was
    // checked to be unregistered when the extended score was taken.
    let span = result.extended.unwrap_or(span);
    let end = entry + span;
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
