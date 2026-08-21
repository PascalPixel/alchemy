use crate::{listing_offsets, region_lines};
use overlay_disasm::compile::assemble_overlay;
use overlay_disasm::{OverlaySource, OVERLAY_BASE};
use std::fs;
use std::path::Path;
use std::process::Command;
use tempfile::tempdir;
fn placeholder_block(lines: &[&str], address: i64) -> Option<(usize, usize, i64)> {
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
        return Err(format!(
            "git {} failed: {}",
            arguments.join(" "),
            String::from_utf8_lossy(&output.stderr).trim()
        ));
    }
    String::from_utf8(output.stdout).map_err(|error| error.to_string())
}
fn pre_adoption_text(root: &Path, overlay: &str, address: i64) -> Result<String, String> {
    let relative = format!("assets/code/{overlay}_overlay.s");
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
                let label: String = bytes[index + 3..]
                    .iter()
                    .take_while(|c| c.is_ascii_hexdigit())
                    .collect();
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
                let label: String = code[index + 3..]
                    .chars()
                    .take_while(|c| c.is_ascii_hexdigit())
                    .collect();
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
                    let mut out: Vec<String> =
                        lines[..line].iter().map(|line| line.to_string()).collect();
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
pub fn reference_bytes(
    root: &Path,
    overlay: &str,
    address: i64,
    span: i64,
) -> Result<Vec<u8>, String> {
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
    let path = root.join(format!("assets/code/{overlay}_overlay.s"));
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
    let mut findings = Vec::new();
    for address in addresses {
        let Some((_, _, span)) = placeholder_block(&lines, address) else {
            continue;
        };
        let source = root.join(format!("exact/{overlay}_c_{address:08x}.c"));
        if !source.exists() {
            continue;
        }
        let reference = match truth_window(root, overlay, address, span) {
            Ok((bytes, _)) => bytes,
            Err(error) => {
                findings.push(format!("{overlay}:{address:08x}\tUNVERIFIED\t{error}"));
                continue;
            }
        };
        let compiled = match overlay_disasm::compile::compile_overlay_c(
            &source,
            work.path(),
            overlay,
            None,
            &[],
        ) {
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
                if compiled[index] != reference[index]
                    || compiled.get(index + 1) != reference.get(index + 1)
                {
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
        for entry in fs::read_dir(root.join("assets/code")).map_err(|e| e.to_string())? {
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
    let manifest = fs::read_to_string(root.join("assets/manifest.json"))
        .map_err(|error| format!("assets/manifest.json: {error}"))?;
    let want = format!("\"{}\"", overlay.trim_start_matches("resource_"));
    let mut address = 0usize;
    let mut compressed = 0usize;
    let mut decoded = 0usize;
    for row in manifest.split('[') {
        if !row.starts_with(&want) {
            continue;
        }
        let numbers: Vec<usize> = row
            .split('"')
            .filter_map(|piece| {
                piece
                    .strip_prefix("0x")
                    .and_then(|hex| usize::from_str_radix(hex, 16).ok())
            })
            .collect();
        if numbers.len() >= 3 {
            address = numbers[0];
            compressed = numbers[1];
            decoded = numbers[2];
            break;
        }
    }
    if address == 0 {
        return Err(format!("{overlay} is not in the overlay series"));
    }
    let rom = fs::read(root.join("roms/gs1-en.gba"))
        .map_err(|error| format!("roms/gs1-en.gba: {error}"))?;
    let start = address - 0x0800_0000;
    let (bytes, _) =
        extract_resource::decode_general(&rom, start, start + compressed, decoded as u64)
            .map_err(|error| error.0)?;
    Ok(bytes)
}
pub fn truth_window(
    root: &Path,
    overlay: &str,
    address: i64,
    span: i64,
) -> Result<(Vec<u8>, &'static str), String> {
    let start = (address - OVERLAY_BASE) as usize;
    let rom = match rom_overlay(root, overlay) {
        Ok(image) => match image.get(start..start + span as usize) {
            Some(window) => return Ok((window.to_vec(), "rom")),
            None => format!(
                "the decoded container is {} bytes and the row needs {}",
                image.len(),
                start + span as usize
            ),
        },
        Err(message) => message,
    };
    let assembled = match placeholder_span(root, overlay, address) {
        Ok(None) => {
            let path = root
                .join("assets/code")
                .join(format!("{overlay}_overlay.s"));
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
        .map_err(|git| {
            format!("{git}; no ROM window either: {rom}; and not from the assembly: {assembled}")
        })
}
pub fn placeholder_span(root: &Path, overlay: &str, address: i64) -> Result<Option<i64>, String> {
    let path = root.join(format!("assets/code/{overlay}_overlay.s"));
    let assembly =
        std::fs::read_to_string(&path).map_err(|error| format!("{}: {error}", path.display()))?;
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
    let assembly = root.join(format!("assets/code/{overlay}_overlay.s"));
    let original = fs::read_to_string(&assembly).map_err(|error| error.to_string())?;
    let lines: Vec<&str> = original.split('\n').collect();
    let (start, end, span) = placeholder_block(&lines, address).ok_or_else(|| {
        format!(
            "no AlchemyC_{address:08x} placeholder in {}",
            assembly.display()
        )
    })?;
    let (reference, oracle) = truth_window(root, overlay, address, span)?;
    let from_git = pre_adoption_text(root, overlay, address)
        .and_then(|text| region_text(&define_dangling_labels(&text), address, span));
    let usable = |lines: &Vec<String>| {
        !lines
            .iter()
            .any(|line| line.trim_start().starts_with("AlchemyC_"))
    };
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
            None => {
                return Err(format!(
                    "restored assembly references .L_{label}, which no placeholder covers"
                ))
            }
        }
    }
    let image = assemble_overlay(&OverlaySource::Str(text.clone()), OVERLAY_BASE)?;
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
        fs::write(&assembly, &text).map_err(|error| error.to_string())?;
        let installed = root.join(format!("exact/{overlay}_c_{address:08x}.c"));
        if installed.exists() {
            let parked = root.join(format!("recon/gs1/en/overlays/{overlay}_c_{address:08x}.c"));
            fs::rename(&installed, &parked).map_err(|error| {
                format!(
                    "cannot move {} to the EN reconstruction corpus: {error}",
                    installed.display()
                )
            })?;
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
        let (overlay, address) = row
            .split_once(':')
            .ok_or_else(|| format!("expected <overlay>:<addressHex>, got {row}"))?;
        let address = i64::from_str_radix(address.trim_start_matches("0x"), 16)
            .map_err(|_| format!("{row}: address must be hexadecimal"))?;
        let address = if address < OVERLAY_BASE {
            OVERLAY_BASE + address
        } else {
            address
        };
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
