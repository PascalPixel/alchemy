// Code-overlay side of the executable inventory.
//
// WHY: 96 of the 97 audited address spaces are code overlays, and each one has
// to be re-assembled and re-classified from its `.s` container before its
// executable bytes can be counted. This module holds that pipeline: the
// `arm-none-eabi-as -al=` listing, the source-line classification that turns
// listing rows into intervals, the two raw-halfword leaf recovery rules, and
// the C-ownership spans read back out of the same container.
//
// PORT NOTE: `assembleOverlay` is supplied by the native
// `overlay-disasm` crate. This module receives each compiled image
// once, up front, with every overlay that has a C sibling, and owns the
// listing classification here.

use std::collections::{HashMap, HashSet};
use std::path::Path;
use std::process::Command;

use crate::intervals::{
    interval_bytes, merge_classified, union_intervals, Interval, Span, OVERLAY_BASE,
};
use crate::js;
use crate::Namespace;

/// `publishedOffset`'s base shift, from `overlay_published.ts`.
const BASE_SHIFT: i64 = 0x8000;

#[derive(Debug, Clone, Copy)]
pub struct DirectiveSpan {
    pub start: i64,
    pub end: i64,
}

#[derive(Debug, Clone, Copy)]
pub struct PublishedLeafEvidence {
    pointer: i64,
    target: i64,
}

/// `AUDITED_PUBLISHED_RAW_LEAVES`, verbatim.
fn audited_published_raw_leaves(overlay: &str) -> Vec<PublishedLeafEvidence> {
    let rows: &[(i64, i64)] = match overlay {
        "resource_377" => &[(0x0200_002c, 0x0200_0090)],
        "resource_378" => &[(0x0200_002c, 0x0200_0064)],
        "resource_398" => &[(0x0200_0fc4, 0x0200_044c), (0x0200_0fd0, 0x0200_045c)],
        "resource_3a7" => &[(0x0200_21f8, 0x0200_04cc)],
        _ => &[],
    };
    rows.iter().map(|&(pointer, target)| PublishedLeafEvidence { pointer, target }).collect()
}

// ---------------------------------------------------------------------------
// BL and published-pointer decoding (from overlay_call_targets/overlay_published)
// ---------------------------------------------------------------------------

/// `storedDisplacement` + 2. All arithmetic is 32-bit, as in JS.
fn target_offset(high: i32, low: i32) -> Option<i32> {
    if (high & 0xf800) != 0xf000 || (low & 0xf800) != 0xf800 {
        return None;
    }
    let upper = high & 0x07ff;
    let lower = low & 0x07ff;
    let signed = if upper >= 0x0400 { upper - 0x0800 } else { upper };
    Some(((signed << 12) | (lower << 1)).wrapping_add(2))
}

/// `publishedOffset(word, imageLength)`.
fn published_offset(word: u32, image_length: usize) -> Option<i64> {
    if word & 1 == 0 {
        return None;
    }
    let offset = i64::from(word & !1) - (OVERLAY_BASE + BASE_SHIFT);
    if offset < 0 || offset + 2 > image_length as i64 {
        return None;
    }
    Some(offset)
}

fn halfword_at(binary: &[u8], offset: usize) -> i32 {
    i32::from(binary[offset]) | (i32::from(binary[offset + 1]) << 8)
}

// ---------------------------------------------------------------------------
// Raw-halfword leaf recovery
// ---------------------------------------------------------------------------

/// Recover leaf code written as raw halfwords when a BL in audited Thumb
/// resolves to it AND the bounded directive run reaches `bx lr`.
pub fn reached_directive_leaves(
    binary: &[u8],
    callers: &[Interval],
    directives: &[DirectiveSpan],
) -> Vec<Interval> {
    // PORT NOTE: `starts` is a JS `Set`, which iterates in insertion order and
    // therefore fixes the order of the leaves this returns. A HashSet would
    // not, so insertion order is tracked explicitly.
    let mut starts: Vec<i64> = Vec::new();
    let mut seen: HashSet<i64> = HashSet::new();
    for caller in callers {
        if caller.kind != "thumb" {
            continue;
        }
        let from = (caller.start - OVERLAY_BASE as f64).max(0.0) as i64;
        let to = (caller.end - OVERLAY_BASE as f64).min(binary.len() as f64) as i64;
        let mut offset = from;
        while offset + 4 <= to {
            let index = offset as usize;
            let target = target_offset(halfword_at(binary, index), halfword_at(binary, index + 2));
            if let Some(target) = target {
                if target >= 0 && (target as usize) < binary.len() {
                    let start = OVERLAY_BASE + i64::from(target);
                    if seen.insert(start) {
                        starts.push(start);
                    }
                }
            }
            offset += 2;
        }
    }
    directive_leaves_at_starts(binary, &starts, directives, "BL-reached")
}

/// Raw leaves whose Thumb entry address is installed in aligned overlay data.
pub fn published_directive_leaves(
    binary: &[u8],
    directives: &[DirectiveSpan],
    publications: &[PublishedLeafEvidence],
) -> Vec<Interval> {
    let directive_starts: HashSet<i64> = directives.iter().map(|span| span.start).collect();
    let mut starts: Vec<i64> = Vec::new();
    let mut seen: HashSet<i64> = HashSet::new();
    for publication in publications {
        let address = publication.pointer;
        let offset = address - OVERLAY_BASE;
        if offset < 0 || offset + 4 > binary.len() as i64 || offset % 4 != 0 {
            continue;
        }
        if !directive_starts.contains(&address) || !directive_starts.contains(&(address + 2)) {
            continue;
        }
        let index = offset as usize;
        let word = u32::from(binary[index])
            | (u32::from(binary[index + 1]) << 8)
            | (u32::from(binary[index + 2]) << 16)
            | (u32::from(binary[index + 3]) << 24);
        if let Some(target) = published_offset(word, binary.len()) {
            if OVERLAY_BASE + target == publication.target && seen.insert(publication.target) {
                starts.push(publication.target);
            }
        }
    }
    directive_leaves_at_starts(binary, &starts, directives, "published")
}

fn directive_leaves_at_starts(
    binary: &[u8],
    starts: &[i64],
    directives: &[DirectiveSpan],
    evidence_prefix: &str,
) -> Vec<Interval> {
    let by_start: HashMap<i64, DirectiveSpan> =
        directives.iter().map(|span| (span.start, *span)).collect();
    let mut leaves: Vec<Interval> = Vec::new();
    for &start in starts {
        if !by_start.contains_key(&start) {
            continue;
        }
        let mut cursor = start;
        let limit = (OVERLAY_BASE + binary.len() as i64).min(start + 128);
        // Insertion-ordered, like the JS `Set`.
        let mut pools: Vec<i64> = Vec::new();
        let mut pooled: HashSet<i64> = HashSet::new();
        while cursor < limit {
            let Some(span) = by_start.get(&cursor) else { break };
            if span.end != cursor + 2 {
                break;
            }
            let offset = (cursor - OVERLAY_BASE) as usize;
            let halfword = halfword_at(binary, offset);
            if (halfword & 0xf800) == 0x4800 {
                // 32-bit `&` and `<<`, as in JS; every value here is far below
                // 2^31 so wrapping cannot be observed, but the mask is exact.
                let pool = ((cursor + 4) & !3) + i64::from((halfword & 0xff) << 2);
                if pooled.insert(pool) {
                    pools.push(pool);
                }
            }
            cursor = span.end;
            if halfword == 0x4770 {
                leaves.push(Interval::new(
                    start,
                    cursor,
                    "thumb",
                    &format!("{evidence_prefix} bounded raw-halfword leaf"),
                ));
                for pool in &pools {
                    if by_start.contains_key(pool) && by_start.contains_key(&(pool + 2)) {
                        leaves.push(Interval::new(
                            *pool,
                            pool + 4,
                            "literal_pool",
                            &format!(
                                "literal pool referenced by {evidence_prefix} raw-halfword leaf"
                            ),
                        ));
                    }
                }
                break;
            }
        }
    }
    leaves
}

// ---------------------------------------------------------------------------
// Assembler listing
// ---------------------------------------------------------------------------

struct ListingRow {
    address: i64,
    width: f64,
}

pub struct Listing {
    rows: HashMap<i64, ListingRow>,
    decoded_bytes: usize,
    binary: Vec<u8>,
}

fn run(command: &mut Command, cwd: Option<&Path>) -> Result<(), String> {
    if let Some(cwd) = cwd {
        command.current_dir(cwd);
    }
    let output = command.output().map_err(|error| error.to_string())?;
    if !output.status.success() {
        // `stderr.toString().trim()` — `toString()` on a Buffer defaults to
        // utf8, and JS `trim` strips the same set `\s` matches.
        return Err(String::from_utf8_lossy(&output.stderr)
            .trim_matches(js::is_js_space)
            .to_string());
    }
    Ok(())
}

fn temporary_directory() -> Result<std::path::PathBuf, String> {
    use std::sync::atomic::{AtomicUsize, Ordering};
    static COUNTER: AtomicUsize = AtomicUsize::new(0);
    let nanos = std::time::SystemTime::now()
        .duration_since(std::time::UNIX_EPOCH)
        .map_err(|error| error.to_string())?
        .as_nanos();
    let sequence = COUNTER.fetch_add(1, Ordering::Relaxed);
    let path = std::env::temp_dir().join(format!(
        "alchemy-full-c-overlay-{}-{nanos}-{sequence}",
        std::process::id()
    ));
    std::fs::create_dir_all(&path).map_err(|error| error.to_string())?;
    Ok(path)
}

/// `assemblerListing`: assemble, link, objcopy, and map source lines to rows.
///
/// `image` is the compiled overlay from the sidecar when the overlay has exact-C
/// siblings, mirroring the TypeScript's `assembleOverlay(source)` call; `None`
/// means "use the placeholder image", the `overlayCSources(...).length === 0`
/// branch.
pub fn assembler_listing(
    root: &Path,
    source: &str,
    image: Option<&[u8]>,
) -> Result<Listing, String> {
    let work = temporary_directory()?;
    let outcome = assembler_listing_in(root, source, image, &work);
    let _ = std::fs::remove_dir_all(&work);
    outcome
}

fn assembler_listing_in(
    root: &Path,
    source: &str,
    image: Option<&[u8]>,
    work: &Path,
) -> Result<Listing, String> {
    let listing = work.join("overlay.lst");
    let object = work.join("overlay.o");
    let elf = work.join("overlay.elf");
    let binary = work.join("overlay.bin");

    run(
        Command::new("arm-none-eabi-as")
            .args(["-mcpu=arm7tdmi", "-mthumb-interwork"])
            .arg(format!("-al={}", listing.display()))
            .arg("-o")
            .arg(&object)
            .arg(source),
        Some(root),
    )?;
    run(
        Command::new("arm-none-eabi-ld")
            .arg(format!("-Ttext=0x{OVERLAY_BASE:x}"))
            .arg("-o")
            .arg(&elf)
            .arg(&object),
        None,
    )?;
    run(
        Command::new("arm-none-eabi-objcopy")
            .args(["-O", "binary", "-j", ".text"])
            .arg(&elf)
            .arg(&binary),
        None,
    )?;

    let mut rows: HashMap<i64, ListingRow> = HashMap::new();
    let text = read_lossy(&listing)?;
    for line in js::split_lines(&text) {
        let Some(row) = js::listing_row(line) else { continue };
        if rows.contains_key(&row.source_line) {
            continue;
        }
        rows.insert(
            row.source_line,
            ListingRow {
                address: OVERLAY_BASE + i64::from_str_radix(row.address_hex, 16).unwrap_or(0),
                width: row.encoding_digits as f64 / 2.0,
            },
        );
    }

    let placeholder = std::fs::read(&binary).map_err(|error| error.to_string())?;
    let data = match image {
        Some(image) => image.to_vec(),
        None => placeholder.clone(),
    };
    if data.len() != placeholder.len() {
        return Err(format!(
            "{source}: compiled code-overlay length differs from its canonical placeholder image"
        ));
    }
    Ok(Listing { rows, decoded_bytes: data.len(), binary: data })
}

/// `readFileSync(path, "utf8")`, which is lossy on invalid UTF-8.
pub fn read_lossy(path: &Path) -> Result<String, String> {
    let bytes = std::fs::read(path).map_err(|error| error.to_string())?;
    Ok(String::from_utf8_lossy(&bytes).into_owned())
}

// ---------------------------------------------------------------------------
// Overlay inventory
// ---------------------------------------------------------------------------

fn basename(path: &str) -> &str {
    path.rsplit('/').next().unwrap_or(path)
}

/// `overlayCSources(source)` — exact/`<prefix>c_*.c`, sorted.
pub fn overlay_c_sources(root: &Path, source: &str) -> Vec<String> {
    if source.contains('\n') || !Path::new(source).exists() {
        return Vec::new();
    }
    let directory = root.join("exact");
    let prefix = js::replace_overlay_suffix(basename(source), "c_");
    let Ok(entries) = std::fs::read_dir(&directory) else { return Vec::new() };
    let mut names: Vec<String> = entries
        .filter_map(|entry| entry.ok())
        .map(|entry| entry.file_name().to_string_lossy().into_owned())
        .filter(|name| name.starts_with(&prefix) && name.ends_with(".c"))
        .collect();
    // JS `Array.prototype.sort` orders by UTF-16 code unit; these names are
    // ASCII, where that coincides with byte order.
    names.sort();
    names.iter().map(|name| directory.join(name).to_string_lossy().into_owned()).collect()
}

pub fn overlay_inventory(
    root: &Path,
    source: &str,
    image: Option<&[u8]>,
    audited_callers: &[Interval],
) -> Result<Namespace, String> {
    let text = read_lossy(Path::new(source))?;
    let lines = js::split_lines(&text);
    let listing = assembler_listing(root, source, image)?;
    let file = basename(source).to_string();

    let mut intervals: Vec<Interval> = Vec::new();
    let mut directive_rows: Vec<(i64, i64)> = Vec::new();
    let mut halfword_directives: Vec<DirectiveSpan> = Vec::new();
    let mut data_addresses: HashSet<i64> = HashSet::new();
    let mut in_c_placeholder = false;

    let mut branched_labels: HashSet<&str> = HashSet::new();
    for line in &lines {
        if let Some(label) = js::branch_label(line) {
            branched_labels.insert(label);
        }
    }
    let mut branched_targets: HashSet<i64> = HashSet::new();

    for index in 0..lines.len() {
        let line = lines[index];
        let row = listing.rows.get(&(index as i64 + 1));
        if js::alchemy_c_label(line).is_some() {
            in_c_placeholder = true;
            continue;
        }
        if let Some(label) = js::local_label(line) {
            if branched_labels.contains(label) {
                // A label emits no bytes, so its address is the next row that does.
                for ahead in index + 1..lines.len() {
                    if let Some(next) = listing.rows.get(&(ahead as i64 + 1)) {
                        branched_targets.insert(next.address);
                        break;
                    }
                }
            }
        }
        if in_c_placeholder && (js::blank_line(line) || js::local_label(line).is_some()) {
            continue;
        }
        let space = js::space_directive(line);
        if let (Some(size), Some(row), true) = (space, row, in_c_placeholder) {
            intervals.push(Interval {
                start: row.address as f64,
                end: (row.address + size) as f64,
                kind: "thumb".to_string(),
                evidence: format!("{file}:{}:verified-C-placeholder", index + 1),
            });
            continue;
        }
        if !js::blank_line(line) {
            in_c_placeholder = false;
        }
        let Some(row) = row else { continue };

        if let Some(value) = js::directive_value(line) {
            let mut byte = 0.0f64;
            while byte < row.width {
                data_addresses.insert(row.address + byte as i64);
                byte += 1.0;
            }
            directive_rows.push((row.address, value));
            let mut byte = 0.0f64;
            while byte + 2.0 <= row.width {
                halfword_directives.push(DirectiveSpan {
                    start: row.address + byte as i64,
                    end: row.address + byte as i64 + 2,
                });
                byte += 2.0;
            }
            continue;
        }
        if js::halfword_directive(line) && row.width == 2.0 {
            halfword_directives
                .push(DirectiveSpan { start: row.address, end: row.address + 2 });
        }
        if js::data_directive(line) {
            let mut byte = 0.0f64;
            while byte < row.width {
                data_addresses.insert(row.address + byte as i64);
                byte += 1.0;
            }
            continue;
        }
        if js::any_directive(line) {
            continue;
        }
        if js::mnemonic_line(line) {
            intervals.push(Interval {
                start: row.address as f64,
                end: row.address as f64 + row.width,
                kind: "thumb".to_string(),
                evidence: format!("{file}:{}:canonical-instruction", index + 1),
            });
            if let Some(offset) = js::pc_relative_literal(line) {
                let address = ((row.address + 4) & !3) + offset;
                intervals.push(Interval::new(
                    address,
                    address + 4,
                    "literal_pool",
                    &format!("{file}:{}:pc-relative-literal", index + 1),
                ));
            }
        }
    }

    // `new Map(...)` keeps the LAST value for a repeated address.
    let directives: HashMap<i64, i64> = directive_rows.iter().copied().collect();
    for &(address, value) in &directive_rows {
        let code = directives.get(&(address + 4)).copied();
        // `value >>> 24` is ToUint32 then a logical shift.
        let bank = ((value.rem_euclid(1 << 32)) as u32) >> 24;
        if value & 1 != 0
            && matches!(bank, 0x02 | 0x08 | 0x09)
            && code == Some(0x4720_4c00)
        {
            intervals.push(Interval::new(
                address,
                address + 8,
                "veneer",
                &format!("{file}:fixed-ldr-bx-veneer"),
            ));
        }
    }

    let overlay = js::strip_overlay_suffix(&file);
    let mut candidates: Vec<Interval> = Vec::new();
    {
        let mut callers = intervals.clone();
        callers.extend_from_slice(audited_callers);
        candidates.extend(reached_directive_leaves(
            &listing.binary,
            &callers,
            &halfword_directives,
        ));
    }
    candidates.extend(published_directive_leaves(
        &listing.binary,
        &halfword_directives,
        &audited_published_raw_leaves(&overlay),
    ));
    candidates.sort_by(|left, right| {
        left.start
            .partial_cmp(&right.start)
            .unwrap()
            .then(left.end.partial_cmp(&right.end).unwrap())
    });
    let mut raw_leaves: Vec<Interval> = Vec::new();
    for leaf in candidates {
        if intervals
            .iter()
            .any(|interval| interval.start < leaf.end && leaf.start < interval.end)
        {
            continue;
        }
        if raw_leaves
            .iter()
            .any(|accepted| accepted.start < leaf.end && leaf.start < accepted.end)
        {
            continue;
        }
        raw_leaves.push(leaf);
    }
    intervals.extend(raw_leaves);

    let veneers: Vec<Interval> =
        intervals.iter().filter(|interval| interval.kind == "veneer").cloned().collect();
    let classified: Vec<Interval> = intervals
        .iter()
        .filter(|interval| {
            if interval.kind != "literal_pool" {
                return true;
            }
            let covered = {
                let mut byte = 0.0f64;
                let mut all = true;
                while byte < interval.end - interval.start {
                    if !data_addresses.contains(&((interval.start + byte) as i64)) {
                        all = false;
                        break;
                    }
                    byte += 1.0;
                }
                all
            };
            covered
                && !veneers.iter().any(|veneer| {
                    veneer.start <= interval.start && interval.end <= veneer.end
                })
        })
        .cloned()
        .collect();

    let mut merged = merge_classified(&classified)?;
    let union = union_intervals(&merged)?;
    let mut alignment: Vec<Interval> = Vec::new();
    for index in 1..union.len() {
        let previous: Span = union[index - 1];
        let current: Span = union[index];
        if current.start - previous.end != 2.0 {
            continue;
        }
        let offset = previous.end - OVERLAY_BASE as f64;
        if branched_targets.contains(&(previous.end as i64)) {
            alignment.push(Interval {
                start: previous.end,
                end: current.start,
                kind: "thumb".to_string(),
                evidence: format!("{file}:branch-target-halfword-between-executable-spans"),
            });
            continue;
        }
        if offset >= 0.0 {
            // PORT NOTE: node's `Buffer#readUInt16LE` THROWS a RangeError when
            // the read runs past the end, so an out-of-range span is a hard
            // failure in the TypeScript too. The message text differs.
            let index = offset as usize;
            if index + 2 > listing.binary.len() {
                return Err(format!(
                    "{source}: alignment probe at 0x{:x} is outside the decoded image \
                     (node: Attempt to access memory outside buffer bounds)",
                    previous.end as i64
                ));
            }
            if halfword_at(&listing.binary, index) == 0 {
                alignment.push(Interval {
                    start: previous.end,
                    end: current.start,
                    kind: "executable_alignment".to_string(),
                    evidence: format!("{file}:two-byte-zero-between-executable-spans"),
                });
            }
        }
    }
    let mut combined = merged.clone();
    combined.extend(alignment);
    merged = merge_classified(&combined)?;
    let executable_bytes = interval_bytes(&merged)?;
    if executable_bytes > listing.decoded_bytes as f64 {
        return Err(format!("{source}: executable bytes exceed decoded size"));
    }
    Ok(Namespace {
        id: overlay,
        decoded_bytes: Some(listing.decoded_bytes as f64),
        executable_bytes,
        excluded_bytes: Some(listing.decoded_bytes as f64 - executable_bytes),
        audit: "complete".to_string(),
        intervals: merged,
        evidence: vec![
            format!("assets/code/{file}"),
            "assembler listing mapped to canonical instruction/directive source lines".to_string(),
            "PC-relative literal targets and fixed ldr/bx veneers".to_string(),
            "byte-identical decoded code-overlay round trip".to_string(),
        ],
    })
}

/// `overlaySources(target)` — only gs1-en has code overlays.
pub fn overlay_sources(root: &Path, target: &str) -> Result<Vec<String>, String> {
    if target != "gs1-en" {
        return Ok(Vec::new());
    }
    let directory = root.join("assets").join("code");
    let entries = std::fs::read_dir(&directory).map_err(|error| error.to_string())?;
    let mut names: Vec<String> = entries
        .filter_map(|entry| entry.ok())
        .map(|entry| entry.file_name().to_string_lossy().into_owned())
        .filter(|name| js::overlay_source_name(name))
        .collect();
    names.sort();
    Ok(names.iter().map(|name| directory.join(name).to_string_lossy().into_owned()).collect())
}

/// `overlayCSpans(source)`.
pub fn overlay_c_spans(root: &Path, source: &str) -> Result<Vec<crate::OwnedSpan>, String> {
    let text = read_lossy(Path::new(source))?;
    let file = basename(source).to_string();
    let namespace = js::strip_overlay_suffix(&file);
    let mut spans: Vec<crate::OwnedSpan> = Vec::new();
    let mut owner = String::new();
    let mut cursor: i64 = 0;
    let mut in_placeholder = false;
    for line in js::split_lines(&text) {
        if let Some(address) = js::alchemy_c_label(line) {
            owner = format!("AlchemyC_{address}");
            cursor = i64::from_str_radix(address, 16).map_err(|error| error.to_string())?;
            in_placeholder = true;
            continue;
        }
        if in_placeholder && (js::blank_line(line) || js::local_label(line).is_some()) {
            continue;
        }
        if let (true, Some(size)) = (in_placeholder, js::space_directive(line)) {
            // `owner.slice(-8)` — the last 8 UTF-16 units, which for this
            // ASCII label is the hex address the label carried.
            let suffix: String = owner.chars().rev().take(8).collect::<Vec<_>>()
                .into_iter()
                .rev()
                .collect();
            let c_name = js::replace_overlay_suffix(&file, &format!("c_{suffix}.c"));
            let c_path = root.join("exact").join(&c_name);
            if !c_path.exists() || !js::canonical_c_source(&read_lossy(&c_path)?) {
                cursor += size;
                continue;
            }
            spans.push(crate::OwnedSpan {
                namespace: namespace.clone(),
                start: cursor as f64,
                end: (cursor + size) as f64,
                owner: format!("{file}:{owner}"),
            });
            cursor += size;
            continue;
        }
        if !js::blank_line(line) {
            in_placeholder = false;
        }
    }
    Ok(spans)
}

#[cfg(test)]
mod tests {
    use super::*;

    fn span(start: i64, end: i64) -> DirectiveSpan {
        DirectiveSpan { start, end }
    }

    fn leaf_image() -> Vec<u8> {
        let mut image = vec![0u8; 0x20];
        // stored displacement 0xe -> target +0x10
        image[0..4].copy_from_slice(&[0x00, 0xf0, 0x07, 0xf8]);
        // movs r0,#1; bx lr
        image[0x10..0x14].copy_from_slice(&[0x01, 0x20, 0x70, 0x47]);
        image
    }

    fn raw_leaf() -> Vec<DirectiveSpan> {
        vec![
            span(OVERLAY_BASE + 0x10, OVERLAY_BASE + 0x12),
            span(OVERLAY_BASE + 0x12, OVERLAY_BASE + 0x14),
        ]
    }

    fn caller() -> Vec<Interval> {
        vec![Interval::new(OVERLAY_BASE, OVERLAY_BASE + 4, "thumb", "test")]
    }

    #[test]
    fn a_bl_reached_return_shaped_run_is_thumb() {
        let reached = reached_directive_leaves(&leaf_image(), &caller(), &raw_leaf());
        assert_eq!(reached.len(), 1);
        assert_eq!(reached[0].start, (OVERLAY_BASE + 0x10) as f64);
        assert_eq!(reached[0].end, (OVERLAY_BASE + 0x14) as f64);
    }

    #[test]
    fn neither_half_of_the_evidence_suffices_alone() {
        let mut data_only = leaf_image();
        data_only[0..4].fill(0);
        assert!(reached_directive_leaves(&data_only, &caller(), &raw_leaf()).is_empty());

        let mut no_return = leaf_image();
        no_return[0x12] = 0;
        no_return[0x13] = 0;
        assert!(reached_directive_leaves(&no_return, &caller(), &raw_leaf()).is_empty());

        assert!(reached_directive_leaves(&leaf_image(), &[], &raw_leaf()).is_empty());

        let mut pool_shape = leaf_image();
        pool_shape[0..4].fill(0);
        pool_shape[4..8].copy_from_slice(&[0x00, 0xf0, 0x07, 0xf8]);
        assert!(reached_directive_leaves(&pool_shape, &caller(), &raw_leaf()).is_empty());
    }

    #[test]
    fn a_reached_getter_carries_its_literal_pool() {
        let mut getter_image = leaf_image();
        getter_image[0x10..0x18]
            .copy_from_slice(&[0x00, 0x48, 0x70, 0x47, 0x88, 0x98, 0x00, 0x02]);
        let raw_getter: Vec<DirectiveSpan> = (0..4)
            .map(|index| span(OVERLAY_BASE + 0x10 + index * 2, OVERLAY_BASE + 0x12 + index * 2))
            .collect();
        let getter = reached_directive_leaves(&getter_image, &caller(), &raw_getter);
        assert_eq!(getter.len(), 2);
        assert_eq!(getter[0].kind, "thumb");
        assert_eq!(getter[0].end, (OVERLAY_BASE + 0x14) as f64);
        assert_eq!(getter[1].kind, "literal_pool");
        assert_eq!(getter[1].start, (OVERLAY_BASE + 0x14) as f64);
        assert_eq!(getter[1].end, (OVERLAY_BASE + 0x18) as f64);
    }

    fn published_fixture() -> (Vec<u8>, Vec<DirectiveSpan>, Vec<PublishedLeafEvidence>) {
        let mut image = leaf_image();
        image[0..4].fill(0);
        // Thumb pointer to overlay offset 0x10
        image[0..4].copy_from_slice(&[0x11, 0x80, 0x00, 0x02]);
        let mut directives = vec![
            span(OVERLAY_BASE, OVERLAY_BASE + 2),
            span(OVERLAY_BASE + 2, OVERLAY_BASE + 4),
        ];
        directives.extend(raw_leaf());
        let publications =
            vec![PublishedLeafEvidence { pointer: OVERLAY_BASE, target: OVERLAY_BASE + 0x10 }];
        (image, directives, publications)
    }

    #[test]
    fn a_published_pointer_recovers_its_leaf() {
        let (image, directives, publications) = published_fixture();
        let published = published_directive_leaves(&image, &directives, &publications);
        assert_eq!(published.len(), 1);
        assert_eq!(published[0].start, (OVERLAY_BASE + 0x10) as f64);
        assert_eq!(published[0].end, (OVERLAY_BASE + 0x14) as f64);
    }

    #[test]
    fn publication_evidence_has_to_be_exact() {
        let (image, directives, publications) = published_fixture();

        let mut even = image.clone();
        even[0] &= 0xfe;
        assert!(published_directive_leaves(&even, &directives, &publications).is_empty());

        let mut out_of_range = image.clone();
        out_of_range[0..4].copy_from_slice(&[0x01, 0x81, 0x00, 0x02]);
        assert!(published_directive_leaves(&out_of_range, &directives, &publications).is_empty());

        let mut without_return = image.clone();
        without_return[0x10..0x14].fill(0);
        assert!(published_directive_leaves(&without_return, &directives, &publications).is_empty());

        // The pointer word must itself be directive-classified.
        assert!(published_directive_leaves(&image, &raw_leaf(), &publications).is_empty());

        let stale =
            vec![PublishedLeafEvidence { pointer: OVERLAY_BASE, target: OVERLAY_BASE + 0x14 }];
        assert!(published_directive_leaves(&image, &directives, &stale).is_empty());
    }

    #[test]
    fn the_audited_publication_table_is_verbatim() {
        assert_eq!(audited_published_raw_leaves("resource_398").len(), 2);
        assert_eq!(audited_published_raw_leaves("resource_3a7")[0].pointer, 0x0200_21f8);
        assert!(audited_published_raw_leaves("resource_371").is_empty());
    }
}
