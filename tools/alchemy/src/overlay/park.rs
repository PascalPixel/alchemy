pub(crate) use crate::compiler::overlay::placeholder_block;
use crate::compiler::{
    overlay::space_size,
    source_paths::{SourceOwner, SourcePaths},
    translation_units::TranslationUnits,
};
use crate::overlay::assembly::OVERLAY_BASE;
use crate::overlay::compile::assemble_overlay;
use crate::overlay::rom::canonical_overlay;
use crate::overlay::rom::CanonicalRom;
use crate::overlay::source::OverlaySource;
use crate::overlay::{
    listing_offsets, overlay_assembly, overlay_offset, region_lines, retained_source,
};
use std::fs;
use std::path::Path;
use std::process::Command;
use std::sync::atomic::{AtomicUsize, Ordering};
use std::sync::Mutex;
use tempfile::tempdir;
fn placeholder_address(line: &str) -> Option<i64> {
    i64::from_str_radix(
        line.trim().strip_prefix("AlchemyC_")?.strip_suffix(':')?,
        16,
    )
    .ok()
}
fn placeholder_addresses(lines: &[&str]) -> Vec<i64> {
    lines
        .iter()
        .filter_map(|line| placeholder_address(line))
        .collect()
}
fn git(root: &Path, arguments: &[&str]) -> Result<String, String> {
    let output = Command::new("git")
        .current_dir(root)
        .args(arguments)
        .output()
        .map_err(|error| format!("git failed: {error}"))?;
    if !output.status.success() {
        return Err(format!(
            "git {} failed: {}",
            arguments.join(" "),
            String::from_utf8_lossy(&output.stderr).trim()
        ));
    }
    String::from_utf8(output.stdout).map_err(|error| error.to_string())
}
fn pre_adoption_text(root: &Path, target: SourceOwner) -> Result<String, String> {
    let overlay = target.overlay_id().expect("overlay owner");
    let address = i64::from(target.address());
    let relative = format!("recon/tbs/raw/overlays/{overlay}_overlay.s");
    let tag = format!("AlchemyC_{address:08x}:");
    let log = git(
        root,
        &[
            "log",
            "--follow",
            "--format=@%H",
            "--name-only",
            "--",
            &relative,
        ],
    )?;
    let mut revisions: Vec<(String, String)> = Vec::new();
    let mut commit = String::new();
    for line in log.lines() {
        let line = line.trim();
        if let Some(sha) = line.strip_prefix('@') {
            commit = sha.to_string();
        } else if !line.is_empty() && !commit.is_empty() {
            revisions.push((commit.clone(), line.to_string()));
            commit.clear();
        }
    }
    revisions.reverse(); // oldest first
    let mut previous: Option<&(String, String)> = None;
    for revision in &revisions {
        let text =
            git(root, &["show", &format!("{}:{}", revision.0, revision.1)]).unwrap_or_default();
        if text.lines().any(|line| line.trim() == tag) {
            let (commit, name) = previous
                .ok_or_else(|| format!("{tag} is present in the first revision of {relative}"))?;
            return git(root, &["show", &format!("{commit}:{name}")]);
        }
        previous = Some(revision);
    }
    Err(format!("no revision of {relative} contains {tag}"))
}
fn label_use(
    text: &str,
) -> (
    std::collections::BTreeSet<String>,
    std::collections::BTreeSet<String>,
) {
    let mut defined = std::collections::BTreeSet::new();
    let mut referenced = std::collections::BTreeSet::new();
    for line in text.split('\n') {
        let code = line.split('@').next().unwrap_or("");
        let definition = code
            .trim()
            .strip_prefix(".L_")
            .and_then(|label| label.strip_suffix(':'))
            .filter(|label| label.len() == 8 && label.chars().all(|c| c.is_ascii_hexdigit()));
        if let Some(label) = definition {
            defined.insert(label.to_string());
            continue;
        }
        for (index, _) in code.match_indices(".L_") {
            let label = code[index + 3..]
                .chars()
                .take_while(|c| c.is_ascii_hexdigit())
                .collect::<String>();
            if label.len() == 8 {
                referenced.insert(label);
            }
        }
    }
    (defined, referenced)
}
fn define_dangling_labels(text: &str) -> String {
    let (defined, referenced) = label_use(text);
    let additions = referenced
        .difference(&defined)
        .map(|label| format!("\n.set .L_{label}, 0x{label}"))
        .collect::<String>();
    if additions.is_empty() {
        text.to_string()
    } else {
        format!("{text}{additions}\n")
    }
}
fn restore_label(text: &str, label: &str) -> Option<String> {
    let target = i64::from_str_radix(label, 16).ok()?;
    let lines: Vec<&str> = text.split('\n').collect();
    for (index, source) in lines.iter().enumerate() {
        let Some(base) = placeholder_address(source) else {
            continue;
        };
        let placeholder = placeholder_block(&lines, base)?;
        let mut cursor = base;
        for line in index + 1..placeholder.end {
            let body = lines[line].trim();
            if body.starts_with(".space ") {
                let value = space_size(body)?;
                if target > cursor && target < cursor + value {
                    let mut out = lines
                        .iter()
                        .map(|line| line.to_string())
                        .collect::<Vec<_>>();
                    out.splice(
                        line..=line,
                        [
                            format!("\t.space 0x{:x}", target - cursor),
                            format!(".L_{label}:"),
                            format!("\t.space 0x{:x}", cursor + value - target),
                        ],
                    );
                    return Some(out.join("\n"));
                }
                cursor += value;
            }
        }
    }
    None
}
fn region_text(text: &str, address: i64, span: i64) -> Result<Vec<String>, String> {
    let work = tempdir().map_err(|error| error.to_string())?;
    let path = work.path().join("historical.s");
    fs::write(&path, text).map_err(|error| error.to_string())?;
    let offsets = listing_offsets(&path)?;
    let (first, last) = region_lines(&offsets, address - OVERLAY_BASE, span)?;
    let lines: Vec<&str> = text.split('\n').collect();
    let slice = lines
        .get((first as usize - 1)..(last as usize))
        .ok_or_else(|| format!("lines {first}-{last} are outside the historical revision"))?;
    Ok(slice.iter().map(|line| line.to_string()).collect())
}
pub(crate) fn reference_bytes(
    root: &Path,
    target: SourceOwner,
    span: i64,
) -> Result<Vec<u8>, String> {
    let text = define_dangling_labels(&pre_adoption_text(root, target)?);
    let image = assemble_overlay(
        &OverlaySource::named(target.overlay_id().expect("overlay owner"), text),
        OVERLAY_BASE,
    )?;
    let start = overlay_offset(target);
    let end = start + span as usize;
    if end > image.len() {
        return Err("the region runs past the historical image".to_string());
    }
    Ok(image[start..end].to_vec())
}
type AuditResult = Result<Vec<String>, String>;
fn audit_with_rom(root: &Path, overlay: &str, rom: Option<&CanonicalRom>) -> AuditResult {
    let path = overlay_assembly(root, overlay);
    let assembly = fs::read_to_string(&path).map_err(|error| error.to_string())?;
    let lines: Vec<&str> = assembly.split('\n').collect();
    let addresses = placeholder_addresses(&lines);
    let placeholders = addresses
        .iter()
        .filter_map(|address| placeholder_block(&lines, *address).map(|row| (*address, row)))
        .collect::<Vec<_>>();
    let source_paths = SourcePaths::load(root)?;
    let mut findings = Vec::new();
    for &(address, _) in &placeholders {
        let owner = SourceOwner::parse(&format!("{overlay}:{address:08x}"))?;
        let source = source_paths.source_path(owner);
        if !source.exists() {
            findings.push(format!(
                "{overlay}:{address:08x}\tMISSING_SOURCE\t{}",
                source.display()
            ));
            continue;
        }
    }
    if !findings.is_empty() {
        return Ok(findings);
    }
    let image = match rom.map_or_else(
        || canonical_overlay(root, overlay),
        |rom| rom.overlay(overlay),
    ) {
        Ok(image) => image,
        Err(error) => return Ok(vec![format!("{overlay}\tUNVERIFIED\t{error}")]),
    };
    let built = match assemble_overlay(&OverlaySource::path(&path), OVERLAY_BASE) {
        Ok(built) => built,
        Err(error) => {
            return Ok(vec![format!(
                "{overlay}\tCOMPILE_FAILED\t{error}\tunverified_owners={}",
                placeholders.len()
            )])
        }
    };
    if built.len() != image.len() {
        return Ok(vec![format!(
            "{overlay}\tDIFFERS\treference={}\tassembled={}",
            image.len(),
            built.len()
        )]);
    }
    for (address, placeholder) in placeholders {
        let span = placeholder.span;
        let start = (address - OVERLAY_BASE) as usize;
        let Some(reference) = image.get(start..start + span as usize) else {
            findings.push(format!(
                "{overlay}:{address:08x}\tUNVERIFIED\tcanonical overlay is {} bytes and the row needs {}",
                image.len(), start + span as usize
            ));
            continue;
        };
        let assembled = &built[start..start + span as usize];
        if assembled != reference {
            let differing = psynergy::compare::differing_offsets(assembled, reference, 2).len();
            findings.push(format!("{overlay}:{address:08x}\tDIFFERS\treference={}\tassembled={}\tdiffering={differing}", reference.len(), assembled.len()));
        }
    }
    if findings.is_empty() && built != image {
        findings.push(format!(
            "{overlay}\tDIFFERS\tassembled object differs outside activated C spans"
        ));
    }
    Ok(findings)
}
pub fn run_audit(root: &Path, argv: &[String]) -> Result<i32, String> {
    if argv == ["--help"] || argv == ["-h"] {
        println!("usage: alchemy overlay audit [RESOURCE ... | --all | --corpus]");
        return Ok(0);
    }
    if argv == ["--corpus"] {
        return crate::overlay::score::audit_corpus(root);
    }
    let overlays: Vec<String> = if argv.is_empty() || argv[0] == "--all" {
        let mut names = Vec::new();
        let directory = crate::targets::target_for(crate::targets::DEFAULT_TARGET).overlay_dir();
        for entry in fs::read_dir(root.join(directory)).map_err(|e| e.to_string())? {
            let name = entry
                .map_err(|e| e.to_string())?
                .file_name()
                .to_string_lossy()
                .to_string();
            if let Some(stem) = name.strip_suffix("_overlay.s") {
                names.push(stem.to_string());
            }
        }
        names.sort();
        names
    } else {
        argv.to_vec()
    };
    let mut findings = 0;
    let rom = CanonicalRom::load(root)?;
    let total = overlays.len();
    let workers = std::thread::available_parallelism()
        .map_or(1, |count| count.get())
        .min(8)
        .min(total.max(1));
    let next = AtomicUsize::new(0);
    let done = AtomicUsize::new(0);
    let results = Mutex::new(Vec::with_capacity(overlays.len()));
    std::thread::scope(|scope| {
        for _ in 0..workers {
            let results = &results;
            let next = &next;
            let done = &done;
            let overlays = &overlays;
            let rom = &rom;
            scope.spawn(move || loop {
                let index = next.fetch_add(1, Ordering::Relaxed);
                if index >= overlays.len() {
                    break;
                }
                let result = audit_with_rom(root, &overlays[index], Some(rom));
                let count = done.fetch_add(1, Ordering::Relaxed) + 1;
                eprintln!("overlay audit {count}/{total}: {}", overlays[index]);
                results.lock().unwrap().push((index, result));
            });
        }
    });
    let mut results = results.into_inner().unwrap();
    results.sort_by_key(|result| result.0);
    for (index, result) in results {
        for line in result.unwrap_or_else(|error| {
            vec![format!("{}\tVERIFICATION_FAILED\t{error}", overlays[index])]
        }) {
            println!("{line}");
            findings += 1;
        }
    }
    eprintln!("audited rows with findings: {findings}");
    Ok(i32::from(findings != 0))
}
fn image_window(image: Vec<u8>, start: usize, span: usize, label: &str) -> Result<Vec<u8>, String> {
    let end = start + span;
    image.get(start..end).map(<[u8]>::to_vec).ok_or_else(|| {
        format!(
            "the {label} is {} bytes and the row needs {end}",
            image.len()
        )
    })
}
pub(crate) fn truth_window(
    root: &Path,
    target: SourceOwner,
    span: i64,
) -> Result<(Vec<u8>, &'static str), String> {
    let overlay = target.overlay_id().expect("overlay owner");
    let start = overlay_offset(target);
    let rom = match canonical_overlay(root, &overlay)
        .and_then(|image| image_window(image, start, span as usize, "decoded container"))
    {
        Ok(window) => return Ok((window, "rom")),
        Err(error) => error,
    };
    let assembled = match placeholder_span(root, target) {
        Ok(None) => {
            let path = overlay_assembly(root, &overlay);
            assemble_overlay(&OverlaySource::path(&path), OVERLAY_BASE)
                .and_then(|image| image_window(image, start, span as usize, "assembled overlay"))
        }
        Ok(Some(_)) => Err("the row is adopted, so its assembly is a placeholder".into()),
        Err(error) => Err(error),
    };
    let assembled = match assembled {
        Ok(window) => return Ok((window, "assembly")),
        Err(error) => error,
    };
    reference_bytes(root, target, span)
        .map(|bytes| (bytes, "git"))
        .map_err(|git| {
            format!("{git}; no ROM window either: {rom}; and not from the assembly: {assembled}")
        })
}
pub(crate) fn placeholder_span(root: &Path, target: SourceOwner) -> Result<Option<i64>, String> {
    let overlay = target.overlay_id().expect("overlay owner");
    let path = overlay_assembly(root, &overlay);
    let assembly =
        std::fs::read_to_string(&path).map_err(|error| format!("{}: {error}", path.display()))?;
    let lines: Vec<&str> = assembly.split('\n').collect();
    Ok(placeholder_block(&lines, i64::from(target.address())).map(|row| row.span))
}
pub struct Parked {
    pub overlay: String,
    pub address: i64,
    pub span: i64,
    pub lines: usize,
    /// Every owner restored: the owner, or each member of its instance.
    pub owners: Vec<SourceOwner>,
}
/// Marks a parked owner retained in every unit that owns it. A unit whose
/// only owner this is points its source at the parked draft with the owner
/// state flipped and its absolute symbols kept; the id is not touched, since
/// nothing keys off it and many units never carried a prefix. A unit with
/// other owners keeps its source and flips only this owner.s state.
fn retire_owner_in_units(
    units_path: &Path,
    overlay: &str,
    address: u32,
    parked_relative: &str,
) -> Result<(), String> {
    let text =
        fs::read_to_string(units_path).map_err(|e| format!("{}: {e}", units_path.display()))?;
    let mut manifest: serde_json::Value =
        serde_json::from_str(&text).map_err(|e| format!("{}: {e}", units_path.display()))?;
    let wanted = format!("0x{address:08x}");
    let mut changed = false;
    for unit in manifest["units"].as_array_mut().into_iter().flatten() {
        if unit["overlay"].as_str() != Some(overlay) {
            continue;
        }
        let Some(owners) = unit["owners"].as_array_mut() else {
            continue;
        };
        let count = owners.len();
        let mut hit = false;
        for owner in owners.iter_mut() {
            if owner["address"].as_str() == Some(wanted.as_str()) {
                owner["state"] = "not-yet-c".into();
                hit = true;
            }
        }
        if !hit {
            continue;
        }
        changed = true;
        if count == 1 {
            unit["source"] = parked_relative.into();
        }
    }
    if !changed {
        return Ok(());
    }
    let rendered = serde_json::to_string_pretty(&manifest).map_err(|e| e.to_string())?;
    fs::write(units_path, format!("{rendered}\n"))
        .map_err(|e| format!("{}: {e}", units_path.display()))
}

/// The listing with one owner's placeholder replaced by the assembly it
/// retired between the owner's listing label and its `.size` end, which
/// bound it as not-yet-C to exactly the placeholder's extent:
/// the text, the owner's span, the restored line count and the reference its
/// bytes must reproduce, with that reference's origin.
fn splice_restored(
    root: &Path,
    target: SourceOwner,
    label: &str,
    text: &str,
) -> Result<(String, i64, usize, Vec<u8>, &'static str), String> {
    let overlay = target.overlay_id().expect("overlay owner");
    let address = i64::from(target.address());
    let lines: Vec<&str> = text.split('\n').collect();
    let placeholder = placeholder_block(&lines, address).ok_or_else(|| {
        format!(
            "no AlchemyC_{address:08x} placeholder in {}",
            overlay_assembly(root, &overlay).display()
        )
    })?;
    let (start, end, span) = (placeholder.start, placeholder.end, placeholder.span);
    let (reference, oracle) = truth_window(root, target, span)?;
    let from_git = pre_adoption_text(root, target)
        .and_then(|text| region_text(&define_dangling_labels(&text), address, span));
    let restored = match from_git {
        Ok(lines)
            if !lines
                .iter()
                .any(|line| line.trim_start().starts_with("AlchemyC_")) =>
        {
            lines
        }
        _ => {
            let image = canonical_overlay(root, &overlay)?;
            let text = crate::overlay::assembly::build_overlay_source(&image, OVERLAY_BASE)?;
            region_text(&define_dangling_labels(&text), address, span)?
        }
    };
    let mut replaced: Vec<String> = Vec::with_capacity(lines.len() + restored.len());
    replaced.extend(lines[..start].iter().map(|line| line.to_string()));
    replaced.push(format!("{label}:"));
    // Assembly recovered from history may carry an earlier owner label.
    replaced.extend(restored.iter().cloned().filter(|line| {
        !crate::overlay::listing::is_owner_label(line) && !line.trim().starts_with(".size ")
    }));
    replaced.push(format!("\t.size {label}, .-{label}"));
    replaced.extend(lines[end..].iter().map(|line| line.to_string()));
    Ok((replaced.join("\n"), span, restored.len(), reference, oracle))
}
/// The owners one park restores: the owner itself, or every member of the
/// instance it belongs to, since an instance links its unit as a whole.
fn parked_members(
    root: &Path,
    target: SourceOwner,
) -> Result<Vec<(SourceOwner, Option<String>)>, String> {
    let image = target.image();
    let units = TranslationUnits::declared(root)?;
    let Some(unit) = units
        .units
        .iter()
        .find(|unit| unit.game == "tbs" && unit.declares(&image, target.address()))
    else {
        return Ok(vec![(target, None)]);
    };
    if let Some(instance) = unit.instance(&image) {
        let mut addresses = instance
            .owners
            .values()
            .map(|owner| owner.address)
            .collect::<Vec<_>>();
        addresses.sort_unstable();
        return addresses
            .into_iter()
            .map(|address| {
                let owner = SourceOwner::parse(&format!("{image}:{address:08x}"))?;
                Ok((owner, Some(unit.id.clone())))
            })
            .collect();
    }
    if !unit.instances.is_empty() {
        let instances = unit.instances.keys().cloned().collect::<Vec<_>>();
        return Err(format!(
            "{} is a canonical owner of unit {}, which is also linked into {}; park those instances first",
            target.id(),
            unit.id,
            instances.join(", ")
        ));
    }
    Ok(vec![(target, None)])
}
pub(crate) fn park_one(root: &Path, target: SourceOwner, apply: bool) -> Result<Parked, String> {
    let overlay = target.overlay_id().expect("overlay owner");
    let address = i64::from(target.address());
    let assembly = overlay_assembly(root, &overlay);
    let original = fs::read_to_string(&assembly).map_err(|error| error.to_string())?;
    let members = parked_members(root, target)?;
    let names = SourcePaths::load(root)?;
    let mut text = original.clone();
    let mut restored = Vec::new();
    for (owner, _) in &members {
        let label = names
            .registered_name(*owner)
            .map_or_else(|| owner.legacy_name(), str::to_string);
        let (updated, span, lines, reference, oracle) =
            splice_restored(root, *owner, &label, &text)?;
        text = updated;
        restored.push((*owner, span, lines, reference, oracle));
    }
    let (defined, referenced) = label_use(&text);
    for label in referenced.difference(&defined) {
        match restore_label(&text, label) {
            Some(updated) => text = updated,
            None => {
                return Err(format!(
                    "restored assembly references .L_{label}, which no placeholder covers"
                ))
            }
        }
    }
    let image = crate::overlay::compile::assemble_overlay_raw(
        &OverlaySource::named(overlay.clone(), text.clone()),
        OVERLAY_BASE,
    )?;
    for (owner, span, _, reference, oracle) in &restored {
        let at = overlay_offset(*owner);
        let window = image
            .get(at..at + *span as usize)
            .ok_or_else(|| "the parked region runs past the image".to_string())?;
        if window != reference.as_slice() {
            return Err(format!(
                "parked bytes differ from the {oracle} reference at 0x{:08x}",
                owner.address()
            ));
        }
    }
    let span = restored
        .iter()
        .find(|row| row.0 == target)
        .map_or(0, |row| row.1);
    let lines = restored.iter().map(|row| row.2).sum();
    if apply {
        match members.first().and_then(|member| member.1.as_deref()) {
            Some(unit) => {
                let spans = restored
                    .iter()
                    .map(|row| (row.0, row.1))
                    .collect::<Vec<_>>();
                retire_instance(root, unit, &overlay, &spans, &assembly, &text)?;
            }
            None => retire_owner(root, target, &assembly, &original, &text)?,
        }
    }
    Ok(Parked {
        overlay: overlay.to_string(),
        address,
        span,
        lines,
        owners: restored.iter().map(|row| row.0).collect(),
    })
}
/// Retires one instance: its restored, labelled assembly, its owners' source
/// paths (their names stay registered) and its manifest entry. The shared
/// source, the canonical unit and every other instance are untouched.
fn retire_instance(
    root: &Path,
    unit: &str,
    overlay: &str,
    spans: &[(SourceOwner, i64)],
    assembly: &Path,
    text: &str,
) -> Result<(), String> {
    let units = root.join("recon/tbs/translation-units.json");
    let register = root.join("recon/tbs/source-paths.json");
    let snapshot = crate::compiler::build_io::Snapshot::take(&[
        units.clone(),
        assembly.to_path_buf(),
        register,
    ])?;
    let outcome = (|| {
        fs::write(assembly, text).map_err(|error| format!("{}: {error}", assembly.display()))?;
        let source_paths = SourcePaths::load(root)?;
        for (owner, _) in spans {
            source_paths.unregister_owner(*owner)?;
        }
        remove_instance_in_units(&units, unit, overlay)?;
        Ok::<(), String>(())
    })();
    outcome.map_err(|error| {
        snapshot.restore();
        format!("{error}; nothing parked, the assembly and registers are restored")
    })
}
/// Removes one image's instance from a unit, keeping its source, canonical
/// owners and other instances.
fn remove_instance_in_units(units_path: &Path, unit: &str, image: &str) -> Result<(), String> {
    let text =
        fs::read_to_string(units_path).map_err(|e| format!("{}: {e}", units_path.display()))?;
    let mut manifest: serde_json::Value =
        serde_json::from_str(&text).map_err(|e| format!("{}: {e}", units_path.display()))?;
    let entry = manifest["units"]
        .as_array_mut()
        .into_iter()
        .flatten()
        .find(|entry| entry["id"].as_str() == Some(unit))
        .and_then(serde_json::Value::as_object_mut)
        .ok_or_else(|| format!("{}: no unit {unit}", units_path.display()))?;
    let instances = entry
        .get_mut("instances")
        .and_then(serde_json::Value::as_object_mut)
        .ok_or_else(|| format!("{unit} declares no instances"))?;
    if instances.shift_remove(image).is_none() {
        return Err(format!("{unit} has no instance in {image}"));
    }
    if instances.is_empty() {
        entry.shift_remove("instances");
    }
    let rendered = serde_json::to_string_pretty(&manifest).map_err(|e| e.to_string())?;
    fs::write(units_path, format!("{rendered}\n"))
        .map_err(|e| format!("{}: {e}", units_path.display()))
}
/// Retires one standalone or canonical unit owner, as parking always has.
fn retire_owner(
    root: &Path,
    target: SourceOwner,
    assembly: &Path,
    original: &str,
    text: &str,
) -> Result<(), String> {
    let overlay = target.overlay_id().expect("overlay owner");
    let owner = target;
    let source_paths = SourcePaths::load(root)?;
    let installed = source_paths.source_path(owner);
    let shared = source_paths
        .owners_for_path(&installed)
        .into_iter()
        .any(|registered| registered != owner);
    let parked = retained_source(root, target);
    fs::write(assembly, text).map_err(|error| error.to_string())?;
    let parked_before = if shared && parked.exists() {
        Some(fs::read(&parked).map_err(|error| {
            let _ = fs::write(assembly, original);
            format!(
                "cannot preserve {} before parking: {error}",
                parked.display()
            )
        })?)
    } else {
        None
    };
    let copied = if installed.exists() && shared {
        fs::copy(&installed, &parked).map_err(|error| {
            let _ = fs::write(assembly, original);
            format!(
                "cannot copy {} to the EN reconstruction corpus: {error}",
                installed.display()
            )
        })?;
        true
    } else {
        false
    };
    let moved = if installed.exists() && !shared {
        fs::rename(&installed, &parked).map_err(|error| {
            let _ = fs::write(assembly, original);
            format!(
                "cannot move {} to the EN reconstruction corpus: {error}",
                installed.display()
            )
        })?;
        true
    } else {
        false
    };
    if let Err(error) = source_paths.unregister_owner(owner) {
        if moved {
            let _ = fs::rename(&parked, &installed);
        }
        if copied {
            match parked_before {
                Some(bytes) => {
                    let _ = fs::write(&parked, bytes);
                }
                None => {
                    let _ = fs::remove_file(&parked);
                }
            }
        }
        let _ = fs::write(assembly, original);
        return Err(error);
    }
    // The unit register mirrors the adoption edit in place rather than
    // losing the unit: a unit pointing at a moved file is unscoreable,
    // and rebuilding it later drops its absolute symbols.
    let units = root.join("recon/tbs/translation-units.json");
    let snapshot = crate::compiler::build_io::Snapshot::take(&[
        units.clone(),
        assembly.to_path_buf(),
        installed.clone(),
        parked.clone(),
        root.join("recon/tbs/source-paths.json"),
    ])?;
    let parked_relative = parked
        .strip_prefix(root)
        .map(|p| p.to_string_lossy().into_owned())
        .unwrap_or_else(|_| parked.to_string_lossy().into_owned());
    let outcome = retire_owner_in_units(&units, &overlay, target.address(), &parked_relative);
    if let Err(error) = outcome {
        snapshot.restore();
        return Err(format!(
            "{error}; nothing parked, the assembly, source and registers are restored"
        ));
    }
    Ok(())
}
pub fn run(root: &Path, argv: &[String]) -> Result<i32, String> {
    let mut apply = false;
    let mut rows: Vec<String> = Vec::new();
    for argument in argv {
        match argument.as_str() {
            "--apply" => apply = true,
            "-h" | "--help" => {
                println!("usage: alchemy overlay park <overlay>:<addressHex> [...] [--apply]");
                return Ok(0);
            }
            other => rows.push(other.to_string()),
        }
    }
    if rows.is_empty() {
        return Err("at least one <overlay>:<addressHex> row is required".to_string());
    }
    let mut failures = 0;
    for row in rows {
        let target = crate::overlay::score::resolve(root, &row)?;
        match park_one(root, target, apply) {
            Ok(parked) => println!(
                "parked {}:{:08x} span={} lines={}{}{}",
                parked.overlay,
                parked.address,
                parked.span,
                parked.lines,
                match parked.owners.as_slice() {
                    [_] => String::new(),
                    owners => format!(
                        " instance={}",
                        owners
                            .iter()
                            .map(|owner| owner.id())
                            .collect::<Vec<_>>()
                            .join(",")
                    ),
                },
                if apply { "" } else { " (dry run)" }
            ),
            Err(error) => {
                eprintln!("{row}: {error}");
                failures += 1;
            }
        }
    }
    Ok(if failures == 0 { 0 } else { 1 })
}
#[cfg(test)]
mod tests {
    #[test]
    fn parking_an_instance_keeps_the_shared_source_and_other_instances() {
        use crate::compiler::source_paths::SourceOwner;
        use crate::compiler::translation_units::fixture::{Repository, STAGED_ACTOR};
        let repository = Repository::new();
        let root = repository.0.path();
        let game = root.join("games/THE BROKEN SEAL");
        let recon = root.join("recon/tbs");
        let owner = |id: &str| SourceOwner::parse(id).unwrap();
        // Parking one member of an instance parks every member it links there.
        let members = super::parked_members(root, owner("resource_39b:02000ba4")).unwrap();
        let unit = Some("staged-actor".to_string());
        assert_eq!(
            members,
            [
                (owner("resource_39b:02000630"), unit.clone()),
                (owner("resource_39b:02000ba4"), unit),
            ]
        );
        let error = super::parked_members(root, owner("resource_3bf:020008c0")).unwrap_err();
        assert!(
            error.contains("canonical owner of unit staged-actor, which is also linked into resource_389, resource_39b; park those instances first"),
            "{error}"
        );
        let standalone = owner("resource_3a0:02000100");
        assert_eq!(
            super::parked_members(root, standalone).unwrap(),
            [(standalone, None)]
        );

        let listing = recon.join("raw/overlays/resource_39b_overlay.s");
        let spans = [
            (owner("resource_39b:02000630"), 296),
            (owner("resource_39b:02000ba4"), 284),
        ];
        let files = [
            "raw/overlays/resource_39b_overlay.s",
            "source-paths.json",
            "translation-units.json",
        ];
        let contents = || files.map(|file| fs::read_to_string(recon.join(file)).unwrap());
        // A failure restores every register it touched.
        let before = contents();
        let error = super::retire_instance(
            root,
            "staged-actor",
            "resource_39c",
            &spans,
            &listing,
            "restored\n",
        )
        .unwrap_err();
        assert!(
            error.contains("staged-actor has no instance in resource_39c"),
            "{error}"
        );
        assert!(error.contains("nothing parked"), "{error}");
        assert_eq!(contents(), before);

        super::retire_instance(
            root,
            "staged-actor",
            "resource_39b",
            &spans,
            &listing,
            "restored\n",
        )
        .unwrap();
        assert_eq!(fs::read_to_string(&listing).unwrap(), "restored\n");
        assert!(game.join("SRC").join(STAGED_ACTOR).is_file());
        let register: serde_json::Value =
            serde_json::from_str(&fs::read_to_string(recon.join("source-paths.json")).unwrap())
                .unwrap();
        for id in ["resource_39b:02000630", "resource_39b:02000ba4"] {
            assert!(register["owners"][id].get("source").is_none(), "{id}");
            assert!(register["owners"][id]["name"].is_string(), "{id}");
        }
        for id in ["resource_3bf:0200034c", "resource_389:020008c0"] {
            assert_eq!(register["owners"][id]["source"], STAGED_ACTOR, "{id}");
        }
        // The canonical unit and its other instance still load as one unit.
        let units = repository.load().unwrap();
        let unit = &units.units[0];
        assert_eq!(
            unit.images().collect::<Vec<_>>(),
            ["resource_3bf", "resource_389"]
        );
        assert_eq!(unit.owners.len(), 2);
        // Parking the last instance leaves no empty instances map behind.
        let listing = recon.join("raw/overlays/resource_389_overlay.s");
        let spans = [
            (owner("resource_389:0200034c"), 296),
            (owner("resource_389:020008c0"), 284),
        ];
        super::retire_instance(
            root,
            "staged-actor",
            "resource_389",
            &spans,
            &listing,
            "restored\n",
        )
        .unwrap();
        let manifest = fs::read_to_string(recon.join("translation-units.json")).unwrap();
        assert!(!manifest.contains("\"instances\""), "{manifest}");
        assert!(repository.load().unwrap().units[0].instances.is_empty());
    }
    #[test]
    fn parking_mirrors_the_adoption_edit_on_the_unit_register() {
        let root = tempdir().unwrap();
        let units = root.path().join("translation-units.json");
        fs::write(&units, r#"{"units":[
{"id":"overlay-37a-actor","overlay":"resource_37a","source":"games/THE BROKEN SEAL/SRC/a.c","absolute_symbols":{"Func_02004698_a":{"address":"0x0200aa54","kind":"thumb"}},"owners":[{"address":"0x02001be8","extent":192,"state":"exact-c"}]},
{"id":"shared-37a","overlay":"resource_37a","source":"games/THE BROKEN SEAL/SRC/b.c","absolute_symbols":{},"owners":[{"address":"0x02001be8","extent":192,"state":"exact-c"},{"address":"0x02002000","extent":8,"state":"exact-c"}]},
{"id":"other-37b","overlay":"resource_37b","source":"games/THE BROKEN SEAL/SRC/c.c","absolute_symbols":{},"owners":[{"address":"0x02001be8","extent":4,"state":"exact-c"}]}
]}"#).unwrap();
        super::retire_owner_in_units(
            &units,
            "resource_37a",
            0x02001be8,
            "recon/tbs/en/overlays/x.c",
        )
        .unwrap();
        let after: serde_json::Value =
            serde_json::from_str(&fs::read_to_string(&units).unwrap()).unwrap();
        let unit = &after["units"][0];
        assert_eq!(unit["id"], "overlay-37a-actor");
        assert_eq!(unit["source"], "recon/tbs/en/overlays/x.c");
        assert_eq!(unit["owners"][0]["state"], "not-yet-c");
        assert_eq!(
            unit["absolute_symbols"]["Func_02004698_a"]["address"],
            "0x0200aa54"
        );
        let shared = &after["units"][1];
        assert_eq!(shared["id"], "shared-37a");
        assert_eq!(shared["source"], "games/THE BROKEN SEAL/SRC/b.c");
        assert_eq!(shared["owners"][0]["state"], "not-yet-c");
        assert_eq!(shared["owners"][1]["state"], "exact-c");
        assert_eq!(after["units"][2]["owners"][0]["state"], "exact-c");
    }
    use super::audit_with_rom;
    use crate::overlay::adopt::audited_span;
    use crate::targets::{target_for, DEFAULT_TARGET};
    use std::fs;
    use tempfile::tempdir;
    #[test]
    fn audit_reports_a_placeholder_without_exact_source() {
        let root = tempdir().unwrap();
        let code = root.path().join("recon/tbs/raw/overlays");
        fs::create_dir_all(&code).unwrap();
        fs::write(
            code.join("resource_382_overlay.s"),
            "AlchemyC_0200dead:\n  .space 4\n",
        )
        .unwrap();
        let findings = audit_with_rom(root.path(), "resource_382", None).unwrap();
        assert_eq!(findings.len(), 1);
        assert!(findings[0].contains("resource_382:0200dead\tMISSING_SOURCE\t"));
    }
    #[test]
    fn literal_pool_address_is_not_adoptable() {
        let root = tempdir().unwrap();
        let listings = root.path().join("recon/tbs/raw/overlays");
        fs::create_dir_all(&listings).unwrap();
        fs::write(
            listings.join("resource_371_overlay.s"),
            "\t.4byte 0x0\n\t.4byte 0x0\n\t.4byte 0x0\n\t.4byte 0x0\nScene_Run:\n\t.space 0x10\n",
        )
        .unwrap();
        let accepts = |entry: u32, span| {
            let owner = crate::compiler::source_paths::SourceOwner::parse(&format!(
                "resource_371:{entry:08x}"
            ))
            .unwrap();
            audited_span(root.path(), target_for(DEFAULT_TARGET), owner, span).is_ok()
        };
        assert!(accepts(0x02000010, 16));
        for (entry, span) in [
            (0x02000000, 4),
            (0x02000012, 14),
            (0x02000010, 4),
            (0x02000010, 18),
        ] {
            assert!(!accepts(entry, span));
        }
    }
}
