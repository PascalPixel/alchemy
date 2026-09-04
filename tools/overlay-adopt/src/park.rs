use crate::{listing_offsets, overlay_assembly, overlay_offset, region_lines, retained_source};
use compiler_core::{
    source_paths::{SourceOwner, SourcePaths},
    thumb::standalone_wide_transfer_lines as thumb_standalone_wide_transfer_lines,
};
use overlay_disasm::compile::assemble_overlay;
use overlay_disasm::{canonical_overlay, CanonicalRom, OverlaySource, OVERLAY_BASE};
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
    let owners: Vec<_> = crate::reviewed_spans(root)?
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
#[derive(Clone, Copy)]
pub(crate) struct Placeholder {
    pub(crate) start: usize,
    pub(crate) end: usize,
    pub(crate) span: i64,
}
fn placeholder_address(line: &str) -> Option<i64> {
    i64::from_str_radix(
        line.trim().strip_prefix("AlchemyC_")?.strip_suffix(':')?,
        16,
    )
    .ok()
}
fn space_size(line: &str) -> Option<i64> {
    let size = line.trim().strip_prefix(".space ")?.trim();
    size.strip_prefix("0x")
        .map(|hex| i64::from_str_radix(hex, 16).ok())
        .unwrap_or_else(|| size.parse().ok())
}
pub(crate) fn placeholder_block(lines: &[&str], address: i64) -> Option<Placeholder> {
    let tag = format!("AlchemyC_{address:08x}:");
    let start = lines.iter().position(|line| line.trim() == tag)?;
    let mut end = start + 1;
    let mut span = 0i64;
    while end < lines.len() {
        let trimmed = lines[end].trim();
        if trimmed.starts_with(".space ") {
            span += space_size(trimmed)?;
            end += 1;
        } else if trimmed.starts_with(".L_") && trimmed.ends_with(':') {
            end += 1;
        } else {
            break;
        }
    }
    if end == start + 1 {
        return None;
    }
    Some(Placeholder { start, end, span })
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
    let relative = format!("games/gs1/assets/code/{overlay}_overlay.s");
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
pub fn audit(root: &Path, overlay: &str) -> AuditResult {
    audit_with_rom(root, overlay, None)
}
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
            return Ok(addresses
                .into_iter()
                .map(|address| format!("{overlay}:{address:08x}\tCOMPILE_FAILED\t{error}"))
                .collect())
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
            let differing = crate::differing_units(assembled, reference, 2);
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
    if argv == ["--corpus"] {
        return crate::score::audit_corpus(root);
    }
    let overlays: Vec<String> = if argv.is_empty() || argv[0] == "--all" {
        let mut names = Vec::new();
        for entry in fs::read_dir(root.join("games/gs1/assets/code")).map_err(|e| e.to_string())? {
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
    for (_, result) in results {
        for line in result? {
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
pub(crate) fn park_one(root: &Path, target: SourceOwner, apply: bool) -> Result<Parked, String> {
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
            let text = overlay_disasm::build_overlay_source(&image, OVERLAY_BASE)?;
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
    let image = overlay_disasm::compile::assemble_overlay_raw(
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
    let mut rows: Vec<String> = Vec::new();
    for argument in argv {
        match argument.as_str() {
            "--apply" => apply = true,
            "-h" | "--help" => {
                println!("usage: overlay park <overlay>:<addressHex> [...] [--apply]");
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
        let target = crate::score::resolve(root, &row)?;
        match park_one(root, target, apply) {
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
    use super::{audit, thumb_standalone_wide_transfer_lines};
    use crate::audited_span;
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
        let code = root.path().join("games/gs1/assets/code");
        fs::create_dir_all(&code).unwrap();
        fs::write(
            code.join("resource_382_overlay.s"),
            "AlchemyC_0200dead:\n  .space 4\n",
        )
        .unwrap();
        let findings = audit(root.path(), "resource_382").unwrap();
        assert_eq!(findings.len(), 1);
        assert!(findings[0].contains("resource_382:0200dead\tMISSING_SOURCE\t"));
    }
    #[test]
    fn literal_pool_address_is_not_adoptable() {
        let root = tempdir().unwrap();
        let metrics = root.path().join("games/gs1/metrics");
        fs::create_dir_all(&metrics).unwrap();
        fs::write(
            metrics.join("gs1-en-executable.json"),
            r#"{"overlays":[{"id":"resource_371","intervals":[{"start":33554432,"end":33554448,"kind":"literal_pool"},{"start":33554448,"end":33554464,"kind":"thumb"}]}]}"#,
        )
        .unwrap();
        assert!(audited_span(root.path(), "resource_371", 0x02000000, 4, "resource_371").is_err());
        assert!(audited_span(root.path(), "resource_371", 0x02000010, 4, "resource_371").is_ok());
    }
}
