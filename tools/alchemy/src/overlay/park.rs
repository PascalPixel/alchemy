pub(crate) use crate::compiler::overlay::placeholder_block;
use crate::compiler::{
    overlay::space_size,
    source_paths::{SourceOwner, SourcePaths},
    thumb::standalone_wide_transfer_lines as thumb_standalone_wide_transfer_lines,
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
fn number(row: &serde_json::Value, key: &str) -> Option<i64> {
    row.get(key).and_then(|value| {
        value.as_i64().or_else(|| {
            value
                .as_str()
                .and_then(|text| i64::from_str_radix(text.trim_start_matches("0x"), 16).ok())
        })
    })
}
fn audit_multi_register_evidence(root: &Path, overlays: &[String]) -> Result<Vec<String>, String> {
    let evidence: serde_json::Value = serde_json::from_slice(
        &fs::read(root.join("games/gs1/semantic/overlay-assembly.json"))
            .map_err(|e| e.to_string())?,
    )
    .map_err(|e| e.to_string())?;
    let wanted: std::collections::BTreeSet<_> = overlays.iter().cloned().collect();
    let owners: Vec<_> = crate::overlay::reviewed_spans(root)?
        .into_iter()
        .filter_map(|(owner, span)| {
            let overlay = owner.overlay_id()?;
            wanted
                .contains(&overlay)
                .then_some((overlay, i64::from(owner.address()), span as i64))
        })
        .collect();
    let claimed: std::collections::BTreeSet<_> = evidence["regions"]
        .as_array()
        .into_iter()
        .flatten()
        .filter(|row| row["kind"].as_str() == Some("thumb_multi_register_module"))
        .filter_map(|row| {
            let overlay = row["overlay"].as_str()?.to_string();
            let start = number(row, "start")?;
            let end = number(row, "end")?;
            wanted.contains(&overlay).then_some((overlay, start, end))
        })
        .collect();
    let mut found = std::collections::BTreeSet::new();
    for overlay in &wanted {
        let path = overlay_assembly(root, overlay);
        let offsets: std::collections::BTreeMap<_, _> =
            listing_offsets(&path)?.into_iter().collect();
        let source = fs::read_to_string(&path).map_err(|e| e.to_string())?;
        for line in thumb_standalone_wide_transfer_lines(&source) {
            let Some(offset) = offsets.get(&(line as i64)) else {
                continue;
            };
            let address = OVERLAY_BASE + offset;
            if let Some((_, entry, span)) = owners.iter().find(|(id, entry, span)| {
                id == overlay && address >= *entry && address < *entry + *span
            }) {
                found.insert((overlay.clone(), *entry, *entry + *span));
            }
        }
    }
    let mut findings = Vec::new();
    for row in &found {
        let mut parts: Vec<_> = claimed
            .iter()
            .filter(|part| part.0 == row.0 && part.1 >= row.1 && part.2 <= row.2)
            .collect();
        parts.sort_by_key(|part| part.1);
        if parts.first().is_none_or(|part| part.1 != row.1)
            || parts.last().is_none_or(|part| part.2 != row.2)
        {
            findings.push(format!(
                "{}:{:08x}\tUNCLAIMED_MULTI_REGISTER_OWNER\tend={:08x}",
                row.0, row.1, row.2
            ));
        }
    }
    for row in &claimed {
        if !found
            .iter()
            .any(|owner| owner.0 == row.0 && row.1 >= owner.1 && row.2 <= owner.2)
        {
            findings.push(format!(
                "{}:{:08x}\tSTALE_MULTI_REGISTER_EVIDENCE\tend={:08x}",
                row.0, row.1, row.2
            ));
        }
    }
    Ok(findings)
}
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
    let relative = format!("games/gs1/asm/overlays/{overlay}_overlay.s");
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
        for entry in fs::read_dir(root.join("games/gs1/asm/overlays")).map_err(|e| e.to_string())? {
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
    for line in audit_multi_register_evidence(root, &overlays)? {
        println!("{line}");
        findings += 1;
    }
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
                owner["state"] = "retained-assembly".into();
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

/// Puts a retained-classification row back over a parked owner's span:
/// the full build takes the non-exact branch for an overlay owner and
/// requires one. The row claims no origin: it opens with the withdrawn
/// standard's disclaimer and carries the operator's reason, in the shape
/// of the rows written by hand for the same purpose. Adoption may have split
/// rather than removed the original row, so a span that overlaps any
/// existing row is refused rather than guessed at.
fn restore_retained_row(
    evidence_path: &Path,
    overlay: &str,
    start: i64,
    span: i64,
    reason: &str,
) -> Result<(), String> {
    let text = fs::read_to_string(evidence_path)
        .map_err(|e| format!("{}: {e}", evidence_path.display()))?;
    let mut document: serde_json::Value =
        serde_json::from_str(&text).map_err(|e| format!("{}: {e}", evidence_path.display()))?;
    let end = start + span;
    let regions = document["regions"]
        .as_array_mut()
        .ok_or("overlay-assembly.json: regions must be an array")?;
    for row in regions.iter() {
        if row["overlay"].as_str() != Some(overlay) {
            continue;
        }
        let (Some(row_start), Some(row_end)) = (number(row, "start"), number(row, "end")) else {
            continue;
        };
        if row_start < end && row_end > start {
            return Err(format!(
                "a retained row 0x{row_start:08x}-0x{row_end:08x} already overlaps 0x{start:08x}-0x{end:08x}; adoption split rather than removed it, restore the row by hand"
            ));
        }
    }
    regions.push(serde_json::json!({
        "overlay": overlay,
        "start": format!("0x{start:08x}"),
        "end": format!("0x{end:08x}"),
        "kind": "compiler_allocation_module",
        "retention": "keep_structured_asm",
        "confidence": "strong",
        "evidence": [
            "Credit withdrawn under the handwritten/third-party-assembly requirement: the observations below do not establish assembly origin. This remains unverified retained code eligible for ordinary C recovery.",
            format!("Parked by alchemy overlay park: {reason}"),
        ],
    }));
    let rendered = serde_json::to_string_pretty(&document).map_err(|e| e.to_string())?;
    fs::write(evidence_path, format!("{rendered}\n"))
        .map_err(|e| format!("{}: {e}", evidence_path.display()))
}

pub(crate) fn park_one(
    root: &Path,
    target: SourceOwner,
    apply: bool,
    reason: Option<&str>,
) -> Result<Parked, String> {
    let overlay = target.overlay_id().expect("overlay owner");
    let address = i64::from(target.address());
    let assembly = overlay_assembly(root, &overlay);
    let original = fs::read_to_string(&assembly).map_err(|error| error.to_string())?;
    let lines: Vec<&str> = original.split('\n').collect();
    let placeholder = placeholder_block(&lines, address).ok_or_else(|| {
        format!(
            "no AlchemyC_{address:08x} placeholder in {}",
            assembly.display()
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
    replaced.extend(restored.iter().cloned());
    replaced.extend(lines[end..].iter().map(|line| line.to_string()));
    let mut text = replaced.join("\n");
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
    let at = (address - OVERLAY_BASE) as usize;
    let window = image
        .get(at..at + span as usize)
        .ok_or_else(|| "the parked region runs past the image".to_string())?;
    if window != reference.as_slice() {
        return Err(format!(
            "parked bytes differ from the {oracle} reference at 0x{address:08x}"
        ));
    }
    if apply {
        let owner = target;
        let source_paths = SourcePaths::load(root)?;
        let installed = source_paths.source_path(owner);
        let shared = source_paths
            .owners_for_path(&installed)
            .into_iter()
            .any(|registered| registered != owner);
        let parked = retained_source(root, target);
        fs::write(&assembly, &text).map_err(|error| error.to_string())?;
        let parked_before = if shared && parked.exists() {
            Some(fs::read(&parked).map_err(|error| {
                let _ = fs::write(&assembly, &original);
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
                let _ = fs::write(&assembly, &original);
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
                let _ = fs::write(&assembly, &original);
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
            let _ = fs::write(&assembly, &original);
            return Err(error);
        }
        // The unit register mirrors the adoption edit in place rather than
        // losing the unit: a unit pointing at a moved file is unscoreable,
        // and rebuilding it later drops its absolute symbols.
        let units = root.join("games/gs1/recon/translation-units.json");
        let evidence = root.join("games/gs1/semantic/overlay-assembly.json");
        let snapshot = crate::compiler::build_io::Snapshot::take(&[
            units.clone(),
            evidence.clone(),
            assembly.clone(),
            installed.clone(),
            parked.clone(),
            root.join("games/gs1/source-paths.json"),
        ])?;
        let parked_relative = parked
            .strip_prefix(root)
            .map(|p| p.to_string_lossy().into_owned())
            .unwrap_or_else(|_| parked.to_string_lossy().into_owned());
        let reason = reason.unwrap_or("no reason given");
        let outcome = retire_owner_in_units(&units, &overlay, target.address(), &parked_relative)
            .and_then(|()| restore_retained_row(&evidence, &overlay, address, span, reason));
        if let Err(error) = outcome {
            snapshot.restore();
            return Err(format!(
                "{error}; nothing parked, the assembly, source and registers are restored"
            ));
        }
    }
    Ok(Parked {
        overlay: overlay.to_string(),
        address,
        span,
        lines: restored.len(),
    })
}
pub fn run(root: &Path, argv: &[String]) -> Result<i32, String> {
    let mut apply = false;
    let mut reason: Option<String> = None;
    let mut rows: Vec<String> = Vec::new();
    let mut arguments = argv.iter();
    while let Some(argument) = arguments.next() {
        match argument.as_str() {
            "--apply" => apply = true,
            "--reason" => reason = arguments.next().cloned(),
            "-h" | "--help" => {
                println!("usage: alchemy overlay park <overlay>:<addressHex> [...] [--apply --reason TEXT]");
                return Ok(0);
            }
            other => rows.push(other.to_string()),
        }
    }
    if apply && reason.as_deref().is_none_or(str::is_empty) {
        return Err("--apply needs --reason TEXT: why the credit is withdrawn, recorded on the retained row".to_string());
    }
    if rows.is_empty() {
        return Err("at least one <overlay>:<addressHex> row is required".to_string());
    }
    let mut failures = 0;
    for row in rows {
        let target = crate::overlay::score::resolve(root, &row)?;
        match park_one(root, target, apply, reason.as_deref()) {
            Ok(parked) => println!(
                "parked {}:{:08x} span={} lines={}{}",
                parked.overlay,
                parked.address,
                parked.span,
                parked.lines,
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
    fn parking_restores_a_neutral_retained_row_and_refuses_overlap() {
        let root = tempdir().unwrap();
        let evidence = root.path().join("overlay-assembly.json");
        fs::write(&evidence, r#"{"format":1,"regions":[{"overlay":"resource_37a","start":"0x02001000","end":"0x02001100","kind":"structured_scene_module","retention":"keep_structured_asm","confidence":"strong","evidence":["x"]}]}"#).unwrap();
        super::restore_retained_row(
            &evidence,
            "resource_37a",
            0x02001be8,
            192,
            "not reproducible",
        )
        .unwrap();
        let after: serde_json::Value =
            serde_json::from_str(&fs::read_to_string(&evidence).unwrap()).unwrap();
        let row = &after["regions"][1];
        assert_eq!(row["start"], "0x02001be8");
        assert_eq!(row["end"], "0x02001ca8");
        assert_eq!(row["retention"], "keep_structured_asm");
        let text = row["evidence"].to_string();
        assert!(text.contains("Credit withdrawn") && text.contains("not reproducible"));
        let overlap =
            super::restore_retained_row(&evidence, "resource_37a", 0x02001c00, 16, "again");
        assert!(overlap.unwrap_err().contains("already overlaps"));
        assert!(
            super::restore_retained_row(&evidence, "resource_37b", 0x02001c00, 16, "other").is_ok()
        );
    }

    #[test]
    fn parking_mirrors_the_adoption_edit_on_the_unit_register() {
        let root = tempdir().unwrap();
        let units = root.path().join("translation-units.json");
        fs::write(&units, r#"{"units":[
{"id":"overlay-37a-actor","overlay":"resource_37a","source":"games/gs1/src/a.c","absolute_symbols":{"Func_02004698_a":{"address":"0x0200aa54","kind":"thumb"}},"owners":[{"address":"0x02001be8","extent":192,"state":"exact-c"}]},
{"id":"shared-37a","overlay":"resource_37a","source":"games/gs1/src/b.c","absolute_symbols":{},"owners":[{"address":"0x02001be8","extent":192,"state":"exact-c"},{"address":"0x02002000","extent":8,"state":"exact-c"}]},
{"id":"other-37b","overlay":"resource_37b","source":"games/gs1/src/c.c","absolute_symbols":{},"owners":[{"address":"0x02001be8","extent":4,"state":"exact-c"}]}
]}"#).unwrap();
        super::retire_owner_in_units(
            &units,
            "resource_37a",
            0x02001be8,
            "games/gs1/recon/en/overlays/x.c",
        )
        .unwrap();
        let after: serde_json::Value =
            serde_json::from_str(&fs::read_to_string(&units).unwrap()).unwrap();
        let unit = &after["units"][0];
        assert_eq!(unit["id"], "overlay-37a-actor");
        assert_eq!(unit["source"], "games/gs1/recon/en/overlays/x.c");
        assert_eq!(unit["owners"][0]["state"], "retained-assembly");
        assert_eq!(
            unit["absolute_symbols"]["Func_02004698_a"]["address"],
            "0x0200aa54"
        );
        let shared = &after["units"][1];
        assert_eq!(shared["id"], "shared-37a");
        assert_eq!(shared["source"], "games/gs1/src/b.c");
        assert_eq!(shared["owners"][0]["state"], "retained-assembly");
        assert_eq!(shared["owners"][1]["state"], "exact-c");
        assert_eq!(after["units"][2]["owners"][0]["state"], "exact-c");
    }
    use super::{audit_with_rom, thumb_standalone_wide_transfer_lines};
    use crate::overlay::audited_span;
    use std::fs;
    use tempfile::tempdir;
    #[test]
    fn recognizes_only_standalone_wide_thumb_transfers() {
        assert_eq!(
            thumb_standalone_wide_transfer_lines("stmia r2!, {r0-r3} @ wide store"),
            vec![1]
        );
        assert!(thumb_standalone_wide_transfer_lines(
            "\tldmia r3!, {r0-r2}\n\tstmia r4!, {r0, r1, r2}"
        )
        .is_empty());
        assert_eq!(
            thumb_standalone_wide_transfer_lines(
                "\tldmia r3!, {r0-r2}\n.L_target:\n\tstmia r4!, {r0-r2}"
            ),
            vec![1, 3]
        );
        assert!(thumb_standalone_wide_transfer_lines("\tldmia r3!, {r0}").is_empty());
        assert!(thumb_standalone_wide_transfer_lines("@ stmia r3!, {r0-r2}").is_empty());
        assert!(thumb_standalone_wide_transfer_lines("\tpush {r4, r5, lr}").is_empty());
    }
    #[test]
    fn audit_reports_a_placeholder_without_exact_source() {
        let root = tempdir().unwrap();
        let code = root.path().join("games/gs1/asm/overlays");
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
        let semantic = root.path().join("games/gs1/semantic");
        fs::create_dir_all(&semantic).unwrap();
        fs::write(
            semantic.join("regions.json"),
            r#"{"manual_regions":[{"overlay":"resource_371","entry":"0x02000010","span_bytes":16}]}"#,
        )
        .unwrap();
        let accepts = |entry, span| audited_span(root.path(), "resource_371", entry, span).is_ok();
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
