use crate::{listing_offsets, region_lines};
use compiler_core::source_paths::{SourceOwner, SourcePaths};
use overlay_disasm::compile::assemble_overlay;
use overlay_disasm::{OverlaySource, OVERLAY_BASE};
use std::fs;
use std::path::Path;
use std::process::Command;
use tempfile::tempdir;

fn number(row: &serde_json::Value, key: &str) -> Option<i64> {
    row.get(key).and_then(|value| {
        value
            .as_i64()
            .or_else(|| value.as_str().and_then(|text| i64::from_str_radix(text.trim_start_matches("0x"), 16).ok()))
    })
}

#[derive(Clone, Debug, PartialEq, Eq)]
struct ThumbTransfer {
    load: bool,
    base: u8,
    registers: Vec<u8>,
    targeted: bool,
}

fn low_register(text: &str) -> Option<u8> {
    let register = text.trim().strip_prefix('r')?.parse::<u8>().ok()?;
    (register <= 7).then_some(register)
}

fn thumb_transfer(line: &str) -> Option<ThumbTransfer> {
    let code = line.split('@').next().unwrap_or("").trim();
    if code.is_empty() {
        return None;
    }
    let (targeted, instruction) = match code.split_once(':') {
        Some((_, instruction)) => (true, instruction.trim()),
        None => (false, code),
    };
    let mut words = instruction.split_whitespace();
    let mnemonic = words.next()?;
    let load = match mnemonic {
        "ldmia" => true,
        "stmia" => false,
        _ => return None,
    };
    let body = instruction.strip_prefix(mnemonic)?.trim_start();
    let (operands, rest) = body.split_once('{')?;
    let (registers, _) = rest.split_once('}')?;
    let base = low_register(operands.split(',').next()?.trim().trim_end_matches('!'))?;
    let mut parsed = Vec::new();
    for item in registers.split(',') {
        let item = item.trim();
        if let Some((first, last)) = item.split_once('-') {
            let first = low_register(first)?;
            let last = low_register(last)?;
            if first > last {
                return None;
            }
            parsed.extend(first..=last);
        } else {
            parsed.push(low_register(item)?);
        }
    }
    parsed.sort_unstable();
    parsed.dedup();
    Some(ThumbTransfer { load, base, registers: parsed, targeted })
}

fn approved_thumb_block_copy_pair(load: &ThumbTransfer, store: &ThumbTransfer) -> bool {
    load.load
        && !store.load
        && !load.targeted
        && !store.targeted
        && matches!(load.registers.len(), 2 | 3)
        && load.registers == store.registers
        && load.base != store.base
        && !load.registers.contains(&load.base)
        && !store.registers.contains(&store.base)
}

fn thumb_standalone_wide_transfer_lines(source: &str) -> Vec<usize> {
    let significant: Vec<_> = source
        .lines()
        .enumerate()
        .filter_map(|(index, line)| {
            let code = line.split('@').next().unwrap_or("").trim();
            (!code.is_empty()).then(|| (index + 1, thumb_transfer(line)))
        })
        .collect();
    significant
        .iter()
        .enumerate()
        .filter_map(|(index, (line, transfer))| {
            let transfer = transfer.as_ref()?;
            if transfer.registers.len() < 3 {
                return None;
            }
            let paired_as_load = significant
                .get(index + 1)
                .and_then(|(_, next)| next.as_ref())
                .is_some_and(|next| approved_thumb_block_copy_pair(transfer, next));
            let paired_as_store = index
                .checked_sub(1)
                .and_then(|previous| significant.get(previous))
                .and_then(|(_, previous)| previous.as_ref())
                .is_some_and(|previous| approved_thumb_block_copy_pair(previous, transfer));
            (!paired_as_load && !paired_as_store).then_some(*line)
        })
        .collect()
}

fn audit_multi_register_evidence(root: &Path, overlays: &[String]) -> Result<Vec<String>, String> {
    let regions: serde_json::Value =
        serde_json::from_slice(&fs::read(root.join("games/gs1/semantic/regions.json")).map_err(|e| e.to_string())?)
            .map_err(|e| e.to_string())?;
    let evidence: serde_json::Value = serde_json::from_slice(
        &fs::read(root.join("games/gs1/semantic/overlay-assembly.json")).map_err(|e| e.to_string())?,
    )
    .map_err(|e| e.to_string())?;
    let wanted: std::collections::BTreeSet<_> = overlays.iter().cloned().collect();
    let owners: Vec<_> = regions["manual_regions"]
        .as_array()
        .into_iter()
        .flatten()
        .filter_map(|row| {
            Some((row["overlay"].as_str()?.to_string(), number(row, "entry")?, number(row, "span_bytes")?))
        })
        .filter(|(overlay, _, _)| wanted.contains(overlay))
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
        let path = root.join("games/gs1/assets/code").join(format!("{overlay}_overlay.s"));
        let offsets: std::collections::BTreeMap<_, _> = listing_offsets(&path)?.into_iter().collect();
        let source = fs::read_to_string(&path).map_err(|e| e.to_string())?;
        for line in thumb_standalone_wide_transfer_lines(&source) {
            let Some(offset) = offsets.get(&(line as i64)) else {
                continue;
            };
            let address = OVERLAY_BASE + offset;
            if let Some((_, entry, span)) =
                owners.iter().find(|(id, entry, span)| id == overlay && address >= *entry && address < *entry + *span)
            {
                found.insert((overlay.clone(), *entry, *entry + *span));
            }
        }
    }
    let mut findings = Vec::new();
    for row in &found {
        let mut parts: Vec<_> =
            claimed.iter().filter(|part| part.0 == row.0 && part.1 >= row.1 && part.2 <= row.2).collect();
        parts.sort_by_key(|part| part.1);
        if parts.first().is_none_or(|part| part.1 != row.1) || parts.last().is_none_or(|part| part.2 != row.2) {
            findings.push(format!("{}:{:08x}\tUNCLAIMED_MULTI_REGISTER_OWNER\tend={:08x}", row.0, row.1, row.2));
        }
    }
    for row in &claimed {
        if !found.iter().any(|owner| owner.0 == row.0 && row.1 >= owner.1 && row.2 <= owner.2) {
            findings.push(format!("{}:{:08x}\tSTALE_MULTI_REGISTER_EVIDENCE\tend={:08x}", row.0, row.1, row.2));
        }
    }
    Ok(findings)
}
pub(crate) fn placeholder_block(lines: &[&str], address: i64) -> Option<(usize, usize, i64)> {
    let tag = format!("AlchemyC_{address:08x}:");
    let start = lines.iter().position(|line| line.trim() == tag)?;
    let mut end = start + 1;
    let mut span = 0i64;
    while end < lines.len() {
        let trimmed = lines[end].trim();
        if let Some(size) = trimmed.strip_prefix(".space ") {
            let size = size.trim();
            let value = match size.strip_prefix("0x") {
                Some(hexadecimal) => i64::from_str_radix(hexadecimal, 16).ok()?,
                None => size.parse::<i64>().ok()?,
            };
            span += value;
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
    Some((start, end, span))
}
fn git(root: &Path, arguments: &[&str]) -> Result<String, String> {
    let output = Command::new("git")
        .current_dir(root)
        .args(arguments)
        .output()
        .map_err(|error| format!("git failed: {error}"))?;
    if !output.status.success() {
        return Err(format!("git {} failed: {}", arguments.join(" "), String::from_utf8_lossy(&output.stderr).trim()));
    }
    String::from_utf8(output.stdout).map_err(|error| error.to_string())
}
fn pre_adoption_text(root: &Path, overlay: &str, address: i64) -> Result<String, String> {
    let relative = format!("games/gs1/assets/code/{overlay}_overlay.s");
    let tag = format!("AlchemyC_{address:08x}:");
    let log = git(root, &["log", "--follow", "--format=@%H", "--name-only", "--", &relative])?;
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
        let text = git(root, &["show", &format!("{}:{}", revision.0, revision.1)]).unwrap_or_default();
        if text.lines().any(|line| line.trim() == tag) {
            let (commit, name) =
                previous.ok_or_else(|| format!("{tag} is present in the first revision of {relative}"))?;
            return git(root, &["show", &format!("{commit}:{name}")]);
        }
        previous = Some(revision);
    }
    Err(format!("no revision of {relative} contains {tag}"))
}
fn define_dangling_labels(text: &str) -> String {
    let mut defined = std::collections::BTreeSet::new();
    let mut referenced = std::collections::BTreeSet::new();
    for line in text.split('\n') {
        let code = line.split('@').next().unwrap_or("");
        let trimmed = code.trim();
        if let Some(rest) = trimmed.strip_prefix(".L_") {
            if let Some(label) = rest.strip_suffix(':') {
                if label.len() == 8 && label.chars().all(|c| c.is_ascii_hexdigit()) {
                    defined.insert(label.to_string());
                    continue;
                }
            }
        }
        let bytes: Vec<char> = code.chars().collect();
        let mut index = 0;
        while index + 3 <= bytes.len() {
            if code[index..].starts_with(".L_") {
                let label: String = bytes[index + 3..].iter().take_while(|c| c.is_ascii_hexdigit()).collect();
                if label.len() == 8 {
                    referenced.insert(label);
                }
                index += 3;
            } else {
                index += 1;
            }
        }
    }
    let mut additions = String::new();
    for label in referenced.difference(&defined) {
        additions.push_str(&format!("\n.set .L_{label}, 0x{label}"));
    }
    if additions.is_empty() {
        text.to_string()
    } else {
        format!("{text}{additions}\n")
    }
}
fn label_use(text: &str) -> (std::collections::BTreeSet<String>, std::collections::BTreeSet<String>) {
    let mut defined = std::collections::BTreeSet::new();
    let mut referenced = std::collections::BTreeSet::new();
    for line in text.split('\n') {
        let code = line.split('@').next().unwrap_or("");
        let trimmed = code.trim();
        if let Some(rest) = trimmed.strip_prefix(".L_") {
            if let Some(label) = rest.strip_suffix(':') {
                if label.len() == 8 && label.chars().all(|c| c.is_ascii_hexdigit()) {
                    defined.insert(label.to_string());
                    continue;
                }
            }
        }
        let mut index = 0;
        while index + 3 <= code.len() {
            if code.is_char_boundary(index) && code[index..].starts_with(".L_") {
                let label: String = code[index + 3..].chars().take_while(|c| c.is_ascii_hexdigit()).collect();
                if label.len() == 8 {
                    referenced.insert(label);
                }
                index += 3;
            } else {
                index += 1;
            }
        }
    }
    (defined, referenced)
}
fn restore_label(text: &str, label: &str) -> Option<String> {
    let target = i64::from_str_radix(label, 16).ok()?;
    let lines: Vec<&str> = text.split('\n').collect();
    let mut index = 0usize;
    while index < lines.len() {
        let trimmed = lines[index].trim();
        let Some(rest) = trimmed.strip_prefix("AlchemyC_") else {
            index += 1;
            continue;
        };
        let Some(start_hex) = rest.strip_suffix(':') else {
            index += 1;
            continue;
        };
        let Ok(base) = i64::from_str_radix(start_hex, 16) else {
            index += 1;
            continue;
        };
        let mut cursor = base;
        let mut line = index + 1;
        while line < lines.len() {
            let body = lines[line].trim();
            if let Some(size) = body.strip_prefix(".space ") {
                let size = size.trim();
                let value = match size.strip_prefix("0x") {
                    Some(hexadecimal) => i64::from_str_radix(hexadecimal, 16).ok()?,
                    None => size.parse::<i64>().ok()?,
                };
                if target > cursor && target < cursor + value {
                    let mut out: Vec<String> = lines[..line].iter().map(|line| line.to_string()).collect();
                    out.push(format!("\t.space 0x{:x}", target - cursor));
                    out.push(format!(".L_{label}:"));
                    out.push(format!("\t.space 0x{:x}", cursor + value - target));
                    out.extend(lines[line + 1..].iter().map(|line| line.to_string()));
                    return Some(out.join("\n"));
                }
                cursor += value;
                line += 1;
            } else if body.starts_with(".L_") && body.ends_with(':') {
                line += 1;
            } else {
                break;
            }
        }
        index = line.max(index + 1);
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
pub fn reference_bytes(root: &Path, overlay: &str, address: i64, span: i64) -> Result<Vec<u8>, String> {
    let text = define_dangling_labels(&pre_adoption_text(root, overlay, address)?);
    let image = assemble_overlay(&OverlaySource::Str(text), OVERLAY_BASE)?;
    let start = (address - OVERLAY_BASE) as usize;
    let end = start + span as usize;
    if end > image.len() {
        return Err("the region runs past the historical image".to_string());
    }
    Ok(image[start..end].to_vec())
}
pub fn audit(root: &Path, overlay: &str) -> Result<Vec<String>, String> {
    let path = root.join(format!("games/gs1/assets/code/{overlay}_overlay.s"));
    let assembly = fs::read_to_string(&path).map_err(|error| error.to_string())?;
    let lines: Vec<&str> = assembly.split('\n').collect();
    let mut addresses = Vec::new();
    for line in &lines {
        let trimmed = line.trim();
        if let Some(rest) = trimmed.strip_prefix("AlchemyC_") {
            if let Some(hex) = rest.strip_suffix(':') {
                if let Ok(address) = i64::from_str_radix(hex, 16) {
                    addresses.push(address);
                }
            }
        }
    }
    let work = tempdir().map_err(|error| error.to_string())?;
    let source_paths = SourcePaths::load(root)?;
    let mut findings = Vec::new();
    for address in addresses {
        let Some((_, _, span)) = placeholder_block(&lines, address) else {
            continue;
        };
        let owner = SourceOwner::parse(&format!("{overlay}:{address:08x}"))?;
        let source = source_paths.source_path(owner);
        if !source.exists() {
            findings.push(format!("{overlay}:{address:08x}\tMISSING_SOURCE\t{}", source.display()));
            continue;
        }
        let reference = match truth_window(root, overlay, address, span) {
            Ok((bytes, _)) => bytes,
            Err(error) => {
                findings.push(format!("{overlay}:{address:08x}\tUNVERIFIED\t{error}"));
                continue;
            }
        };
        let compiled = match overlay_disasm::compile::compile_overlay_c(&source, work.path(), overlay, None, &[]) {
            Ok(compiled) => compiled.data,
            Err(error) => {
                findings.push(format!("{overlay}:{address:08x}\tCOMPILE_FAILED\t{error}"));
                continue;
            }
        };
        let padded = compiled.len() < reference.len()
            && reference[..compiled.len()] == compiled[..]
            && reference[compiled.len()..].iter().all(|byte| *byte == 0);
        if compiled != reference && !padded {
            let shared = compiled.len().min(reference.len());
            let mut differing = (compiled.len().abs_diff(reference.len())).div_ceil(2);
            for index in (0..shared).step_by(2) {
                if compiled[index] != reference[index] || compiled.get(index + 1) != reference.get(index + 1) {
                    differing += 1;
                }
            }
            findings.push(format!(
                "{overlay}:{address:08x}\tDIFFERS\treference={}\tcompiled={}\tdiffering={differing}",
                reference.len(),
                compiled.len()
            ));
        }
    }
    let _ = fs::remove_dir_all(&work);
    Ok(findings)
}
pub fn run_audit(root: &Path, argv: &[String]) -> Result<i32, String> {
    let overlays: Vec<String> = if argv.is_empty() || argv[0] == "--all" {
        let mut names = Vec::new();
        for entry in fs::read_dir(root.join("games/gs1/assets/code")).map_err(|e| e.to_string())? {
            let name = entry.map_err(|e| e.to_string())?.file_name().to_string_lossy().to_string();
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
    for overlay in overlays {
        for line in audit(root, &overlay)? {
            println!("{line}");
            findings += 1;
        }
    }
    eprintln!("audited rows with findings: {findings}");
    Ok(if findings == 0 { 0 } else { 1 })
}
fn rom_overlay(root: &Path, overlay: &str) -> Result<Vec<u8>, String> {
    let rom = fs::read(root.join("roms/gs1-en.gba")).map_err(|error| format!("roms/gs1-en.gba: {error}"))?;
    let table = crate::twins::resource_table(&rom)?;
    crate::twins::decode_overlay(&rom, table, overlay)
}
pub fn truth_window(root: &Path, overlay: &str, address: i64, span: i64) -> Result<(Vec<u8>, &'static str), String> {
    let start = (address - OVERLAY_BASE) as usize;
    let rom = match rom_overlay(root, overlay) {
        Ok(image) => match image.get(start..start + span as usize) {
            Some(window) => return Ok((window.to_vec(), "rom")),
            None => {
                format!("the decoded container is {} bytes and the row needs {}", image.len(), start + span as usize)
            }
        },
        Err(message) => message,
    };
    let assembled = match placeholder_span(root, overlay, address) {
        Ok(None) => {
            let path = root.join("games/gs1/assets/code").join(format!("{overlay}_overlay.s"));
            match assemble_overlay(&OverlaySource::path(&path), OVERLAY_BASE) {
                Ok(image) => match image.get(start..start + span as usize) {
                    Some(window) => return Ok((window.to_vec(), "assembly")),
                    None => format!(
                        "the assembled overlay is {} bytes and the row needs {}",
                        image.len(),
                        start + span as usize
                    ),
                },
                Err(message) => message,
            }
        }
        Ok(Some(_)) => "the row is adopted, so its assembly is a placeholder".to_string(),
        Err(message) => message,
    };
    reference_bytes(root, overlay, address, span)
        .map(|bytes| (bytes, "git"))
        .map_err(|git| format!("{git}; no ROM window either: {rom}; and not from the assembly: {assembled}"))
}
pub fn placeholder_span(root: &Path, overlay: &str, address: i64) -> Result<Option<i64>, String> {
    let path = root.join(format!("games/gs1/assets/code/{overlay}_overlay.s"));
    let assembly = std::fs::read_to_string(&path).map_err(|error| format!("{}: {error}", path.display()))?;
    let lines: Vec<&str> = assembly.split('\n').collect();
    Ok(placeholder_block(&lines, address).map(|(_, _, span)| span))
}
pub struct Parked {
    pub overlay: String,
    pub address: i64,
    pub span: i64,
    pub lines: usize,
}
pub fn park_one(root: &Path, overlay: &str, address: i64, apply: bool) -> Result<Parked, String> {
    let assembly = root.join(format!("games/gs1/assets/code/{overlay}_overlay.s"));
    let original = fs::read_to_string(&assembly).map_err(|error| error.to_string())?;
    let lines: Vec<&str> = original.split('\n').collect();
    let (start, end, span) = placeholder_block(&lines, address)
        .ok_or_else(|| format!("no AlchemyC_{address:08x} placeholder in {}", assembly.display()))?;
    let (reference, oracle) = truth_window(root, overlay, address, span)?;
    let from_git = pre_adoption_text(root, overlay, address)
        .and_then(|text| region_text(&define_dangling_labels(&text), address, span));
    let usable = |lines: &Vec<String>| !lines.iter().any(|line| line.trim_start().starts_with("AlchemyC_"));
    let restored = match from_git {
        Ok(lines) if usable(&lines) => lines,
        _ => {
            let image = rom_overlay(root, overlay)?;
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
            None => return Err(format!("restored assembly references .L_{label}, which no placeholder covers")),
        }
    }
    let image = assemble_overlay(&OverlaySource::Str(text.clone()), OVERLAY_BASE)?;
    let at = (address - OVERLAY_BASE) as usize;
    let window =
        image.get(at..at + span as usize).ok_or_else(|| "the parked region runs past the image".to_string())?;
    if window != reference.as_slice() {
        return Err(format!("parked bytes differ from the {oracle} reference at 0x{address:08x}"));
    }
    if apply {
        let owner = SourceOwner::parse(&format!("{overlay}:{address:08x}"))?;
        let source_paths = SourcePaths::load(root)?;
        let installed = source_paths.source_path(owner);
        let shared = source_paths.owners_for_path(&installed).into_iter().any(|registered| registered != owner);
        let parked = root.join(format!("games/gs1/recon/en/overlays/{overlay}_c_{address:08x}.c"));
        fs::write(&assembly, &text).map_err(|error| error.to_string())?;
        let parked_before = if shared && parked.exists() {
            Some(fs::read(&parked).map_err(|error| {
                let _ = fs::write(&assembly, &original);
                format!("cannot preserve {} before parking: {error}", parked.display())
            })?)
        } else {
            None
        };
        let copied = if installed.exists() && shared {
            fs::copy(&installed, &parked).map_err(|error| {
                let _ = fs::write(&assembly, &original);
                format!("cannot copy {} to the EN reconstruction corpus: {error}", installed.display())
            })?;
            true
        } else {
            false
        };
        let moved = if installed.exists() && !shared {
            fs::rename(&installed, &parked).map_err(|error| {
                let _ = fs::write(&assembly, &original);
                format!("cannot move {} to the EN reconstruction corpus: {error}", installed.display())
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
    Ok(Parked { overlay: overlay.to_string(), address, span, lines: restored.len() })
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
        let (overlay, address) =
            row.split_once(':').ok_or_else(|| format!("expected <overlay>:<addressHex>, got {row}"))?;
        let address = i64::from_str_radix(address.trim_start_matches("0x"), 16)
            .map_err(|_| format!("{row}: address must be hexadecimal"))?;
        let address = if address < OVERLAY_BASE { OVERLAY_BASE + address } else { address };
        match park_one(root, overlay, address, apply) {
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
    use super::{audit, rom_overlay, thumb_standalone_wide_transfer_lines};
    use std::fs;
    use tempfile::tempdir;

    #[test]
    fn recognizes_only_standalone_wide_thumb_transfers() {
        assert_eq!(thumb_standalone_wide_transfer_lines("stmia r2!, {r0-r3} @ wide store"), vec![1]);
        assert!(thumb_standalone_wide_transfer_lines("\tldmia r3!, {r0-r2}\n\tstmia r4!, {r0, r1, r2}").is_empty());
        assert_eq!(
            thumb_standalone_wide_transfer_lines("\tldmia r3!, {r0-r2}\n.L_target:\n\tstmia r4!, {r0-r2}"),
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
        fs::write(code.join("resource_382_overlay.s"), "AlchemyC_0200dead:\n  .space 4\n").unwrap();
        let findings = audit(root.path(), "resource_382").unwrap();
        assert_eq!(findings.len(), 1);
        assert!(findings[0].contains("resource_382:0200dead\tMISSING_SOURCE\t"));
    }

    #[test]
    fn rom_overlay_uses_live_directory_and_dispatches_tag_one() {
        const ROM_BASE: usize = 0x0800_0000;
        const RESOURCE: usize = 0x36f;
        let root = tempdir().unwrap();
        let table = 0x100usize;
        let start = 0x1000usize;
        let stream = [1, 0x30, b'A', b'B', 0x01, 0x02, 0, 0];
        let mut rom = vec![0u8; start + stream.len() + 16];

        rom[table..table + 4].copy_from_slice(&(ROM_BASE as u32).to_le_bytes());
        rom[table + 4..table + 8].copy_from_slice(&((ROM_BASE + table) as u32).to_le_bytes());
        let pointer = table + RESOURCE * 4;
        rom[pointer..pointer + 4].copy_from_slice(&((ROM_BASE + start) as u32).to_le_bytes());
        rom[pointer + 4..pointer + 8].copy_from_slice(&((ROM_BASE + start + stream.len()) as u32).to_le_bytes());
        rom[start..start + stream.len()].copy_from_slice(&stream);

        fs::create_dir_all(root.path().join("roms")).unwrap();
        fs::write(root.path().join("roms/gs1-en.gba"), rom).unwrap();

        assert_eq!(rom_overlay(root.path(), "resource_36f").unwrap(), b"ABAB");
    }
}
