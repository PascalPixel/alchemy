//! `tools/overlay/overlay_adopt.ts`, in Rust.
//!
//! Adopt a byte-exact overlay C reconstruction: install it as the overlay's
//! `_c_<address>.c` sibling and swap the reconstruction assembly for the
//! `AlchemyC_<address>: / .space` placeholder the asset builder expects.
//!
//! The swap is only committed when the rebuilt overlay is byte-identical to
//! the overlay as it stands today, so adoption can never change the ROM
//! image. The region's instructions are located with the assembler's own
//! listing rather than by counting directive widths: Thumb encodings are 2 or
//! 4 bytes and a hand-rolled width table would be wrong exactly where it
//! matters.
//!
//! The TypeScript original is retained until this crate is wired into
//! `tools/overlay/index.ts` and verified byte-identical on stdout, stderr and
//! exit code.

use std::fs;
use std::path::{Path, PathBuf};
use std::process::Command;

use exact_reading_list::json::{parse as parse_json, Value};
use overlay_disasm::compile::TempDir;
use overlay_disasm::{assemble_overlay, OverlaySource, OVERLAY_BASE};

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

/// `hex8(value)`: `value.toString(16).padStart(8, "0")`.
fn hex8(value: i64) -> String {
    format!("{:08x}", value)
}

// ---------------------------------------------------------------------------
// Assembler listing -> source line -> byte offset
// ---------------------------------------------------------------------------

/// A listing row: `"<line> <offset> <bytes> <source>"`; continuation rows of
/// a wide directive repeat the line number and omit the offset. GNU as prints
/// the offset in lowercase but the byte column in uppercase, so the byte
/// column has to be matched case-insensitively: a lowercase-only class
/// silently drops every row whose first byte is >= 0xa0, which shortens the
/// region by those rows and makes a byte-exact candidate fail the rebuild by
/// the dropped byte count.
///
/// `/^\s*(\d+)\s+([0-9a-f]{4,})\s+[0-9A-Fa-f]/`, matched by hand rather than
/// via a general regex engine: the listing format is rigidly column-based, so
/// a single greedy left-to-right scan (no backtracking) reproduces the same
/// match set.
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

/// Source line number -> section offset, straight from the assembler.
/// Keeps the FIRST offset seen per line, matching the TS `Map` semantics
/// (`if (!offsets.has(line)) offsets.set(...)`).
fn listing_offsets(assembly: &Path) -> Result<Vec<(i64, i64)>, String> {
    let work = TempDir::new("alchemy-adopt-").map_err(|error| error.to_string())?;
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

/// The lines that encode `[offset, offset + span)`, as a half-open line range.
fn region_lines(offsets: &[(i64, i64)], offset: i64, span: i64) -> Result<(i64, i64), String> {
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
    let first_offset = offsets.iter().find(|&(line, _)| *line == first).map(|&(_, at)| at);
    if first_offset != Some(offset) {
        return Err(format!(
            "region does not start on an encoded boundary: 0x{:x}",
            first_offset.unwrap()
        ));
    }
    Ok((first, last))
}

// ---------------------------------------------------------------------------
// Internal aliases and the placeholder they produce
// ---------------------------------------------------------------------------

/// `/^\s*(\.L_[0-9a-f]+):/`, matched by hand.
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

/// `line.replace(/@.*$/, "").replace(/\/\/.*$/, "")`: truncate at the earlier
/// of an `@` comment or a `//` comment.
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

/// `new RegExp(`(^|[^A-Za-z0-9_.])${escaped}([^A-Za-z0-9_.]|$)`, "m").test(outside)`.
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

/// Preserve labels inside a C-owned span when assembly outside the span still
/// branches to them. These are genuine secondary entry points: the exact C
/// bytes remain callable at the same address, while the alias keeps the
/// surrounding assembly linkable.
///
/// `first`/`last` are the 1-indexed inclusive line range from [`region_lines`]
/// (i.e. `lines[first - 1 ..= last - 1]` in 0-indexed terms).
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
        let Some(label) = parse_local_label(&lines[index]) else { continue };
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

pub fn self_test() -> Result<(), String> {
    let lines: Vec<String> = [
        "\tb .L_02000006",
        "\tmovs r0, #0 @ .L_02000008 is comment-only",
        "Func_02000004:",
        "\tmovs r0, #0",
        ".L_02000006:",
        "\tbx lr",
        ".L_02000008:",
        "\t.2byte 0",
    ]
    .iter()
    .map(|line| line.to_string())
    .collect();
    let aliases = internal_aliases(&lines, 3, 6, 4, 4)?;
    if aliases.len() != 1 || aliases[0].label != ".L_02000006" || aliases[0].offset != 2 {
        return Err("internal-entry alias self-test failed".to_string());
    }
    let placeholder = placeholder_lines("02000004", 4, &aliases).join("\n");
    if placeholder != "AlchemyC_02000004:\n\t.space 0x2\n.L_02000006:\n\t.space 0x2" {
        return Err("segmented placeholder self-test failed".to_string());
    }
    println!("self-test=ok");
    Ok(())
}

// ---------------------------------------------------------------------------
// Audited-interval safety check
// ---------------------------------------------------------------------------

fn audit_intervals(root: &Path, overlay: &str) -> Result<Option<Vec<AuditInterval>>, String> {
    let report = root.join("metrics").join("gs1-en-executable.json");
    if !report.exists() {
        return Ok(None);
    }
    let text = fs::read_to_string(&report).map_err(|error| error.to_string())?;
    let value = parse_json(&text)?;
    let overlays = value
        .get("overlays")
        .and_then(Value::as_array)
        .ok_or_else(|| "metrics/gs1-en-executable.json: unexpected shape".to_string())?;
    // `Map` semantics: on a duplicate id the LAST entry wins, so scan forward
    // and let a later match overwrite an earlier one.
    let mut found: Option<Vec<AuditInterval>> = None;
    for row in overlays {
        if row.get("id").and_then(Value::as_str) != Some(overlay) {
            continue;
        }
        let intervals = row
            .get("intervals")
            .and_then(Value::as_array)
            .ok_or_else(|| "metrics/gs1-en-executable.json: unexpected shape".to_string())?;
        let mut out = Vec::with_capacity(intervals.len());
        for interval in intervals {
            out.push(AuditInterval {
                start: interval.get("start").and_then(Value::as_f64).unwrap_or(0.0) as i64,
                end: interval.get("end").and_then(Value::as_f64).unwrap_or(0.0) as i64,
                kind: interval.get("kind").and_then(Value::as_str).unwrap_or("").to_string(),
            });
        }
        found = Some(out);
    }
    Ok(found)
}

/// The audited code length of the function entered at `entry`, when the
/// audit ends its code interval sooner than the registered span does.
pub fn audited_code_span(root: &Path, overlay: &str, entry: i64) -> Result<Option<i64>, String> {
    let intervals = match audit_intervals(root, overlay)? {
        Some(intervals) => intervals,
        None => return Ok(None),
    };
    let code = intervals
        .iter()
        .find(|interval| interval.start <= entry && entry < interval.end && (interval.kind == "thumb" || interval.kind == "arm"));
    Ok(code.map(|interval| interval.end - entry))
}

/// Refuse a span that is not contained in ONE audited executable interval.
/// See the TypeScript original's `auditedInterval` doc comment for the full
/// rationale; it is reproduced there, not duplicated here.
fn audited_interval(root: &Path, fn_row: &FunctionRow) -> Result<(), String> {
    let intervals = match audit_intervals(root, &fn_row.overlay)? {
        Some(intervals) => intervals,
        None => return Ok(()), // un-audited overlay: not this check's call to make
    };
    let start = fn_row.entry;
    let end = start + fn_row.span_bytes;
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
        fn_row.id, start, end, detail, suggestion
    ))
}

// ---------------------------------------------------------------------------
// CLI
// ---------------------------------------------------------------------------

const USAGE: &str = "usage: overlay_adopt.ts <overlay:offsetHex> --source FILE [--span BYTES] [--apply] [--where]";

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

fn revert(installed: &Path, assembly: &Path, preexisting: &Option<Vec<u8>>, original_text: &str) -> Result<(), String> {
    match preexisting {
        Some(data) => fs::write(installed, data).map_err(|error| error.to_string())?,
        None => {
            let _ = fs::remove_file(installed);
        }
    }
    fs::write(assembly, original_text).map_err(|error| error.to_string())
}

/// `main()`.
pub fn run(root: &Path, args: &[String]) -> Result<i32, String> {
    if args.len() == 1 && args[0] == "--self-test" {
        self_test()?;
        return Ok(0);
    }

    let options = match options_of(args)? {
        ParseOutcome::Help => {
            println!("{USAGE}");
            return Ok(0);
        }
        ParseOutcome::Options(options) => options,
    };

    let inventory_path: PathBuf = root.join("out/decomp/overlays.json");
    let inventory_text = fs::read_to_string(&inventory_path).map_err(|error| error.to_string())?;
    let inventory = parse_json(&inventory_text)?;
    let functions = inventory
        .get("functions")
        .and_then(Value::as_array)
        .ok_or_else(|| "out/decomp/overlays.json: unexpected shape".to_string())?;

    let found = functions.iter().find(|row| row.get("id").and_then(Value::as_str) == Some(options.id.as_str()));

    // Discovery seeds from control flow inside the stream, so a function whose
    // only callers live in the main image or in an external pointer table is
    // never inventoried. `--span` adopts such a function from its id alone. It
    // weakens nothing: the region boundary, straddling-label and rehearse-and-
    // compare checks below are what actually gate the splice, and they read
    // the assembly, not the inventory.
    let fn_row: FunctionRow = if let Some(row) = found {
        FunctionRow {
            id: options.id.clone(),
            overlay: row.get("overlay").and_then(Value::as_str).unwrap_or("").to_string(),
            entry: row.get("entry").and_then(Value::as_f64).unwrap_or(0.0) as i64,
            offset: row.get("offset").and_then(Value::as_f64).unwrap_or(0.0) as i64,
            span_bytes: row.get("span_bytes").and_then(Value::as_f64).unwrap_or(0.0) as i64,
        }
    } else if let Some(span) = options.span {
        let mut parts = options.id.splitn(2, ':');
        let overlay = parts.next().unwrap_or("").to_string();
        let offset_text = parts.next().unwrap_or("");
        let offset = i64::from_str_radix(offset_text, 16)
            .map_err(|_| format!("unparseable overlay id: {}", options.id))?;
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

    let assembly = root.join("assets/code").join(format!("{}_overlay.s", fn_row.overlay));
    let baseline = assemble_overlay(&OverlaySource::path(&assembly), OVERLAY_BASE)?;
    let original_text = fs::read_to_string(&assembly).map_err(|error| error.to_string())?;
    let lines: Vec<String> = original_text.split('\n').map(|line| line.to_string()).collect();
    let offsets = listing_offsets(&assembly)?;
    let (first, last) = region_lines(&offsets, fn_row.offset, fn_row.span_bytes)?;

    // Adopting a region twice appends a second `AlchemyC_` label in front of
    // the first one's `.space`. The assembler tolerates the duplicate and the
    // ROM still rebuilds byte-identically, so the full build does not catch
    // it; only the inventory's placeholder walk does, one run later. Refuse
    // up front.
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

    // Rehearse in place rather than in a scratch copy. Compiler flags are
    // routed by repository-relative path (`sourceKey` in the compiler tables), so
    // a candidate compiled from a temp directory silently loses every
    // path-keyed flag -- a dry run under /tmp would reject a correct
    // flag-routed match. Both files are restored unless the rebuild is
    // byte-identical and --apply was given.
    let installed = root.join("exact").join(format!("{}_c_{}.c", fn_row.overlay, stem));
    // Only remove the installed C file if this run created it. A rehearsal
    // over a region that already has one (a dry run, or a repeat) must leave
    // the existing source alone -- deleting it orphans the placeholder and
    // the next inventory run fails on the mismatch.
    let preexisting = if installed.exists() {
        Some(fs::read(&installed).map_err(|error| error.to_string())?)
    } else {
        None
    };

    let rebuild: Result<Vec<u8>, String> = (|| {
        fs::copy(&options.source, &installed).map_err(|error| error.to_string())?;
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
            // Runs of consecutive addresses, printed as ROM addresses. An
            // empty list with a non-zero count means the difference is a
            // length change only.
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
        "adopt=applied {} span={} aliases={} c=exact/{}_c_{}.c",
        options.id,
        fn_row.span_bytes,
        aliases.len(),
        fn_row.overlay,
        stem
    );
    Ok(0)
}

#[cfg(test)]
mod tests {
    use super::*;

    fn fixture_lines() -> Vec<String> {
        [
            "\tb .L_02000006",
            "\tmovs r0, #0 @ .L_02000008 is comment-only",
            "Func_02000004:",
            "\tmovs r0, #0",
            ".L_02000006:",
            "\tbx lr",
            ".L_02000008:",
            "\t.2byte 0",
        ]
        .iter()
        .map(|line| line.to_string())
        .collect()
    }

    #[test]
    fn self_test_fixture() {
        self_test().expect("self-test must pass");
    }

    #[test]
    fn internal_aliases_matches_ts_fixture() {
        let lines = fixture_lines();
        let aliases = internal_aliases(&lines, 3, 6, 4, 4).unwrap();
        assert_eq!(aliases, vec![InternalAlias { label: ".L_02000006".to_string(), offset: 2 }]);
    }

    #[test]
    fn placeholder_lines_matches_ts_fixture() {
        let aliases = vec![InternalAlias { label: ".L_02000006".to_string(), offset: 2 }];
        let placeholder = placeholder_lines("02000004", 4, &aliases).join("\n");
        assert_eq!(placeholder, "AlchemyC_02000004:\n\t.space 0x2\n.L_02000006:\n\t.space 0x2");
    }

    #[test]
    fn placeholder_lines_empty_aliases() {
        let placeholder = placeholder_lines("02000004", 4, &[]).join("\n");
        assert_eq!(placeholder, "AlchemyC_02000004:\n\t.space 0x4");
    }

    #[test]
    fn placeholder_lines_alias_at_offset_zero() {
        let aliases = vec![InternalAlias { label: ".L_02000004".to_string(), offset: 0 }];
        let placeholder = placeholder_lines("02000004", 4, &aliases).join("\n");
        // No gap emitted before an alias that starts at the region's own base.
        assert_eq!(placeholder, "AlchemyC_02000004:\n.L_02000004:\n\t.space 0x4");
    }

    #[test]
    fn placeholder_lines_span_with_no_trailing_space() {
        let aliases = vec![InternalAlias { label: ".L_02000006".to_string(), offset: 2 }];
        // span == alias offset: nothing remains after the last alias, so no
        // trailing `.space` directive is emitted.
        let placeholder = placeholder_lines("02000004", 2, &aliases).join("\n");
        assert_eq!(placeholder, "AlchemyC_02000004:\n\t.space 0x2\n.L_02000006:");
    }

    #[test]
    fn listing_row_matches_upper_and_lower_byte_columns() {
        assert_eq!(parse_listing_row("   12 0000a2 D0            b .L_foo"), Some((12, 0x00a2)));
        assert_eq!(parse_listing_row("   12 0000a2 d0            b .L_foo"), Some((12, 0x00a2)));
        assert_eq!(parse_listing_row("not a listing row"), None);
    }

    #[test]
    fn word_boundary_contains_respects_line_boundaries() {
        assert!(word_boundary_contains("b .L_02000006\nbx lr", ".L_02000006"));
        assert!(!word_boundary_contains("b .L_020000060\nbx lr", ".L_02000006"));
        // Comment stripping happens in `strip_comments` before this check runs;
        // word_boundary_contains itself has no comment awareness.
        assert!(word_boundary_contains("@ .L_02000006 is comment-only", ".L_02000006"));
    }
}
