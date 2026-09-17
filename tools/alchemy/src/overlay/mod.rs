pub mod adopt;
pub mod assembly;
pub mod compile;
pub mod export;
pub mod owners;
pub mod park;
pub mod rom;
pub mod score;
pub mod source;
use crate::compiler::source_paths::SourceOwner;
use crate::overlay::assembly::OVERLAY_BASE;
use serde::Deserialize;
use std::collections::BTreeMap;
use std::fs;
use std::path::{Path, PathBuf};
use std::process::Command;
use tempfile::tempdir;
#[derive(Debug, Clone, PartialEq)]
pub struct InternalAlias {
    pub label: String,
    pub offset: i64,
}
#[derive(Debug, Clone, Deserialize)]
pub struct AuditInterval {
    pub start: i64,
    pub end: i64,
    pub kind: String,
}
#[derive(Deserialize)]
struct AuditReport {
    overlays: Vec<AuditOverlay>,
}
#[derive(Deserialize)]
struct AuditOverlay {
    id: String,
    intervals: Vec<AuditInterval>,
}
pub(crate) fn overlay_assembly(root: &Path, overlay: &str) -> PathBuf {
    root.join(crate::targets::target_for(crate::targets::DEFAULT_TARGET).overlay_assembly(overlay))
}
pub(crate) fn retained_source(root: &Path, owner: SourceOwner) -> PathBuf {
    root.join(format!(
        "games/THE BROKEN SEAL/recon/en/overlays/{}.c",
        owner.legacy_stem()
    ))
}
pub(crate) fn overlay_offset(owner: SourceOwner) -> usize {
    (i64::from(owner.address()) - OVERLAY_BASE) as usize
}
fn parse_listing_row(row: &str) -> Option<(i64, i64)> {
    let mut fields = row.split_whitespace();
    let line = fields.next()?;
    let offset = fields.next()?;
    let encoded = fields.next()?;
    if !line.bytes().all(|byte| byte.is_ascii_digit())
        || offset.len() < 4
        || !offset
            .bytes()
            .all(|byte| byte.is_ascii_digit() || matches!(byte, b'a'..=b'f'))
        || !encoded.bytes().next()?.is_ascii_hexdigit()
    {
        return None;
    }
    Some((line.parse().ok()?, i64::from_str_radix(offset, 16).ok()?))
}

#[derive(Debug, Clone, Copy)]
pub struct ListingRow {
    pub line: i64,
    pub offset: i64,
    pub width: i64,
}

fn parse_listing_width(row: &str) -> Option<ListingRow> {
    let mut fields = row.split_whitespace();
    let line = fields.next()?;
    let offset = fields.next()?;
    let encoded = fields.next()?;
    if !line.bytes().all(|byte| byte.is_ascii_digit())
        || !offset.bytes().all(|byte| byte.is_ascii_hexdigit())
    {
        return None;
    }
    let digits = encoded
        .bytes()
        .take_while(|byte| byte.is_ascii_hexdigit())
        .count();
    if digits < 4 || !digits.is_multiple_of(2) {
        return None;
    }
    Some(ListingRow {
        line: line.parse().ok()?,
        offset: i64::from_str_radix(offset, 16).ok()?,
        width: (digits / 2) as i64,
    })
}

pub fn listing_rows(assembly: &Path) -> Result<Vec<ListingRow>, String> {
    let work = tempdir().map_err(|error| error.to_string())?;
    let listing = work.path().join("listing.lst");
    let object = work.path().join("listing.o");
    let output = Command::new("arm-none-eabi-as")
        .args(["-mcpu=arm7tdmi", "-mthumb-interwork"])
        .arg(format!("-al={}", listing.display()))
        .arg("-o")
        .arg(&object)
        .arg(assembly)
        .output()
        .map_err(|error| format!("as failed: {error}"))?;
    if !output.status.success() {
        return Err(format!(
            "as failed: {}",
            String::from_utf8_lossy(&output.stderr).trim()
        ));
    }
    let text = fs::read_to_string(&listing).map_err(|error| error.to_string())?;
    let mut rows = Vec::new();
    let mut seen = std::collections::HashSet::new();
    for row in text.lines() {
        if let Some(parsed) = parse_listing_width(row) {
            if seen.insert(parsed.line) {
                rows.push(parsed);
            }
        }
    }
    Ok(rows)
}
pub fn listing_offsets(assembly: &Path) -> Result<Vec<(i64, i64)>, String> {
    let work = tempdir().map_err(|error| error.to_string())?;
    let listing = work.path().join("listing.lst");
    let object = work.path().join("listing.o");
    let output = Command::new("arm-none-eabi-as")
        .args(["-mcpu=arm7tdmi", "-mthumb-interwork"])
        .arg(format!("-al={}", listing.display()))
        .arg("-o")
        .arg(&object)
        .arg(assembly)
        .output()
        .map_err(|error| format!("as failed: {error}"))?;
    if !output.status.success() {
        let stderr = String::from_utf8_lossy(&output.stderr);
        return Err(format!("as failed: {}", stderr.trim()));
    }
    let text = fs::read_to_string(&listing).map_err(|error| error.to_string())?;
    let mut offsets: Vec<(i64, i64)> = Vec::new();
    let mut seen: std::collections::HashSet<i64> = std::collections::HashSet::new();
    for row in text.split('\n') {
        if let Some((line, offset)) = parse_listing_row(row) {
            if seen.insert(line) {
                offsets.push((line, offset));
            }
        }
    }
    Ok(offsets)
}
pub fn region_lines(offsets: &[(i64, i64)], offset: i64, span: i64) -> Result<(i64, i64), String> {
    let inside: Vec<i64> = offsets
        .iter()
        .filter(|&&(_, at)| at >= offset && at < offset + span)
        .map(|&(line, _)| line)
        .collect();
    if inside.is_empty() {
        return Err(format!("no assembly covers offset 0x{:x}", offset));
    }
    let first = *inside.iter().min().unwrap();
    let last = *inside.iter().max().unwrap();
    for &(line, at) in offsets {
        if line >= first && line <= last && (at < offset || at >= offset + span) {
            return Err(format!(
                "lines {first}-{last} straddle the region boundary at 0x{:x}",
                at
            ));
        }
    }
    let first_offset = offsets
        .iter()
        .find(|&(line, _)| *line == first)
        .map(|&(_, at)| at);
    if first_offset != Some(offset) {
        return Err(format!(
            "region does not start on an encoded boundary: 0x{:x}",
            first_offset.unwrap()
        ));
    }
    Ok((first, last))
}
fn parse_local_label(line: &str) -> Option<String> {
    let trimmed = line.trim_start();
    let rest = trimmed.strip_prefix(".L_")?;
    let hex_len = rest
        .chars()
        .take_while(|c| c.is_ascii_digit() || ('a'..='f').contains(c))
        .count();
    if hex_len == 0 {
        return None;
    }
    if rest[hex_len..].starts_with(':') {
        Some(format!(".L_{}", &rest[..hex_len]))
    } else {
        None
    }
}
fn strip_comments(line: &str) -> String {
    line.split('@')
        .next()
        .unwrap_or(line)
        .split("//")
        .next()
        .unwrap_or(line)
        .to_string()
}
fn is_word_char(c: char) -> bool {
    c.is_ascii_alphanumeric() || c == '_' || c == '.'
}
fn word_boundary_contains(haystack: &str, needle: &str) -> bool {
    if needle.is_empty() {
        return false;
    }
    haystack.match_indices(needle).any(|(index, _)| {
        let before = haystack[..index].chars().next_back();
        let after = haystack[index + needle.len()..].chars().next();
        before.is_none_or(|character| character == '\n' || !is_word_char(character))
            && after.is_none_or(|character| character == '\n' || !is_word_char(character))
    })
}
pub fn internal_aliases(
    lines: &[String],
    first: i64,
    last: i64,
    region_offset: i64,
    span: i64,
) -> Result<Vec<InternalAlias>, String> {
    let first_u = (first - 1).max(0) as usize;
    let last_u = (last.max(0)) as usize;
    let outside: String = lines[..first_u.min(lines.len())]
        .iter()
        .chain(lines[last_u.min(lines.len())..].iter())
        .map(|line| strip_comments(line))
        .collect::<Vec<_>>()
        .join("\n");
    let mut aliases: Vec<InternalAlias> = Vec::new();
    for line in lines.iter().take(last_u.min(lines.len())).skip(first_u) {
        let Some(label) = parse_local_label(line) else {
            continue;
        };
        if !word_boundary_contains(&outside, &label) {
            continue;
        }
        let hex_part = &label[3..]; // strip ".L_"
        let value = i64::from_str_radix(hex_part, 16)
            .map_err(|_| format!("referenced label {label} lies outside its encoded region"))?;
        let offset = value - OVERLAY_BASE - region_offset;
        if offset < 0 || offset >= span {
            return Err(format!(
                "referenced label {label} lies outside its encoded region"
            ));
        }
        aliases.push(InternalAlias { label, offset });
    }
    aliases.sort_by_key(|alias| alias.offset);
    Ok(aliases)
}
pub fn placeholder_lines(stem: &str, span: i64, aliases: &[InternalAlias]) -> Vec<String> {
    let mut result = vec![format!("AlchemyC_{stem}:")];
    let mut cursor = 0i64;
    for alias in aliases {
        if alias.offset > cursor {
            result.push(format!("\t.space 0x{:x}", alias.offset - cursor));
        }
        result.push(format!("{}:", alias.label));
        cursor = alias.offset;
    }
    if span > cursor {
        result.push(format!("\t.space 0x{:x}", span - cursor));
    }
    result
}
fn audit_intervals(root: &Path, overlay: &str) -> Result<Option<Vec<AuditInterval>>, String> {
    let path = root
        .join("games/THE BROKEN SEAL/metrics")
        .join("tbs-en-executable.json");
    if !path.exists() {
        return Ok(None);
    }
    let report: AuditReport = serde_json::from_slice(
        &fs::read(&path).map_err(|error| format!("{}: {error}", path.display()))?,
    )
    .map_err(|error| format!("{}: {error}", path.display()))?;
    Ok(report
        .overlays
        .into_iter()
        .find(|row| row.id == overlay)
        .map(|row| row.intervals))
}
pub fn audited_kind(root: &Path, overlay: &str, entry: i64) -> Result<Option<String>, String> {
    Ok(audit_intervals(root, overlay)?.and_then(|intervals| {
        intervals
            .into_iter()
            .find(|interval| interval.start <= entry && entry < interval.end)
            .map(|interval| interval.kind)
    }))
}
pub(crate) fn reviewed_spans(root: &Path) -> Result<BTreeMap<SourceOwner, usize>, String> {
    owners::reviewed_spans(
        root,
        crate::targets::target_for(crate::targets::DEFAULT_TARGET),
    )
}
use crate::compiler::routing::root;
use std::process::ExitCode;

const OVERLAY_USAGE: &str = "usage: alchemy overlay <adopt|park|audit|export> [args]";

pub(crate) fn code(result: Result<i32, String>) -> ExitCode {
    match result {
        Ok(0) => ExitCode::SUCCESS,
        Ok(_) => ExitCode::FAILURE,
        Err(error) => {
            eprintln!("{error}");
            ExitCode::FAILURE
        }
    }
}

pub fn entry(arguments: &[String]) -> ExitCode {
    let Some(command) = arguments.first().map(String::as_str) else {
        eprintln!("{OVERLAY_USAGE}");
        return ExitCode::from(2);
    };
    let rest = &arguments[1..];
    match command {
        "adopt" => code(adopt::run(root(), rest)),
        "park" => code(park::run(root(), rest)),
        "audit" => code(park::run_audit(root(), rest)),
        "export" => code(export::run(root(), rest)),
        "-h" | "--help" => {
            println!("{OVERLAY_USAGE}");
            ExitCode::SUCCESS
        }
        _ => {
            eprintln!("unknown overlay command: {command}\n{OVERLAY_USAGE}");
            ExitCode::from(2)
        }
    }
}
