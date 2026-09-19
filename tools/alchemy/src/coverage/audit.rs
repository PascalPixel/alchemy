//! Executable-byte audit derived from canonical ROM images.
pub(crate) mod index;

use crate::compiler::canonical_json::canonical_json;
use crate::overlay::assembly::{
    adjacent_prologue_spans, build_overlay_source, compiler_idiom_spans, executable_spans,
    main_executable_spans, overlay_flow_spans, trusted_overlay_spans, ExecutableSpan, OVERLAY_BASE,
    ROM_BASE,
};
use crate::overlay::listing_rows;
use crate::overlay::rom::CanonicalRom;
use crate::targets::{decomp_target, target_for, DecompTarget, DecompTargetId, TARGET_IDS};
use serde::Deserialize;
use serde_json::{json, Value};
use std::path::{Path, PathBuf};
use tempfile::NamedTempFile;

const USAGE: &str = "usage: alchemy coverage audit (--target TARGET | --all) [--output out/...json] [--calibrate] [--data]";

#[derive(Default)]
struct Options {
    target: Option<String>,
    all: bool,
    output: Option<PathBuf>,
    calibrate: bool,
    help: bool,
    data: bool,
}

#[derive(Deserialize)]
struct ExpectedReport {
    main: ExpectedMain,
    overlays: Vec<ExpectedOverlay>,
}

#[derive(Deserialize)]
struct ExpectedMain {
    executable_bytes: u64,
    intervals: Vec<ExpectedSpan>,
}

#[derive(Deserialize)]
struct ExpectedSpan {
    start: i64,
    end: i64,
    kind: String,
}

#[derive(Deserialize)]
struct ExpectedOverlay {
    id: String,
    executable_bytes: u64,
    intervals: Vec<ExpectedSpan>,
}

fn parse(arguments: &[String]) -> Result<Options, String> {
    let mut options = Options::default();
    let mut index = 0;
    while index < arguments.len() {
        match arguments[index].as_str() {
            "--target" => {
                index += 1;
                options.target = arguments.get(index).cloned();
            }
            "--output" => {
                index += 1;
                options.output = arguments.get(index).map(PathBuf::from);
            }
            "--all" => options.all = true,
            "--calibrate" => options.calibrate = true,
            "--data" => options.data = true,
            "-h" | "--help" => options.help = true,
            argument => return Err(format!("unrecognized argument {argument:?}\n{USAGE}")),
        }
        index += 1;
    }
    if !options.help && options.target.is_some() == options.all {
        return Err(format!("choose exactly one of --target or --all\n{USAGE}"));
    }
    Ok(options)
}

fn union_bytes(spans: &[ExecutableSpan]) -> u64 {
    let mut ranges = spans
        .iter()
        .map(|span| (span.start, span.end))
        .collect::<Vec<_>>();
    ranges.sort_unstable();
    let mut merged: Vec<(i64, i64)> = Vec::new();
    for range in ranges {
        if let Some(last) = merged.last_mut() {
            if range.0 <= last.1 {
                last.1 = last.1.max(range.1);
                continue;
            }
        }
        merged.push(range);
    }
    merged
        .into_iter()
        .map(|(start, end)| (end - start) as u64)
        .sum()
}

fn normalized_ranges<I: IntoIterator<Item = (i64, i64)>>(ranges: I) -> Vec<(i64, i64)> {
    let mut ranges = ranges
        .into_iter()
        .filter(|(start, end)| end > start)
        .collect::<Vec<_>>();
    ranges.sort_unstable();
    let mut merged: Vec<(i64, i64)> = Vec::new();
    for range in ranges {
        if let Some(last) = merged.last_mut() {
            if range.0 <= last.1 {
                last.1 = last.1.max(range.1);
                continue;
            }
        }
        merged.push(range);
    }
    merged
}

fn intersection_bytes(left: &[(i64, i64)], right: &[(i64, i64)]) -> u64 {
    let left = normalized_ranges(left.iter().copied());
    let right = normalized_ranges(right.iter().copied());
    let mut total = 0u64;
    let mut a = 0;
    let mut b = 0;
    while a < left.len() && b < right.len() {
        let start = left[a].0.max(right[b].0);
        let end = left[a].1.min(right[b].1);
        if end > start {
            total += (end - start) as u64;
        }
        if left[a].1 < right[b].1 {
            a += 1;
        } else {
            b += 1;
        }
    }
    total
}

fn difference_ranges(left: &[(i64, i64)], right: &[(i64, i64)]) -> Vec<(i64, i64)> {
    let right = normalized_ranges(right.iter().copied());
    let mut out = Vec::new();
    for (start, end) in normalized_ranges(left.iter().copied()) {
        let mut cursor = start;
        for &(cut_start, cut_end) in &right {
            if cut_end <= cursor {
                continue;
            }
            if cut_start >= end {
                break;
            }
            if cut_start > cursor {
                out.push((cursor, cut_start.min(end)));
            }
            cursor = cursor.max(cut_end);
        }
        if cursor < end {
            out.push((cursor, end));
        }
    }
    out
}

fn subtract_spans(spans: Vec<ExecutableSpan>, cuts: &[(i64, i64)]) -> Vec<ExecutableSpan> {
    let cuts = normalized_ranges(cuts.iter().copied());
    let mut out = Vec::new();
    for span in spans {
        for (start, end) in difference_ranges(&[(span.start, span.end)], &cuts) {
            out.push(ExecutableSpan {
                start,
                end,
                kind: span.kind,
            });
        }
    }
    out
}

fn asset_cuts(root: &Path, target: DecompTarget) -> Result<Vec<(i64, i64)>, String> {
    let candidates = [
        root.join(target.output_dir)
            .join("full/assets/manifest.json"),
        root.join(target.output_dir).join("assets/manifest.json"),
    ];
    let path = candidates
        .iter()
        .find(|path| path.exists())
        .ok_or_else(|| format!("{} has no built asset manifest", target.id))?;
    let manifest: Value = serde_json::from_slice(
        &std::fs::read(path).map_err(|error| format!("{}: {error}", path.display()))?,
    )
    .map_err(|error| format!("{}: {error}", path.display()))?;
    Ok(manifest["regions"]
        .as_array()
        .into_iter()
        .flatten()
        .filter_map(|row| {
            let start = row["address"].as_i64()?;
            let size = row["size"].as_i64()?;
            (size > 0).then_some((start, start + size))
        })
        .collect())
}

fn source_spans(source: &Path, image: &[u8], overlay: &str) -> Result<Vec<ExecutableSpan>, String> {
    let text = std::fs::read_to_string(source)
        .map_err(|error| format!("{}: {error}", source.display()))?;
    let lines = text.lines().collect::<Vec<_>>();
    let rows = listing_rows(source)?
        .into_iter()
        .map(|row| (row.line, row))
        .collect::<std::collections::BTreeMap<_, _>>();
    let mut spans = Vec::new();
    let mut directives = std::collections::BTreeSet::new();
    let mut data_bytes = std::collections::BTreeSet::new();
    let mut literal_words = Vec::new();
    let mut branch_labels = std::collections::BTreeSet::new();
    let mut branch_targets = std::collections::BTreeSet::new();
    for line in &lines {
        let trimmed = line.split('@').next().unwrap_or("").trim();
        let Some((mnemonic, operand)) = trimmed.split_once(char::is_whitespace) else {
            continue;
        };
        if matches!(
            mnemonic.trim_end_matches(".n").trim_end_matches(".w"),
            "b" | "beq"
                | "bne"
                | "bcs"
                | "bcc"
                | "bmi"
                | "bpl"
                | "bvs"
                | "bvc"
                | "bhi"
                | "bls"
                | "bge"
                | "blt"
                | "bgt"
                | "ble"
                | "bhs"
                | "blo"
        ) {
            let label = operand.trim().trim_end_matches(',');
            if label.starts_with('.') {
                branch_labels.insert(label.to_owned());
            }
        }
    }
    let mut in_c = false;
    for (index, line) in lines.iter().enumerate() {
        let trimmed = line.split('@').next().unwrap_or("").trim();
        if trimmed.starts_with("AlchemyC_") && trimmed.ends_with(':') {
            in_c = true;
            continue;
        }
        let row = rows.get(&((index + 1) as i64));
        if let Some(label) = trimmed.strip_suffix(':') {
            if branch_labels.contains(label) {
                if let Some(next) =
                    ((index + 2)..=lines.len()).find_map(|line| rows.get(&(line as i64)))
                {
                    branch_targets.insert(OVERLAY_BASE + next.offset);
                }
            }
        }
        if in_c && trimmed.starts_with(".space ") {
            let row = row.ok_or_else(|| {
                format!(
                    "{}:{}: placeholder emitted no bytes",
                    source.display(),
                    index + 1
                )
            })?;
            let value = trimmed.trim_start_matches(".space ").trim();
            let width = value
                .strip_prefix("0x")
                .and_then(|hex| i64::from_str_radix(hex, 16).ok())
                .or_else(|| value.parse::<i64>().ok())
                .ok_or_else(|| format!("{}: invalid C placeholder {trimmed}", source.display()))?;
            spans.push(ExecutableSpan {
                start: OVERLAY_BASE + row.offset,
                end: OVERLAY_BASE + row.offset + width,
                kind: "thumb",
            });
            continue;
        }
        if !trimmed.is_empty() && !trimmed.ends_with(':') {
            in_c = false;
        }
        if trimmed.is_empty() || trimmed.starts_with('.') || trimmed.ends_with(':') {
            if matches!(trimmed.split_whitespace().next(), Some(".2byte" | ".4byte")) {
                if let Some(row) = row {
                    for at in row.offset..row.offset + row.width {
                        data_bytes.insert(OVERLAY_BASE + at);
                    }
                    for at in (row.offset..row.offset + row.width).step_by(2) {
                        directives.insert(OVERLAY_BASE + at);
                    }
                }
            }
            continue;
        }
        let Some(row) = row else { continue };
        spans.push(ExecutableSpan {
            start: OVERLAY_BASE + row.offset,
            end: OVERLAY_BASE + row.offset + row.width,
            kind: "thumb",
        });
        if let Some(pc) = trimmed.find("[pc, #") {
            let tail = &trimmed[pc + 6..];
            if let Some(end) = tail.find(']') {
                if let Ok(offset) = tail[..end].parse::<i64>() {
                    let start = OVERLAY_BASE + row.offset;
                    let word = ((start + 4) & !3) + offset;
                    literal_words.push(word);
                }
            }
        }
    }
    for word in literal_words {
        if (word..word + 4).all(|at| data_bytes.contains(&at)) {
            spans.push(ExecutableSpan {
                start: word,
                end: word + 4,
                kind: "literal_pool",
            });
        }
    }
    for offset in (0..image.len().saturating_sub(7)).step_by(4) {
        let address = OVERLAY_BASE + offset as i64;
        if image[offset..offset + 4] == [0x00, 0x4c, 0x20, 0x47] {
            let target = u32::from_le_bytes(image[offset + 4..offset + 8].try_into().unwrap());
            // bx selects ARM or Thumb from bit zero. Even IWRAM targets are
            // real ARM import veneers, not data to omit from the image.
            if matches!(target >> 24, 0x02 | 0x03 | 0x08 | 0x09) {
                spans.push(ExecutableSpan {
                    start: address,
                    end: address + 8,
                    kind: "veneer",
                });
            }
        }
    }
    let half = |address: i64| {
        let offset = (address - OVERLAY_BASE) as usize;
        image
            .get(offset..offset + 2)
            .map(|bytes| u16::from_le_bytes(bytes.try_into().unwrap()))
    };
    let callers = spans.clone();
    let mut leaves = std::collections::BTreeSet::new();
    for caller in callers.iter().filter(|span| span.kind == "thumb") {
        let mut at = caller.start;
        while at + 4 <= caller.end {
            let Some(high) = half(at).map(i32::from) else {
                break;
            };
            let Some(low) = half(at + 2).map(i32::from) else {
                break;
            };
            if high & 0xf800 == 0xf000 && low & 0xf800 == 0xf800 {
                let upper = high & 0x07ff;
                let signed = if upper >= 0x0400 {
                    upper - 0x0800
                } else {
                    upper
                };
                // The linked overlay encodes the destination as a displacement
                // from its fixed load base; the historical audit's relocation
                // decoder normalizes the stored value with the Thumb +2 bias.
                let displacement = (signed << 12) | ((low & 0x07ff) << 1);
                let target = OVERLAY_BASE + i64::from(displacement) + 2;
                if directives.contains(&target) {
                    leaves.insert(target);
                }
            }
            at += 2;
        }
    }
    for start in leaves {
        let mut at = start;
        let mut pools = Vec::new();
        while at < start + 128 && directives.contains(&at) {
            let Some(instruction) = half(at) else {
                break;
            };
            if instruction & 0xf800 == 0x4800 {
                pools.push(((at + 4) & !3) + i64::from((instruction & 0xff) << 2));
            }
            at += 2;
            if instruction == 0x4770 {
                spans.push(ExecutableSpan {
                    start,
                    end: at,
                    kind: "thumb",
                });
                for pool in pools {
                    if directives.contains(&pool) && directives.contains(&(pool + 2)) {
                        spans.push(ExecutableSpan {
                            start: pool,
                            end: pool + 4,
                            kind: "literal_pool",
                        });
                    }
                }
                break;
            }
        }
    }
    let published: &[(i64, i64)] = match overlay {
        "resource_377" => &[(0x0200_002c, 0x0200_0090)],
        "resource_378" => &[(0x0200_002c, 0x0200_0064)],
        "resource_398" => &[(0x0200_0fc4, 0x0200_044c), (0x0200_0fd0, 0x0200_045c)],
        "resource_3a7" => &[(0x0200_21f8, 0x0200_04cc)],
        _ => &[],
    };
    for &(pointer, start) in published {
        let pointer_offset = (pointer - OVERLAY_BASE) as usize;
        if pointer_offset + 4 > image.len()
            || !directives.contains(&pointer)
            || !directives.contains(&(pointer + 2))
        {
            continue;
        }
        let word = u32::from_le_bytes(
            image[pointer_offset..pointer_offset + 4]
                .try_into()
                .unwrap(),
        );
        let decoded = i64::from(word & !1) - (OVERLAY_BASE + 0x8000);
        if word & 1 == 0 || OVERLAY_BASE + decoded != start {
            continue;
        }
        let mut at = start;
        while at < start + 128 && directives.contains(&at) {
            let Some(instruction) = half(at) else {
                break;
            };
            at += 2;
            if instruction == 0x4770 {
                spans.push(ExecutableSpan {
                    start,
                    end: at,
                    kind: "thumb",
                });
                break;
            }
        }
    }
    let mut ranges = spans
        .iter()
        .map(|span| (span.start, span.end))
        .collect::<Vec<_>>();
    ranges.sort_unstable();
    let mut union: Vec<(i64, i64)> = Vec::new();
    for range in ranges {
        if let Some(last) = union.last_mut() {
            if range.0 <= last.1 {
                last.1 = last.1.max(range.1);
                continue;
            }
        }
        union.push(range);
    }
    for pair in union.windows(2) {
        if pair[1].0 - pair[0].1 == 2
            && (half(pair[0].1) == Some(0) || branch_targets.contains(&pair[0].1))
        {
            spans.push(ExecutableSpan {
                start: pair[0].1,
                end: pair[1].0,
                kind: "executable_alignment",
            });
        }
    }
    Ok(spans)
}

fn report(root: &Path, target: DecompTarget) -> Result<Value, String> {
    let rom = CanonicalRom::load_target(root, target)?;
    let resources = rom.overlay_resources(target.overlay_entry_veneers);
    let code_resources = resources
        .iter()
        .copied()
        .collect::<std::collections::BTreeSet<_>>();
    let resource_rows = (0..rom.resource_count())
        .map(|resource| {
            let pointer = rom.resource_pointer(resource)?;
            let stream = rom.stream(resource).ok();
            let role = if code_resources.contains(&resource) {
                "code-overlay"
            } else if stream.is_some() {
                "data-resource"
            } else {
                "raw-pointer"
            };
            let mut row = json!({
                "id": format!("resource_{resource:03x}"),
                "rom_start": ROM_BASE + pointer as i64,
                "role": role,
            });
            if let Some(stream) = stream {
                let encoded = stream.encoded()?;
                let end = stream.start + encoded.len();
                if rom.bytes().get(stream.start..end) != Some(encoded.as_slice()) {
                    return Err(format!(
                        "resource_{resource:03x} does not reproduce its physical encoding"
                    ));
                }
                row["rom_start"] = json!(ROM_BASE + stream.start as i64);
                row["rom_end"] = json!(ROM_BASE + end as i64);
                row["encoded_bytes"] = json!(end - stream.start);
            }
            Ok(row)
        })
        .collect::<Result<Vec<_>, String>>()?;
    let runtime = if target.id.as_str() == "tbs-en" {
        Some(crate::compiler::runtime::Registry::load(root)?)
    } else {
        None
    };
    let mut overlays = Vec::with_capacity(resources.len());
    let mut decoded_total = 0u64;
    let mut encoded_total = 0u64;
    let mut executable_total = 0u64;
    for resource in resources {
        let stream = rom.stream(resource)?;
        let id = format!("resource_{resource:03x}");
        let retained = root.join(target.overlay_assembly(&id));
        let temporary;
        let (source, source_evidence) = if retained.exists() {
            (retained.as_path(), "retained-source")
        } else {
            temporary = NamedTempFile::new().map_err(|error| error.to_string())?;
            std::fs::write(
                temporary.path(),
                build_overlay_source(&stream.decoded, OVERLAY_BASE)?,
            )
            .map_err(|error| error.to_string())?;
            (temporary.path(), "decoder-generated-source")
        };
        let mut spans = source_spans(source, &stream.decoded, &id)?;
        let prologues = adjacent_prologue_spans(&stream.decoded, OVERLAY_BASE, &spans);
        spans.extend(prologues);
        spans.extend(compiler_idiom_spans(&stream.decoded, OVERLAY_BASE));
        if let Some(runtime) = &runtime {
            for link in runtime.links.iter().filter(|link| link.image == id) {
                let linked = crate::compiler::runtime::build(root, link)?;
                spans.push(ExecutableSpan {
                    start: i64::from(link.text),
                    end: i64::from(link.text) + linked.text.len() as i64,
                    kind: "compiler_runtime",
                });
            }
        }
        let generated = build_overlay_source(&stream.decoded, OVERLAY_BASE)?;
        let generated_file = NamedTempFile::new().map_err(|error| error.to_string())?;
        std::fs::write(generated_file.path(), generated).map_err(|error| error.to_string())?;
        let mut generated_spans = source_spans(generated_file.path(), &stream.decoded, &id)?;
        let prologues = adjacent_prologue_spans(&stream.decoded, OVERLAY_BASE, &generated_spans);
        generated_spans.extend(prologues);
        generated_spans.extend(compiler_idiom_spans(&stream.decoded, OVERLAY_BASE));
        let generated_executable = union_bytes(&generated_spans);
        let trusted_spans =
            trusted_overlay_spans(&stream.decoded, OVERLAY_BASE, target.overlay_entry_veneers)?;
        let combined_executable = union_bytes(
            &generated_spans
                .iter()
                .chain(&trusted_spans)
                .cloned()
                .collect::<Vec<_>>(),
        );
        // A maintained raw listing may still spell a real routine as numeric
        // directives. Conversely, it can contain exact C the prologue sweep
        // cannot discover. Follow entry/call/switch evidence independently of
        // those spellings, and keep every method's diagnostic ranges below.
        let flow = overlay_flow_spans(
            &stream.decoded,
            OVERLAY_BASE,
            target.overlay_entry_veneers,
            true,
        )?;
        spans.extend(generated_spans.iter().cloned());
        spans.extend(flow);
        let executable = union_bytes(&spans);
        let decoded = stream.decoded.len() as u64;
        let encoded = (stream.end - stream.start) as u64;
        decoded_total += decoded;
        encoded_total += encoded;
        executable_total += executable;
        overlays.push(json!({
            "id": id,
            "decoded_bytes": decoded,
            "rom_start": ROM_BASE + stream.start as i64,
            "rom_end": ROM_BASE + stream.end as i64,
            "encoded_bytes": encoded,
            "executable_bytes": executable,
            "generated_executable_bytes": generated_executable,
            "combined_executable_bytes": combined_executable,
            "excluded_bytes": decoded - executable,
            "source_evidence": source_evidence,
            "intervals": spans.into_iter().map(|span| json!({
                "start": span.start,
                "end": span.end,
                "kind": span.kind,
            })).collect::<Vec<_>>(),
            "generated_intervals": generated_spans.into_iter().map(|span| json!({
                "start": span.start,
                "end": span.end,
                "kind": span.kind,
            })).collect::<Vec<_>>(),
            "trusted_intervals": trusted_spans.into_iter().map(|span| json!({
                "start": span.start,
                "end": span.end,
                "kind": span.kind,
            })).collect::<Vec<_>>(),
        }));
    }
    // This is deliberately a candidate, not a completion denominator.  The
    // upper bound is the last reviewed main-image owner; the decoder then
    // inventories code reached from Thumb prologues and in-image veneers.
    // Calibration against TBS below measures what this method still misses
    // before TLA is ever allowed to publish a complete audit.
    let metric_path = root.join(format!("{}/metrics/executable.json", target.game_dir()));
    let metric: Value = serde_json::from_slice(
        &std::fs::read(&metric_path)
            .map_err(|error| format!("{}: {error}", metric_path.display()))?,
    )
    .map_err(|error| format!("{}: {error}", metric_path.display()))?;
    let main_end = metric["main"]["intervals"]
        .as_array()
        .into_iter()
        .flatten()
        .filter_map(|row| row["end"].as_i64())
        .max()
        .ok_or_else(|| format!("{} has no main-image audit boundary", metric_path.display()))?;
    let main_start = ROM_BASE + 0xc0;
    let main_image = &rom.bytes()[(main_start - ROM_BASE) as usize..(main_end - ROM_BASE) as usize];
    let cuts = asset_cuts(root, target)?;
    let asset_complement = difference_ranges(&[(main_start, main_end)], &cuts)
        .into_iter()
        .map(|(start, end)| ExecutableSpan {
            start,
            end,
            kind: "asset_complement",
        })
        .collect::<Vec<_>>();
    let asset_complement_bytes = union_bytes(&asset_complement);
    let main_spans = subtract_spans(executable_spans(main_image, main_start)?, &cuts);
    let main_executable = union_bytes(&main_spans);
    let call_target_spans = subtract_spans(main_executable_spans(main_image, main_start)?, &cuts);
    let call_target_executable = union_bytes(&call_target_spans);
    Ok(json!({
        "format": "alchemy-executable-audit-v1",
        "target": target.id.as_str(),
        "state": "candidate",
        "scope": "code-overlays",
        "audit": "candidate",
        "resources": resource_rows,
        "overlays": overlays,
        "main": {
            "start": main_start,
            "end": main_end,
            "candidate_executable_bytes": main_executable,
            "intervals": main_spans.into_iter().map(|span| json!({
                "start": span.start,
                "end": span.end,
                "kind": span.kind,
            })).collect::<Vec<_>>(),
            "methods": {
                "prologue_sweep": {"candidate_executable_bytes": main_executable},
                "call_targets": {
                    "candidate_executable_bytes": call_target_executable,
                    "intervals": call_target_spans.into_iter().map(|span| json!({
                        "start": span.start,
                        "end": span.end,
                        "kind": span.kind,
                    })).collect::<Vec<_>>(),
                },
                "asset_complement": {
                    "candidate_executable_bytes": asset_complement_bytes,
                    "intervals": asset_complement.into_iter().map(|span| json!({
                        "start": span.start,
                        "end": span.end,
                        "kind": span.kind,
                    })).collect::<Vec<_>>(),
                },
            },
            "asset_cut_bytes": intersection_bytes(&cuts, &[(main_start, main_end)]),
        },
        "summary": {
            "overlays": overlays.len(),
            "decoded_bytes": decoded_total,
            "encoded_bytes": encoded_total,
            "executable_bytes": executable_total,
            "main_candidate_executable_bytes": main_executable,
            "excluded_bytes": decoded_total - executable_total,
        },
        "derivation": [
            "resource-directory streams that decode to the target's fixed entry-veneer shape",
            "only streams whose traced encoding reproduces the ROM receive physical byte extents; raw directory pointers are not files",
            "instructions proved by the byte-identical overlay decoder",
            "PC-relative literal targets, fixed ldr/bx veneers, and bounded executable alignment"
        ]
    }))
}

fn calibrate(root: &Path, document: &Value) -> Result<String, String> {
    let path = root.join("games/THE BROKEN SEAL/metrics/executable.json");
    let expected: ExpectedReport = serde_json::from_slice(
        &std::fs::read(&path).map_err(|error| format!("{}: {error}", path.display()))?,
    )
    .map_err(|error| format!("{}: {error}", path.display()))?;
    let actual_main = document["main"]["intervals"]
        .as_array()
        .ok_or("audit report has no main intervals")?
        .iter()
        .filter_map(|row| Some((row["start"].as_i64()?, row["end"].as_i64()?)))
        .collect::<Vec<_>>();
    let expected_main = expected
        .main
        .intervals
        .iter()
        .map(|span| (span.start, span.end))
        .collect::<Vec<_>>();
    let actual_main_bytes = document["main"]["candidate_executable_bytes"]
        .as_u64()
        .ok_or("audit report has no main byte total")?;
    let overlap = intersection_bytes(&actual_main, &expected_main);
    let false_positive = actual_main_bytes - overlap;
    let missed = expected.main.executable_bytes - overlap;
    let mut missed_by_kind = std::collections::BTreeMap::<&str, u64>::new();
    for span in &expected.main.intervals {
        let size = (span.end - span.start) as u64;
        let found = intersection_bytes(&actual_main, &[(span.start, span.end)]);
        *missed_by_kind.entry(&span.kind).or_default() += size - found;
    }
    let missed_by_kind = missed_by_kind
        .into_iter()
        .map(|(kind, bytes)| format!("{kind}={bytes}"))
        .collect::<Vec<_>>()
        .join(",");
    let call_target_main = document["main"]["methods"]["call_targets"]["intervals"]
        .as_array()
        .ok_or("audit report has no call-target intervals")?
        .iter()
        .filter_map(|row| Some((row["start"].as_i64()?, row["end"].as_i64()?)))
        .collect::<Vec<_>>();
    let call_target_bytes = document["main"]["methods"]["call_targets"]
        ["candidate_executable_bytes"]
        .as_u64()
        .ok_or("audit report has no call-target byte total")?;
    let call_target_overlap = intersection_bytes(&call_target_main, &expected_main);
    let call_target_false = call_target_bytes - call_target_overlap;
    let call_target_missed = expected.main.executable_bytes - call_target_overlap;
    let asset_complement = document["main"]["methods"]["asset_complement"]["intervals"]
        .as_array()
        .ok_or("audit report has no asset-complement intervals")?
        .iter()
        .filter_map(|row| Some((row["start"].as_i64()?, row["end"].as_i64()?)))
        .collect::<Vec<_>>();
    let asset_complement_bytes = document["main"]["methods"]["asset_complement"]
        ["candidate_executable_bytes"]
        .as_u64()
        .ok_or("audit report has no asset-complement byte total")?;
    let asset_complement_overlap = intersection_bytes(&asset_complement, &expected_main);
    let asset_complement_false = asset_complement_bytes - asset_complement_overlap;
    let asset_complement_missed = expected.main.executable_bytes - asset_complement_overlap;
    let mut largest_call_false = difference_ranges(&call_target_main, &expected_main);
    largest_call_false.sort_by_key(|(start, end)| std::cmp::Reverse(end - start));
    let largest_call_false = largest_call_false
        .into_iter()
        .take(8)
        .map(|(start, end)| format!("0x{start:08x}-0x{end:08x} ({} bytes)", end - start))
        .collect::<Vec<_>>()
        .join(", ");
    let mut largest_false = difference_ranges(&actual_main, &expected_main);
    largest_false.sort_by_key(|(start, end)| std::cmp::Reverse(end - start));
    let largest_false = largest_false
        .into_iter()
        .take(8)
        .map(|(start, end)| format!("0x{start:08x}-0x{end:08x} ({} bytes)", end - start))
        .collect::<Vec<_>>()
        .join(", ");
    let actual = document["overlays"]
        .as_array()
        .ok_or("audit report has no overlays")?;
    let mut mismatches = Vec::new();
    let mut overlay_false = 0u64;
    let mut overlay_missed = 0u64;
    let mut generated_false = 0u64;
    let mut generated_missed = 0u64;
    let mut combined_false = 0u64;
    let mut combined_missed = 0u64;
    for row in expected.overlays {
        let found = actual.iter().find(|actual| actual["id"] == row.id);
        let bytes = found.and_then(|actual| actual["executable_bytes"].as_u64());
        if let Some(found) = found {
            let found_spans = found["intervals"]
                .as_array()
                .into_iter()
                .flatten()
                .filter_map(|span| Some((span["start"].as_i64()?, span["end"].as_i64()?)))
                .collect::<Vec<_>>();
            let expected_spans = row
                .intervals
                .iter()
                .map(|span| (span.start, span.end))
                .collect::<Vec<_>>();
            let generated_spans = found["generated_intervals"]
                .as_array()
                .into_iter()
                .flatten()
                .filter_map(|span| Some((span["start"].as_i64()?, span["end"].as_i64()?)))
                .collect::<Vec<_>>();
            let generated_bytes = found["generated_executable_bytes"].as_u64().unwrap_or(0);
            let generated_common = intersection_bytes(&generated_spans, &expected_spans);
            let generated_false_bytes = generated_bytes.saturating_sub(generated_common);
            let generated_missed_bytes = row.executable_bytes.saturating_sub(generated_common);
            generated_false += generated_false_bytes;
            generated_missed += generated_missed_bytes;
            let trusted_spans = found["trusted_intervals"]
                .as_array()
                .into_iter()
                .flatten()
                .filter_map(|span| Some((span["start"].as_i64()?, span["end"].as_i64()?)))
                .collect::<Vec<_>>();
            let combined_spans = generated_spans
                .iter()
                .chain(&trusted_spans)
                .copied()
                .collect::<Vec<_>>();
            let combined_bytes = found["combined_executable_bytes"].as_u64().unwrap_or(0);
            let combined_common = intersection_bytes(&combined_spans, &expected_spans);
            let combined_false_bytes = combined_bytes.saturating_sub(combined_common);
            let combined_missed_bytes = row.executable_bytes.saturating_sub(combined_common);
            let mut combined_false_ranges = difference_ranges(&combined_spans, &expected_spans);
            combined_false_ranges.sort_by_key(|(start, end)| std::cmp::Reverse(end - start));
            let mut combined_missed_ranges = difference_ranges(&expected_spans, &combined_spans);
            combined_missed_ranges.sort_by_key(|(start, end)| std::cmp::Reverse(end - start));
            combined_false += combined_false_bytes;
            combined_missed += combined_missed_bytes;
            let common = intersection_bytes(&found_spans, &expected_spans);
            let false_bytes = bytes.unwrap_or(0).saturating_sub(common);
            let missed_bytes = row.executable_bytes.saturating_sub(common);
            overlay_false += false_bytes;
            overlay_missed += missed_bytes;
            if bytes != Some(row.executable_bytes) || false_bytes != 0 || missed_bytes != 0 {
                let mut false_ranges = difference_ranges(&found_spans, &expected_spans);
                false_ranges.sort_by_key(|(start, end)| std::cmp::Reverse(end - start));
                let mut missed_ranges = difference_ranges(&expected_spans, &found_spans);
                missed_ranges.sort_by_key(|(start, end)| std::cmp::Reverse(end - start));
                let mut generated_false_ranges =
                    difference_ranges(&generated_spans, &expected_spans);
                generated_false_ranges.sort_by_key(|(start, end)| std::cmp::Reverse(end - start));
                let mut generated_missed_ranges =
                    difference_ranges(&expected_spans, &generated_spans);
                generated_missed_ranges.sort_by_key(|(start, end)| std::cmp::Reverse(end - start));
                let summarize = |ranges: &[(i64, i64)]| {
                    ranges
                        .iter()
                        .take(3)
                        .map(|(start, end)| format!("0x{start:08x}-0x{end:08x}({})", end - start))
                        .collect::<Vec<_>>()
                        .join(",")
                };
                mismatches.push(format!(
                    "{} expected {} got {} false {} [{}] missed {} [{}] generated false {} [{}] missed {} [{}] combined false {} [{}] missed {} [{}]",
                    row.id,
                    row.executable_bytes,
                    bytes.map_or("missing".into(), |value| value.to_string()),
                    false_bytes,
                    summarize(&false_ranges),
                    missed_bytes,
                    summarize(&missed_ranges),
                    generated_false_bytes,
                    summarize(&generated_false_ranges),
                    generated_missed_bytes,
                    summarize(&generated_missed_ranges),
                    combined_false_bytes,
                    summarize(&combined_false_ranges),
                    combined_missed_bytes,
                    summarize(&combined_missed_ranges)
                ));
            }
        } else {
            overlay_missed += row.executable_bytes;
            mismatches.push(format!(
                "{} expected {} got {}",
                row.id,
                row.executable_bytes,
                bytes.map_or("missing".into(), |value| value.to_string())
            ));
        }
    }
    if !mismatches.is_empty() {
        return Err(format!(
            "TBS calibration failed: asset-complement main overlap {asset_complement_overlap}/{}, false-positive {asset_complement_false}, missed {asset_complement_missed}; prologue main overlap {overlap}/{}, false-positive {false_positive}, missed {missed} ({missed_by_kind}); call/pointer main overlap {call_target_overlap}/{}, false-positive {call_target_false}, missed {call_target_missed}; largest prologue false spans: {largest_false}; largest call/pointer false spans: {largest_call_false}; overlay source false-positive {overlay_false}, missed {overlay_missed}, mismatches {}/{}; generated-source false-positive {generated_false}, missed {generated_missed}; combined false-positive {combined_false}, missed {combined_missed}:\n{}",
            expected.main.executable_bytes,
            expected.main.executable_bytes,
            expected.main.executable_bytes,
            mismatches.len(),
            actual.len(),
            mismatches.join("\n")
        ));
    }
    if asset_complement_false != 0 || asset_complement_missed != 0 {
        return Err(format!(
            "TBS main calibration failed: asset-complement overlap {asset_complement_overlap}/{}, false-positive {asset_complement_false}, missed {asset_complement_missed}; prologue overlap {overlap}/{}, false-positive {false_positive}, missed {missed} ({missed_by_kind}); call/pointer overlap {call_target_overlap}/{}, false-positive {call_target_false}, missed {call_target_missed}; largest prologue false spans: {largest_false}; largest call/pointer false spans: {largest_call_false}",
            expected.main.executable_bytes,
            expected.main.executable_bytes,
            expected.main.executable_bytes
        ));
    }
    Ok(format!("calibration=exact main overlays={}", actual.len()))
}

pub fn run(root: &Path, arguments: &[String]) -> Result<String, String> {
    let options = parse(arguments)?;
    if options.help {
        return Ok(USAGE.into());
    }
    if options.all {
        if !options.data || options.calibrate || options.output.is_some() {
            return Err(format!(
                "--all requires --data and accepts no other mode\n{USAGE}"
            ));
        }
        let canonical = [DecompTargetId::TbsEn, DecompTargetId::TlaEn];
        let mut reports = Vec::with_capacity(TARGET_IDS.len());
        for id in canonical
            .into_iter()
            .chain(TARGET_IDS.into_iter().filter(|id| !canonical.contains(id)))
        {
            reports.push(index::run(root, target_for(id))?);
        }
        return Ok(reports.join("\n"));
    }
    let target = decomp_target(options.target.as_deref())?;
    if options.data {
        if options.calibrate || options.output.is_some() {
            return Err("--data writes out/<target>/reports/rom-index.json; cannot combine with --calibrate or --output".into());
        }
        return index::run(root, target);
    }
    if options.calibrate && target.id.as_str() != "tbs-en" {
        return Err("--calibrate is the TBS completed-audit gate; use --target tbs-en".into());
    }
    let document = report(root, target)?;
    let calibration = options
        .calibrate
        .then(|| calibrate(root, &document))
        .transpose()?;
    if let Some(path) = options.output {
        let path = if path.is_absolute() {
            path
        } else {
            root.join(path)
        };
        if let Some(parent) = path.parent() {
            std::fs::create_dir_all(parent).map_err(|error| error.to_string())?;
        }
        std::fs::write(&path, canonical_json(&document))
            .map_err(|error| format!("{}: {error}", path.display()))?;
    }
    Ok(calibration.unwrap_or_else(|| {
        format!(
            "target={} overlays={} executable={} decoded={}",
            target.id,
            document["summary"]["overlays"],
            document["summary"]["executable_bytes"],
            document["summary"]["decoded_bytes"]
        )
    }))
}

#[cfg(test)]
mod tests {
    use super::{difference_ranges, intersection_bytes, parse, source_spans, union_bytes};
    use crate::overlay::assembly::ExecutableSpan;

    #[test]
    fn all_data_is_the_single_twelve_target_entry_point() {
        let options = parse(&["--all".into(), "--data".into()]).unwrap();
        assert!(options.all && options.data && options.target.is_none());
        assert!(parse(&["--all".into(), "--target".into(), "tbs-en".into()]).is_err());
        assert!(parse(&["--data".into()]).is_err());
    }

    #[test]
    fn overlapping_kinds_are_counted_once() {
        let spans = [
            ExecutableSpan {
                start: 0,
                end: 8,
                kind: "veneer",
            },
            ExecutableSpan {
                start: 0,
                end: 4,
                kind: "thumb",
            },
            ExecutableSpan {
                start: 10,
                end: 12,
                kind: "thumb",
            },
        ];
        assert_eq!(union_bytes(&spans), 10);
    }

    #[test]
    fn calibration_counts_overlap_without_double_counting() {
        assert_eq!(
            intersection_bytes(&[(0, 8), (4, 12)], &[(2, 6), (10, 14)]),
            6
        );
    }

    #[test]
    fn asset_complement_splits_at_every_verified_region() {
        assert_eq!(
            difference_ranges(&[(0, 20)], &[(2, 4), (8, 12), (10, 16)]),
            vec![(0, 2), (4, 8), (16, 20)]
        );
    }

    #[test]
    fn narrow_branch_target_recovers_an_instruction_after_a_literal_pool() {
        let file = tempfile::NamedTempFile::new().unwrap();
        std::fs::write(file.path(), ".syntax unified\n.thumb\nldr r0, [pc, #0]\nb.n .L_next\n.4byte 0xffffffff\n.L_next:\n.2byte 0x2800\nmovs r0, #1\nbx lr\n").unwrap();
        let image = [
            0x00, 0x48, 0x01, 0xe0, 0xff, 0xff, 0xff, 0xff, 0x00, 0x28, 0x01, 0x20, 0x70, 0x47,
        ];
        let spans = source_spans(file.path(), &image, "synthetic").unwrap();
        assert_eq!(union_bytes(&spans), 14);
    }

    #[test]
    fn fixed_veneer_can_enter_arm_iwram_code() {
        let file = tempfile::NamedTempFile::new().unwrap();
        std::fs::write(
            file.path(),
            ".syntax unified\n.thumb\n.4byte 0x47204c00\n.4byte 0x03000000\n",
        )
        .unwrap();
        let image = [0x00, 0x4c, 0x20, 0x47, 0x00, 0x00, 0x00, 0x03];
        let spans = source_spans(file.path(), &image, "synthetic").unwrap();
        assert_eq!(union_bytes(&spans), 8);
        assert_eq!(spans[0].kind, "veneer");
    }
}
