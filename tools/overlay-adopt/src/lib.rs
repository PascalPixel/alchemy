pub mod park;
pub mod score;
pub mod twins;
use compiler_core::source_paths::{SourceOwner, SourcePaths};
use no_asm_c::{expanded_forbidden, find_forbidden};
use overlay_disasm::{assemble_overlay, OverlaySource, OVERLAY_BASE};
use serde::Deserialize;
use serde_json::Value;
use std::collections::BTreeMap;
use std::fs;
use std::path::{Path, PathBuf};
use std::process::Command;
use tempfile::tempdir;
#[derive(Debug, Clone)]
pub struct Options {
    pub span: Option<i64>,
    pub id: String,
    pub source: String,
    pub apply: bool,
    pub where_: bool,
}
#[derive(Debug, Clone)]
struct FunctionRow {
    overlay: String,
    entry: i64,
    offset: i64,
    span_bytes: i64,
}
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
#[derive(Deserialize)]
struct ReviewedRegions {
    manual_regions: Vec<ReviewedRegion>,
}
#[derive(Deserialize)]
struct ReviewedRegion {
    overlay: String,
    entry: String,
    span_bytes: usize,
}
pub(crate) fn overlay_assembly(root: &Path, overlay: &str) -> PathBuf {
    root.join(format!("games/gs1/assets/code/{overlay}_overlay.s"))
}
pub(crate) fn retained_source(root: &Path, owner: SourceOwner) -> PathBuf {
    root.join(format!(
        "games/gs1/recon/en/overlays/{}.c",
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
pub fn listing_offsets(assembly: &Path) -> Result<Vec<(i64, i64)>, String> {
    let work = tempdir().map_err(|error| error.to_string())?;
    let listing = work.path().join("listing.txt");
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
        .join("games/gs1/metrics")
        .join("gs1-en-executable.json");
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
    let path = root.join("games/gs1/semantic/regions.json");
    let document: ReviewedRegions = serde_json::from_slice(
        &fs::read(&path).map_err(|error| format!("{}: {error}", path.display()))?,
    )
    .map_err(|error| format!("{}: {error}", path.display()))?;
    document
        .manual_regions
        .into_iter()
        .map(|region| {
            let owner = SourceOwner::parse(&format!(
                "{}:{}",
                region.overlay,
                region.entry.trim_start_matches("0x")
            ))?;
            (region.span_bytes > 0)
                .then_some((owner, region.span_bytes))
                .ok_or_else(|| "overlay region has no positive span_bytes".into())
        })
        .collect()
}
fn audited_span(
    root: &Path,
    overlay: &str,
    start: i64,
    span_bytes: i64,
    id: &str,
) -> Result<(), String> {
    let intervals = audit_intervals(root, overlay)?
        .ok_or_else(|| format!("{id}: {overlay} has no executable audit"))?;
    let end = start + span_bytes;
    if intervals.iter().any(|interval| {
        interval.start <= start
            && end <= interval.end
            && matches!(interval.kind.as_str(), "thumb" | "arm")
    }) {
        return Ok(());
    }
    let mut touched: Vec<&AuditInterval> = intervals
        .iter()
        .filter(|interval| interval.start < end && start < interval.end)
        .collect();
    touched.sort_by_key(|interval| interval.start);
    let tiles = !touched.is_empty()
        && touched[0].start <= start
        && touched[touched.len() - 1].end >= end
        && touched.windows(2).all(|pair| pair[0].end == pair[1].start);
    let starts_in_code = touched
        .first()
        .is_some_and(|interval| matches!(interval.kind.as_str(), "thumb" | "arm"));
    if tiles
        && starts_in_code
        && touched.last().map(|interval| interval.kind.as_str()) != Some("executable_alignment")
    {
        return Ok(());
    }
    // Only an exact reviewed owner may bridge a missed pre-prologue instruction.
    let owner = SourceOwner::parse(&format!("{overlay}:{start:08x}"))?;
    if reviewed_spans(root)?.get(&owner).copied() == usize::try_from(span_bytes).ok() {
        return Ok(());
    }
    let mut detail = touched
        .iter()
        .map(|interval| {
            format!(
                "[{:#010x},{:#010x}) {}",
                interval.start, interval.end, interval.kind
            )
        })
        .collect::<Vec<_>>()
        .join(" + ");
    if detail.is_empty() {
        detail.push_str("no audited executable interval covers it");
    }
    Err(format!(
        "{id} span 0x{start:08x}..0x{end:08x} is not inside one audited executable interval: {detail}"
    ))
}
const USAGE: &str =
    "usage: overlay-adopt <overlay:offsetHex> --source FILE [--span BYTES] [--apply] [--where]";
fn options_of(argv: &[String]) -> Result<Option<Options>, String> {
    let (mut span, mut id, mut source) = (None, String::new(), String::new());
    let (mut apply, mut where_) = (false, false);
    let mut args = argv.iter();
    while let Some(argument) = args.next() {
        match argument.as_str() {
            "--source" | "-s" => source = args.next().cloned().unwrap_or_default(),
            "--span" => {
                span = Some(
                    args.next()
                        .and_then(|value| value.parse::<i64>().ok())
                        .filter(|value| *value > 0)
                        .ok_or("--span must be a positive byte count")?,
                )
            }
            "--apply" => apply = true,
            "--where" => where_ = true,
            "-h" | "--help" => return Ok(None),
            _ if id.is_empty() => id = argument.clone(),
            _ => return Err(format!("unrecognized argument: {argument}")),
        }
    }
    if id.is_empty() || source.is_empty() {
        return Err("both an overlay function id and --source are required".to_string());
    }
    Ok(Some(Options {
        span,
        id,
        source,
        apply,
        where_,
    }))
}
struct OverlayLock {
    path: PathBuf,
}
impl OverlayLock {
    fn acquire(assembly: &Path) -> Result<Self, String> {
        let path = assembly.with_extension("s.adopt-lock");
        for attempt in 0..600 {
            match fs::OpenOptions::new()
                .write(true)
                .create_new(true)
                .open(&path)
            {
                Ok(_) => return Ok(OverlayLock { path }),
                Err(error) if error.kind() == std::io::ErrorKind::AlreadyExists => {
                    if attempt == 0 {
                        eprintln!(
                            "waiting for another adoption to finish with {}",
                            assembly.display()
                        );
                    }
                    std::thread::sleep(std::time::Duration::from_millis(100));
                }
                Err(error) => return Err(format!("{}: {error}", path.display())),
            }
        }
        Err(format!(
            "{} is still locked after 60s. If no other adoption is running, a \
             previous one was killed mid-splice: check `git status games/gs1/assets/code/` \
             before deleting the lock file.",
            path.display()
        ))
    }
}
impl Drop for OverlayLock {
    fn drop(&mut self) {
        let _ = fs::remove_file(&self.path);
    }
}
fn revert(
    installed: &Path,
    assembly: &Path,
    preexisting: &Option<Vec<u8>>,
    original_text: &str,
) -> Result<(), String> {
    match preexisting {
        Some(data) => fs::write(installed, data).map_err(|error| error.to_string())?,
        None => {
            let _ = fs::remove_file(installed);
        }
    }
    fs::write(assembly, original_text).map_err(|error| error.to_string())
}
pub(crate) fn differing_units(actual: &[u8], expected: &[u8], width: usize) -> usize {
    actual
        .chunks(width)
        .zip(expected.chunks(width))
        .filter(|(left, right)| left != right)
        .count()
        + actual.len().abs_diff(expected.len()).div_ceil(width)
}
fn differing_runs(actual: &[u8], expected: &[u8]) -> String {
    let mut runs = Vec::new();
    let mut start = None;
    let mut previous = 0;
    for offset in
        (0..actual.len().min(expected.len())).filter(|offset| actual[*offset] != expected[*offset])
    {
        if start.is_some() && offset != previous + 1 {
            runs.push((start.take().unwrap(), previous));
        }
        start.get_or_insert(offset);
        previous = offset;
    }
    if let Some(start) = start {
        runs.push((start, previous));
    }
    if runs.is_empty() {
        return "(none; length change only)".to_string();
    }
    runs.into_iter()
        .map(|(start, end)| format!("0x{:x}+{}", OVERLAY_BASE + start as i64, end - start + 1))
        .collect::<Vec<_>>()
        .join(" ")
}
pub fn run(root: &Path, args: &[String]) -> Result<i32, String> {
    let Some(options) = options_of(args)? else {
        println!("{USAGE}");
        return Ok(0);
    };
    let source_text = fs::read_to_string(&options.source)
        .map_err(|error| format!("{}: {error}", options.source))?;
    let mut forbidden = find_forbidden(&options.source, &source_text)
        .into_iter()
        .map(|finding| format!("{}:{}", finding.token, finding.line))
        .collect::<Vec<_>>()
        .join(",");
    // The generated function inventory has no writer in the current tree
    // (its generator was retired in a consolidation wave). Adoption safety
    // never rested on it — audited_span validates every entry against the
    // tracked audited intervals — so a missing inventory falls back to the
    // explicit --span path instead of failing before it.
    let inventory_path: PathBuf = root.join("out/decomp/overlays.json");
    let inventory_text =
        fs::read_to_string(&inventory_path).unwrap_or_else(|_| r#"{"functions":[]}"#.to_string());
    let inventory: Value =
        serde_json::from_str(&inventory_text).map_err(|error| error.to_string())?;
    let functions = inventory
        .get("functions")
        .and_then(Value::as_array)
        .ok_or_else(|| "out/decomp/overlays.json: unexpected shape".to_string())?;
    let found = functions
        .iter()
        .find(|row| row.get("id").and_then(Value::as_str) == Some(options.id.as_str()));
    let fn_row: FunctionRow = if let Some(row) = found {
        FunctionRow {
            overlay: row["overlay"].as_str().unwrap_or("").to_string(),
            entry: row["entry"].as_i64().unwrap_or(0),
            offset: row["offset"].as_i64().unwrap_or(0),
            span_bytes: row["span_bytes"].as_i64().unwrap_or(0),
        }
    } else if let Some(span) = options.span {
        let mut parts = options.id.splitn(2, ':');
        let overlay = parts.next().unwrap_or("").to_string();
        let offset_text = parts.next().unwrap_or("");
        let offset = i64::from_str_radix(offset_text.trim_start_matches("0x"), 16)
            .map_err(|_| format!("unparseable overlay id: {}", options.id))?;
        let offset = if offset >= OVERLAY_BASE {
            offset - OVERLAY_BASE
        } else {
            offset
        };
        FunctionRow {
            overlay,
            entry: OVERLAY_BASE + offset,
            offset,
            span_bytes: span,
        }
    } else {
        return Err(format!(
            "no such overlay function: {} (pass --span BYTES to adopt an undiscovered entry)",
            options.id
        ));
    };
    let owner = SourceOwner::parse(&format!("{}:{:08x}", fn_row.overlay, fn_row.entry))?;
    let source_paths = SourcePaths::load(root)?;
    let installed = source_paths.registered_source_path(owner)?;
    let stem = owner.address_stem();
    if fn_row.entry - OVERLAY_BASE != fn_row.offset {
        return Err("inventory entry and offset disagree".to_string());
    }
    audited_span(
        root,
        &fn_row.overlay,
        fn_row.entry,
        fn_row.span_bytes,
        &options.id,
    )?;
    let assembly = overlay_assembly(root, &fn_row.overlay);
    let _lock = OverlayLock::acquire(&assembly)?;
    let baseline = assemble_overlay(&OverlaySource::path(&assembly), OVERLAY_BASE)?;
    let original_text = fs::read_to_string(&assembly).map_err(|error| error.to_string())?;
    let lines: Vec<String> = original_text
        .split('\n')
        .map(|line| line.to_string())
        .collect();
    let offsets = listing_offsets(&assembly)?;
    let (first, last) = region_lines(&offsets, fn_row.offset, fn_row.span_bytes)?;
    let marker = format!("AlchemyC_{stem}:");
    if lines.iter().any(|line| line == &marker) {
        return Err(format!("{} is already adopted as C", options.id));
    }
    let aliases = internal_aliases(&lines, first, last, fn_row.offset, fn_row.span_bytes)?;
    let mut replaced_lines: Vec<String> = Vec::with_capacity(lines.len());
    replaced_lines.extend(lines[..(first - 1) as usize].iter().cloned());
    replaced_lines.extend(placeholder_lines(&stem, fn_row.span_bytes, &aliases));
    replaced_lines.extend(lines[last as usize..].iter().cloned());
    let replaced = replaced_lines.join("\n");
    let preexisting = if installed.exists() {
        Some(fs::read(&installed).map_err(|error| error.to_string())?)
    } else {
        None
    };
    let shared_with_other_owners = source_paths
        .owners_for_path(&installed)
        .into_iter()
        .any(|registered| registered != owner);
    if shared_with_other_owners {
        let candidate = fs::read(&options.source).map_err(|error| error.to_string())?;
        if preexisting.as_deref() != Some(candidate.as_slice()) {
            return Err(format!("{} is shared by other exact owners; refusing to overwrite it with differing source", installed.display()));
        }
    }
    let source_is_installed = fs::canonicalize(&options.source)
        .ok()
        .zip(fs::canonicalize(&installed).ok())
        .is_some_and(|(source, destination)| source == destination);
    let rebuild: Result<Vec<u8>, String> = (|| {
        if let Some(parent) = installed.parent() {
            fs::create_dir_all(parent).map_err(|error| error.to_string())?;
        }
        if !source_is_installed {
            fs::copy(&options.source, &installed).map_err(|error| error.to_string())?;
        }
        fs::write(&assembly, &replaced).map_err(|error| error.to_string())?;
        assemble_overlay(&OverlaySource::path(&assembly), OVERLAY_BASE)
    })();
    let rebuilt = match rebuild {
        Ok(data) => data,
        Err(error) => {
            revert(&installed, &assembly, &preexisting, &original_text)?;
            return Err(error);
        }
    };
    if rebuilt.len() != baseline.len() || rebuilt != baseline {
        let differing = differing_units(&rebuilt, &baseline, 1);
        revert(&installed, &assembly, &preexisting, &original_text)?;
        println!(
            "adopt=rejected {} differing_bytes={} size={}/{}",
            options.id,
            differing,
            rebuilt.len(),
            baseline.len()
        );
        if options.where_ {
            println!("differing_at {}", differing_runs(&rebuilt, &baseline));
        }
        return Ok(1);
    }
    if forbidden.is_empty() {
        forbidden = match expanded_forbidden(root, &installed) {
            Ok(forbidden) => forbidden,
            Err(error) => {
                revert(&installed, &assembly, &preexisting, &original_text)?;
                return Err(error);
            }
        };
    }
    if !forbidden.is_empty() {
        revert(&installed, &assembly, &preexisting, &original_text)?;
        println!(
            "adopt=evidence-only {} exact_bytes={} forbidden={} source_retained=true",
            options.id, fn_row.span_bytes, forbidden
        );
        return Ok(1);
    }
    if !options.apply {
        revert(&installed, &assembly, &preexisting, &original_text)?;
        let source_base = overlay_disasm::paths::basename(&options.source);
        println!(
            "adopt=ready {} span={} aliases={} lines={}-{} source={} (pass --apply to install)",
            options.id,
            fn_row.span_bytes,
            aliases.len(),
            first,
            last,
            source_base
        );
        return Ok(0);
    }
    println!(
        "adopt=applied {} span={} aliases={} c={}",
        options.id,
        fn_row.span_bytes,
        aliases.len(),
        source_paths.repository_relative_path(owner).display()
    );
    Ok(0)
}
