pub mod park;
pub mod score;
pub mod twins;
use compiler_core::source_paths::{SourceOwner, SourcePaths};
use overlay_disasm::{assemble_overlay, OverlaySource, OVERLAY_BASE};
use serde_json::Value;
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
pub struct FunctionRow {
    pub id: String,
    pub overlay: String,
    pub entry: i64,
    pub offset: i64,
    pub span_bytes: i64,
}
#[derive(Debug, Clone, PartialEq)]
pub struct InternalAlias {
    pub label: String,
    pub offset: i64,
}
#[derive(Debug, Clone)]
pub struct AuditInterval {
    pub start: i64,
    pub end: i64,
    pub kind: String,
}
enum ParseOutcome {
    Help,
    Options(Options),
}
fn hex8(value: i64) -> String {
    format!("{:08x}", value)
}
fn parse_listing_row(row: &str) -> Option<(i64, i64)> {
    let bytes = row.as_bytes();
    let mut i = 0usize;
    while i < bytes.len() && (bytes[i] as char).is_whitespace() {
        i += 1;
    }
    let digits_start = i;
    while i < bytes.len() && bytes[i].is_ascii_digit() {
        i += 1;
    }
    if i == digits_start {
        return None;
    }
    let line: i64 = row[digits_start..i].parse().ok()?;
    let ws1_start = i;
    while i < bytes.len() && (bytes[i] as char).is_whitespace() {
        i += 1;
    }
    if i == ws1_start {
        return None;
    }
    let hex_start = i;
    while i < bytes.len() && matches!(bytes[i], b'0'..=b'9' | b'a'..=b'f') {
        i += 1;
    }
    if i - hex_start < 4 {
        return None;
    }
    let offset = i64::from_str_radix(&row[hex_start..i], 16).ok()?;
    let ws2_start = i;
    while i < bytes.len() && (bytes[i] as char).is_whitespace() {
        i += 1;
    }
    if i == ws2_start {
        return None;
    }
    if i >= bytes.len() || !(bytes[i] as char).is_ascii_hexdigit() {
        return None;
    }
    Some((line, offset))
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
    let inside: Vec<i64> =
        offsets.iter().filter(|&&(_, at)| at >= offset && at < offset + span).map(|&(line, _)| line).collect();
    if inside.is_empty() {
        return Err(format!("no assembly covers offset 0x{:x}", offset));
    }
    let first = *inside.iter().min().unwrap();
    let last = *inside.iter().max().unwrap();
    for &(line, at) in offsets {
        if line >= first && line <= last && (at < offset || at >= offset + span) {
            return Err(format!("lines {first}-{last} straddle the region boundary at 0x{:x}", at));
        }
    }
    let first_offset = offsets.iter().find(|&(line, _)| *line == first).map(|&(_, at)| at);
    if first_offset != Some(offset) {
        return Err(format!("region does not start on an encoded boundary: 0x{:x}", first_offset.unwrap()));
    }
    Ok((first, last))
}
fn parse_local_label(line: &str) -> Option<String> {
    let trimmed = line.trim_start();
    let rest = trimmed.strip_prefix(".L_")?;
    let hex_len = rest.chars().take_while(|c| c.is_ascii_digit() || ('a'..='f').contains(c)).count();
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
    let step1 = match line.find('@') {
        Some(index) => &line[..index],
        None => line,
    };
    let step2 = match step1.find("//") {
        Some(index) => &step1[..index],
        None => step1,
    };
    step2.to_string()
}
fn is_word_char(c: char) -> bool {
    c.is_ascii_alphanumeric() || c == '_' || c == '.'
}
fn word_boundary_contains(haystack: &str, needle: &str) -> bool {
    if needle.is_empty() {
        return false;
    }
    let bytes = haystack.as_bytes();
    let nbytes = needle.as_bytes();
    let n = nbytes.len();
    let mut i = 0usize;
    while i + n <= bytes.len() {
        if &bytes[i..i + n] == nbytes {
            let before_ok = if i == 0 {
                true
            } else {
                let c = haystack[..i].chars().last().unwrap();
                c == '\n' || !is_word_char(c)
            };
            let after = i + n;
            let after_ok = if after == bytes.len() {
                true
            } else {
                let c = haystack[after..].chars().next().unwrap();
                c == '\n' || !is_word_char(c)
            };
            if before_ok && after_ok {
                return true;
            }
        }
        i += 1;
    }
    false
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
    for index in first_u..last_u.min(lines.len()) {
        let Some(label) = parse_local_label(&lines[index]) else {
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
            return Err(format!("referenced label {label} lies outside its encoded region"));
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
    let report = root.join("games/gs1/metrics").join("gs1-en-executable.json");
    if !report.exists() {
        return Ok(None);
    }
    let text = fs::read_to_string(&report).map_err(|error| error.to_string())?;
    let value: Value = serde_json::from_str(&text).map_err(|error| error.to_string())?;
    let overlays = value
        .get("overlays")
        .and_then(Value::as_array)
        .ok_or_else(|| "games/gs1/metrics/gs1-en-executable.json: unexpected shape".to_string())?;
    let mut found: Option<Vec<AuditInterval>> = None;
    for row in overlays {
        if row.get("id").and_then(Value::as_str) != Some(overlay) {
            continue;
        }
        let intervals = row
            .get("intervals")
            .and_then(Value::as_array)
            .ok_or_else(|| "games/gs1/metrics/gs1-en-executable.json: unexpected shape".to_string())?;
        let mut out = Vec::with_capacity(intervals.len());
        for interval in intervals {
            out.push(AuditInterval {
                start: interval.get("start").and_then(Value::as_i64).unwrap_or(0),
                end: interval.get("end").and_then(Value::as_i64).unwrap_or(0),
                kind: interval.get("kind").and_then(Value::as_str).unwrap_or("").to_string(),
            });
        }
        found = Some(out);
    }
    Ok(found)
}
pub fn audited_code_span(root: &Path, overlay: &str, entry: i64) -> Result<Option<i64>, String> {
    let intervals = match audit_intervals(root, overlay)? {
        Some(intervals) => intervals,
        None => return Ok(None),
    };
    let code = intervals.iter().find(|interval| {
        interval.start <= entry && entry < interval.end && (interval.kind == "thumb" || interval.kind == "arm")
    });
    Ok(code.map(|interval| interval.end - entry))
}
pub fn span_is_adoptable(root: &Path, overlay: &str, entry: i64, span_bytes: i64) -> bool {
    audited_span(root, overlay, entry, span_bytes, overlay).is_ok()
}
fn audited_interval(root: &Path, fn_row: &FunctionRow) -> Result<(), String> {
    audited_span(root, &fn_row.overlay, fn_row.entry, fn_row.span_bytes, &fn_row.id)
}
fn audited_span(root: &Path, overlay: &str, start: i64, span_bytes: i64, id: &str) -> Result<(), String> {
    let intervals = match audit_intervals(root, overlay)? {
        Some(intervals) => intervals,
        None => return Ok(()), // un-audited overlay: not this check's call to make
    };
    let end = start + span_bytes;
    if intervals.iter().any(|interval| interval.start <= start && end <= interval.end) {
        return Ok(());
    }
    let mut touched: Vec<&AuditInterval> =
        intervals.iter().filter(|interval| interval.start < end && start < interval.end).collect();
    touched.sort_by_key(|interval| interval.start);
    let tiles = !touched.is_empty()
        && touched[0].start <= start
        && touched[touched.len() - 1].end >= end
        && touched.windows(2).all(|pair| pair[0].end == pair[1].start);
    let last = touched.last();
    if tiles && last.map(|interval| interval.kind.as_str()) != Some("executable_alignment") {
        return Ok(());
    }
    // Some GCC by-value entry points reserve incoming stack arguments before
    // the conventional push. The interval audit can miss that first
    // instruction even though the reviewed owner record explicitly includes
    // it. Only the exact recorded owner span may bridge such a gap.
    if reviewed_owner_span(root, overlay, start, span_bytes)? {
        return Ok(());
    }
    let detail = if touched.is_empty() {
        "no audited executable interval covers it".to_string()
    } else {
        touched
            .iter()
            .map(|interval| format!("[0x{:08x},0x{:08x}) {}", interval.start, interval.end, interval.kind))
            .collect::<Vec<_>>()
            .join(" + ")
    };
    let code = touched.iter().find(|interval| interval.kind == "thumb" || interval.kind == "arm");
    let suggestion = match code {
        Some(code) if code.start == start && code.end < end => {
            format!(" -- the audited code ends at 0x{:08x}; retry with --span {}", code.end, code.end - start)
        }
        _ => String::new(),
    };
    Err(format!(
        "{} span 0x{:08x}..0x{:08x} is not inside one audited executable interval: {}{}",
        id, start, end, detail, suggestion
    ))
}

fn reviewed_owner_span(root: &Path, overlay: &str, start: i64, span_bytes: i64) -> Result<bool, String> {
    let path = root.join("games/gs1/semantic/regions.json");
    let document: Value =
        serde_json::from_str(&fs::read_to_string(&path).map_err(|error| format!("{}: {error}", path.display()))?)
            .map_err(|error| format!("{}: {error}", path.display()))?;
    let Some(regions) = document.get("manual_regions").and_then(Value::as_array) else {
        return Err(format!("{} has no manual_regions array", path.display()));
    };
    Ok(regions.iter().any(|region| {
        let entry = region
            .get("entry")
            .and_then(Value::as_str)
            .and_then(|value| i64::from_str_radix(value.trim_start_matches("0x"), 16).ok());
        region.get("overlay").and_then(Value::as_str) == Some(overlay)
            && entry == Some(start)
            && region.get("span_bytes").and_then(Value::as_i64) == Some(span_bytes)
    }))
}
const USAGE: &str = "usage: overlay-adopt <overlay:offsetHex> --source FILE [--span BYTES] [--apply] [--where]";
fn options_of(argv: &[String]) -> Result<ParseOutcome, String> {
    let mut span: Option<i64> = None;
    let mut id = String::new();
    let mut source = String::new();
    let mut apply = false;
    let mut where_ = false;
    let mut index = 0usize;
    while index < argv.len() {
        let argument = argv[index].as_str();
        if argument == "--source" || argument == "-s" {
            index += 1;
            source = argv.get(index).cloned().unwrap_or_default();
        } else if argument == "--apply" {
            apply = true;
        } else if argument == "--where" {
            where_ = true;
        } else if argument == "--span" {
            index += 1;
            let raw = argv.get(index).cloned().unwrap_or_default();
            let parsed: f64 = raw.trim().parse().unwrap_or(f64::NAN);
            if !parsed.is_finite() || parsed.fract() != 0.0 || parsed <= 0.0 {
                return Err("--span must be a positive byte count".to_string());
            }
            span = Some(parsed as i64);
        } else if argument == "-h" || argument == "--help" {
            return Ok(ParseOutcome::Help);
        } else if id.is_empty() {
            id = argument.to_string();
        } else {
            return Err(format!("unrecognized argument: {argument}"));
        }
        index += 1;
    }
    if id.is_empty() || source.is_empty() {
        return Err("both an overlay function id and --source are required".to_string());
    }
    Ok(ParseOutcome::Options(Options { span, id, source, apply, where_ }))
}
struct OverlayLock {
    path: PathBuf,
}
impl OverlayLock {
    fn acquire(assembly: &Path) -> Result<Self, String> {
        let path = assembly.with_extension("s.adopt-lock");
        for attempt in 0..600 {
            match fs::OpenOptions::new().write(true).create_new(true).open(&path) {
                Ok(_) => return Ok(OverlayLock { path }),
                Err(error) if error.kind() == std::io::ErrorKind::AlreadyExists => {
                    if attempt == 0 {
                        eprintln!("waiting for another adoption to finish with {}", assembly.display());
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
fn revert(installed: &Path, assembly: &Path, preexisting: &Option<Vec<u8>>, original_text: &str) -> Result<(), String> {
    match preexisting {
        Some(data) => fs::write(installed, data).map_err(|error| error.to_string())?,
        None => {
            let _ = fs::remove_file(installed);
        }
    }
    fs::write(assembly, original_text).map_err(|error| error.to_string())
}
pub fn run(root: &Path, args: &[String]) -> Result<i32, String> {
    let options = match options_of(args)? {
        ParseOutcome::Help => {
            println!("{USAGE}");
            return Ok(0);
        }
        ParseOutcome::Options(options) => options,
    };
    let inventory_path: PathBuf = root.join("out/decomp/overlays.json");
    let inventory_text = fs::read_to_string(&inventory_path).map_err(|error| error.to_string())?;
    let inventory: Value = serde_json::from_str(&inventory_text).map_err(|error| error.to_string())?;
    let functions = inventory
        .get("functions")
        .and_then(Value::as_array)
        .ok_or_else(|| "out/decomp/overlays.json: unexpected shape".to_string())?;
    let found = functions.iter().find(|row| row.get("id").and_then(Value::as_str) == Some(options.id.as_str()));
    let fn_row: FunctionRow = if let Some(row) = found {
        FunctionRow {
            id: options.id.clone(),
            overlay: row.get("overlay").and_then(Value::as_str).unwrap_or("").to_string(),
            entry: row.get("entry").and_then(Value::as_i64).unwrap_or(0),
            offset: row.get("offset").and_then(Value::as_i64).unwrap_or(0),
            span_bytes: row.get("span_bytes").and_then(Value::as_i64).unwrap_or(0),
        }
    } else if let Some(span) = options.span {
        let mut parts = options.id.splitn(2, ':');
        let overlay = parts.next().unwrap_or("").to_string();
        let offset_text = parts.next().unwrap_or("");
        let offset = i64::from_str_radix(offset_text.trim_start_matches("0x"), 16)
            .map_err(|_| format!("unparseable overlay id: {}", options.id))?;
        let offset = if offset >= OVERLAY_BASE { offset - OVERLAY_BASE } else { offset };
        FunctionRow { id: options.id.clone(), overlay, entry: OVERLAY_BASE + offset, offset, span_bytes: span }
    } else {
        return Err(format!(
            "no such overlay function: {} (pass --span BYTES to adopt an undiscovered entry)",
            options.id
        ));
    };
    let stem = hex8(fn_row.entry);
    if fn_row.entry - OVERLAY_BASE != fn_row.offset {
        return Err("inventory entry and offset disagree".to_string());
    }
    audited_interval(root, &fn_row)?;
    let assembly = root.join("games/gs1/assets/code").join(format!("{}_overlay.s", fn_row.overlay));
    let _lock = OverlayLock::acquire(&assembly)?;
    let baseline = assemble_overlay(&OverlaySource::path(&assembly), OVERLAY_BASE)?;
    let original_text = fs::read_to_string(&assembly).map_err(|error| error.to_string())?;
    let lines: Vec<String> = original_text.split('\n').map(|line| line.to_string()).collect();
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
    let owner = SourceOwner::parse(&format!("{}:{}", fn_row.overlay, stem))?;
    let source_paths = SourcePaths::load(root)?;
    let installed = source_paths.registered_source_path(owner)?;
    let preexisting =
        if installed.exists() { Some(fs::read(&installed).map_err(|error| error.to_string())?) } else { None };
    let shared_with_other_owners =
        source_paths.owners_for_path(&installed).into_iter().any(|registered| registered != owner);
    if shared_with_other_owners {
        let candidate = fs::read(&options.source).map_err(|error| error.to_string())?;
        if preexisting.as_deref() != Some(candidate.as_slice()) {
            return Err(format!(
                "{} is shared by other exact owners; refusing to overwrite it with differing source",
                installed.display()
            ));
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
        let mut differing = (rebuilt.len() as i64 - baseline.len() as i64).abs();
        let min_len = rebuilt.len().min(baseline.len());
        for byte in 0..min_len {
            if rebuilt[byte] != baseline[byte] {
                differing += 1;
            }
        }
        let mut addresses: Vec<i64> = Vec::new();
        if options.where_ {
            for byte in 0..min_len {
                if rebuilt[byte] != baseline[byte] {
                    addresses.push(byte as i64);
                }
            }
        }
        revert(&installed, &assembly, &preexisting, &original_text)?;
        println!(
            "adopt=rejected {} differing_bytes={} size={}/{}",
            options.id,
            differing,
            rebuilt.len(),
            baseline.len()
        );
        if options.where_ {
            let mut runs: Vec<String> = Vec::new();
            let mut index = 0usize;
            while index < addresses.len() {
                let mut end = index;
                while end + 1 < addresses.len() && addresses[end + 1] == addresses[end] + 1 {
                    end += 1;
                }
                let from = OVERLAY_BASE + addresses[index];
                let bytes = (end - index + 1) as i64;
                runs.push(format!("0x{:x}+{bytes}", from));
                index = end + 1;
            }
            let text = if runs.is_empty() { "(none; length change only)".to_string() } else { runs.join(" ") };
            println!("differing_at {text}");
        }
        return Ok(1);
    }
    if !options.apply {
        revert(&installed, &assembly, &preexisting, &original_text)?;
        let source_base = Path::new(&options.source)
            .file_name()
            .map(|name| name.to_string_lossy().to_string())
            .unwrap_or_else(|| options.source.clone());
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
